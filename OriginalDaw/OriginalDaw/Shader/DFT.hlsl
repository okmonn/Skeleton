// ���[�g�V�O�l�`���̐錾
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u1, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u2, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
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

// ���̓f�[�^
RWStructuredBuffer<float> input : register(u0);
// ������
RWStructuredBuffer<float> real : register(u1);
// ������
RWStructuredBuffer<float> imag : register(u2);


// �~����
#define PI 3.14159265f

// �n�j���O
float Haninng(uint i, uint size)
{
    float tmp = 0.0f;

    tmp = (size % 2 == 0) ?
		//����
		0.5f - 0.5f * cos(2.0f * PI * i / size) :
		//�
		0.5f - 0.5f * cos(2.0f * PI * (i + 0.5f) / size);

    return tmp;
}

// ���U�t�[���G�ϊ�
void DFT(uint id)
{
    uint2 size;
    input.GetDimensions(size.x, size.y);

    real[id] = 0.0f;
    imag[id] = 0.0f;

    for (uint i = 0; i < size.x; ++i)
    {
        float re =  cos(2.0f * PI * id * i / size.x);
        float im = -sin(2.0f * PI * id * i / size.x);

        real[id] += re * input[i] * Haninng(i, size.x);
        imag[id] += im * input[i] * Haninng(i, size.x);
    }

    real[id] = round(real[id]);
    imag[id] = round(imag[id]);

    //real[id] = lerp(real[id], 0.0f,
    //                step(true, real[id] == -0.0f));
    //imag[id] = lerp(imag[id], 0.0f,
    //                step(true, imag[id] == -0.0f));
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void CS(uint3 gID : SV_GroupID, uint3 gtID : SV_GroupThreadID, uint3 disID : SV_DispatchThreadID)
{
    DFT(gID.x);

    AllMemoryBarrierWithGroupSync();
}