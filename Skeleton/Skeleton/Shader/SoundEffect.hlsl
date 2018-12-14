// ���[�g�V�O�l�`���̐錾
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u1, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "StaticSampler(s0, "\
                                  "filter         = FILTER_MIN_MAG_MIP_LINEAR,"\
                                  "addressU       = TEXTURE_ADDRESS_WRAP,"\
                                  "addressV       = TEXTURE_ADDRESS_WRAP,"\
                                  "addressW       = TEXTURE_ADDRESS_WRAP,"\
                                  "mipLodBias     = 0.0f,"\
                                  "maxAnisotropy  = 0,"\
                                  "comparisonFunc = COMPARISON_NEVER,"\
                                  "borderColor    = STATIC_BORDER_COLOR_TRANSPARENT_BLACK,"\
                                  "minLOD         = 0.0f,"\
                                  "maxLOD         = 3.402823466e+38f,"\
                                  "space          = 0,"\
                                  "visibility     = SHADER_VISIBILITY_ALL)"

// �K���O�f�[�^
RWStructuredBuffer<float> origin : register(u0);
// �K���f�[�^
RWStructuredBuffer<float> real   : register(u1);

#define PI 3.14159265f

// �f�B�X�g�[�V����
void Distortion(uint index)
{
    //������
    float gain = 5.0f;
    //���ʃ��x��
    float level = 0.7f;

    real[index] = origin[index] * gain;
    if (real[index] > 1.0f)
    {
        real[index] = 1.0f;
    }
    else if (real[index] < -1.0f)
    {
        real[index] = -1.0f;
    }
}

// �t�B���^�p���̓o�b�t�@ 
groupshared float input[2];
// �t�B���^�p�o�̓o�b�t�@
groupshared float output[2];

// IIR ���[�p�X
void LowPass(uint index)
{
    float cutoff = 10000.0f;
    float q = 1.0f / sqrt(2.0f);

    //�t�B���^�W���v�Z�Ŏg�p���钆�Ԓl
    float omega = 2.0f * PI * cutoff / 44100.0f;
    float alpha = sin(omega) / (2.0f * q);

	//�t�B���^�W��
    float a0 =  1.0f + alpha;
    float a1 = -2.0f * cos(omega);
    float a2 =  1.0f - alpha;
    float b0 = (1.0f - cos(omega)) / 2.0f;
    float b1 =  1.0f - cos(omega);
    float b2 = (1.0f - cos(omega)) / 2.0f;

    real[index] = (b0 / a0) * origin[index] + (b1 / a0) * input[0] + (b2 / a0) * input[1] - (a1 / a0) * output[0] - (a2 / a0) * output[1];

    //���̓o�b�t�@
    input[1] = input[0];
    input[0] = origin[index];

    //�o�̓o�b�t�@
    output[1] = output[0];
    output[0] = real[index];

    //�v�Z���I���܂łق��̃X���b�h�͑ҋ@
    GroupMemoryBarrierWithGroupSync();
}

float Hanning(int index, uint size)
{
    float tmp = 0.0f;

    tmp = (size % 2 == 0) ?
				//����
				0.5f - 0.5f * cos(2.0f * 3.14159265f * (float) index / (float) size) :
				//�
				0.5f - 0.5f * cos(2.0f * 3.14159265f * ((float) index + 0.5f) / (float) size);

    if (tmp == 0.0f)
    {
        tmp = 1.0f;
    }

    return tmp;
}

float Sinc(float x)
{
    return (x == 0.0f) ? 1.0f : sin(x) / x;
}

void LowPass2(uint index)
{
    real[index] = 0.0f;

    float fe = 10000.0f / 44100.0f; /* �G�b�W���g�� */
    float delta = 1000.0f / 44100.0f; /* �J�ڑш敝 */
    int J = (int) (3.1f / delta + 0.5f) - 1; /* �x����̐� */
    if (J % 2 == 1)
    {
        J++; /* J+1����ɂȂ�悤�ɒ������� */
    }
    int m = 0;
    float b[137];
    int offset = J / 2;
    for (m = -J / 2; m <= J / 2; m++)
    {
        b[offset + m] = 2.0 * fe * Sinc(2.0 * 3.14159265f * fe * m);
    }
    for (m = 0; m <= J; m++)
    {
        b[m] *= Hanning(m, J + 1);
        if (index - m >= 0)
        {
            real[index] += b[m] * origin[index - m];
        }
    }
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void CS(uint3 gID : SV_GroupID, uint3 gtID : SV_GroupThreadID, uint3 disID : SV_DispatchThreadID)
{
    //LowPass(gID.x);
    real[gID.x] = origin[gID.x];

    //AllMemoryBarrierWithGroupSync();
}