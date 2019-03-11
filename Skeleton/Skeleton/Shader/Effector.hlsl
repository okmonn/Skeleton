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

// �K���O�f�[�^
RWStructuredBuffer<float> input  : register(u0);
// �K���f�[�^
RWStructuredBuffer<float> output : register(u1);

// �~����
#define PI 3.14159265f

// �p�����[�^
cbuffer Param : register(b0)
{
    //����
    float volume;
    //������
    float amp;
    //���ʃ��x��
    float level;
    //臒l
    float threshold;
    //���k�䗦
    float ratio;
    //�m�C�Y�Q�[�g�p臒l
    float noiseThd;
};

// �f�B�X�g�[�V����(�n�[�h�N���b�s���O)
void Distortion(uint id)
{
    output[id] *= amp;
    if(output[id] > 1.0f)
    {
        output[id] = 1.0f;
    }
    else if(output[id] < -1.0f)
    {
        output[id] = -1.0f;
    }
}

// �m�C�Y�Q�[�g
void NoiseGate(uint id)
{
    if(output[id] <= noiseThd && output[id] >= -noiseThd)
    {
        output[id] = 0.0f;
    }
}

// ���~�b�^
void Limiter(uint id)
{
    output[id] *= amp;
    if (output[id] > threshold)
    {
        output[id] = threshold;
    }
    else if (output[id] < -threshold)
    {
        output[id] = -threshold;
    }
}

// �R���v���b�T
void Compressor(uint id)
{
    //������
    float gain= 1.0f / (threshold + (1.0f - threshold) * ratio);

    //���k
    if(output[id] > threshold)
    {
        output[id] = threshold + (output[id] - threshold) * ratio;
    }
    else if(output[id] < -threshold)
    {
        output[id] = -threshold + (input[id] + threshold) * ratio;
    }

    //����
    output[id] *= gain;
}

// �g������
void Tremolo(uint index)
{
    //�ϒ��[�x
    float depth = 0.5f;
    //�ϒ����g��
    float rate = 5.0f;

    //�ϒ��M��
    float signal = 1.0f + depth * sin((2.0f * PI * rate * index) / 44100.0f);

    output[index] = signal * input[index];
}

// ���ʒ���
void Volume(uint id)
{
    output[id] *= volume;
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void CS(uint3 gID : SV_GroupID, uint3 gtID : SV_GroupThreadID, uint3 disID : SV_DispatchThreadID)
{
    output[gID.x] = input[gID.x];

    Distortion(gID.x);
    NoiseGate(gID.x);
    Compressor(gID.x);
    Volume(gID.x);

    AllMemoryBarrierWithGroupSync();
}