// ���[�g�V�O�l�`���̐錾
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u1, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "StaticSampler(s0, "\
                                  "filter = FILTER_MIN_MAG_MIP_LINEAR, "\
                                  "addressU = TEXTURE_ADDRESS_WRAP, "\
                                  "addressV = TEXTURE_ADDRESS_WRAP, "\
                                  "addressW = TEXTURE_ADDRESS_WRAP, "\
                                  "mipLodBias = 0.0f, "\
                                  "maxAnisotropy = 0, "\
                                  "comparisonFunc = COMPARISON_NEVER, "\
                                  "borderColor = STATIC_BORDER_COLOR_TRANSPARENT_BLACK, "\
                                  "minLOD = 0.0f, "\
                                  "maxLOD = 3.402823466e+38f, "\
                                  "space = 0, "\
                                  "visibility = SHADER_VISIBILITY_ALL)"

// �p�����[�^
cbuffer Param : register(b0)
{
    //������
    float attenuation;
    //�x������
    float time;
    //���[�v��
    int loop;
    //�S�̂̔g�`���̑��ΐ�
    uint waveIndex;
    //�T���v�����O���g��
    int sample;
};

// �K���O�f�[�^
RWStructuredBuffer<float> origin : register(u0);
// �K���f�[�^
RWStructuredBuffer<float> real : register(u1);

#define PI 3.14159265f

// �f�B���C
void Delay(uint index)
{
    real[index] = origin[index];
    uint2 size;
    origin.GetDimensions(size.x, size.y);

    uint num = size.x * waveIndex + index;

    for (int i = 1; i <= loop; ++i)
    {
        int m = (int) (num - i * (sample * time));

        real[index] += (m >= 0) ? pow(attenuation, i) * origin[index] : 0.0f;
    }
}

// �f�B�X�g�[�V����
void Distortion(uint index)
{
    //������
    float gain = 10.0f;
    //���ʃ��x��
    float level = 0.5f;

    real[index] = origin[index] * gain;
    
    //�N���b�s���O
    if(real[index] >= 0.0f)
    {
        real[index] = atan(real[index]) / (PI / 2.0f);
    }
    else if(real[index] < -0.0f)
    {
        real[index] = atan(real[index]) / (PI / 2.0f) * 0.1f;
    }

    //���ʒ���
    real[index] *= level;
}

// �R���v���b�T
void Compressor(uint index)
{
    //�������l
    float threshold = 0.2f;
    //���V�I
    float ratio = 1.0f / 10.0f;
    //������
    float gain = 1.0f / (threshold + (1.0f - threshold) * ratio);

    real[index] = origin[index];

    //�U���̈��k
    if (real[index] > threshold)
    {
        real[index] = threshold + (real[index] - threshold) * ratio;
    }
    else if (real[index] < -threshold)
    {
        real[index] = -threshold + (real[index] + threshold) * ratio;
    }
    
    real[index] *= gain;
}

// ���~�b�^
void Limiter(uint index)
{
    //�������l
    float threshold = 0.2f;
    //���V�I
    float ratio = 1.0f / 10.0f;

    real[index] = origin[index];

    //�U���̈��k
    if (real[index] > threshold)
    {
        real[index] = threshold;
    }
    else if (real[index] < -threshold)
    {
        real[index] = -threshold;
    }
}

// �t�F�[�h
void Fade(uint index)
{
    real[index] = origin[index];
    uint2 size;
    origin.GetDimensions(size.x, size.y);

    //�K������
    float duration = 0.5f;
    if(index < size.x * duration)
    {
        real[index] *= index / (size.x * duration);
        real[size.x - index - 1] *= index / (size.x * duration);
    }
}

// �t�B���^�p���̓o�b�t�@
groupshared float input[2];
// �t�B���^�p�o�̓o�b�t�@
groupshared float output[2];

// IIR ���[�p�X
void LowPass(uint index)
{
    float cutoff = 20000.0f;
    float q = 1.0f / sqrt(2.0f);

    //�t�B���^�W���v�Z�Ŏg�p���钆�Ԓl�����߂�B
    float omega = 2.0f * PI * cutoff / sample;
    float alpha = sin(omega) / (2.0f * q);

	//�t�B���^�W�������߂�B
    float a0 =  1.0f + alpha;
    float a1 = -2.0f * cos(omega);
    float a2 =  1.0f - alpha;
    float b0 = (1.0f - cos(omega)) / 2.0f;
    float b1 =  1.0f - cos(omega);
    float b2 = (1.0f - cos(omega)) / 2.0f;

    real[index] = (b0 / a0 * origin[index]) + (b1 / a0 * input[0]) + (b2 / a0 * input[1]) - (a1 / a0 * output[0]) - (a2 / a0 * output[1]);

    //���̓o�b�t�@
    input[1] = input[0];
    input[0] = origin[index];

    //�o�̓o�b�t�@
    output[1] = output[0];
    output[0] = real[index];

    //�v�Z���I���܂łق��̃X���b�h�͑ҋ@
    GroupMemoryBarrierWithGroupSync();
}

// �g������
void Tremolo(uint index)
{
    //�ϒ��[�x
    float depth = 0.5f;
    //�ϒ����g��
    float rate = 5.0f;

    //�ϒ��M��
    float signal = 1.0f + depth * sin((2.0f * PI * rate * index) / sample);

    real[index] = signal * origin[index];
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void CS(uint3 gID : SV_GroupID, uint3 gtID : SV_GroupThreadID, uint3 disID : SV_DispatchThreadID)
{
    LowPass(gID.x);

    AllMemoryBarrierWithGroupSync();
}