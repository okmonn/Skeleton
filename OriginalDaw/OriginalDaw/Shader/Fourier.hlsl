// ルートシグネチャの宣言
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u1, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u2, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u3, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
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

// 入力データ
RWStructuredBuffer<float> input : register(u0);
// 実数部
RWStructuredBuffer<float> real  : register(u1);
// 虚数部
RWStructuredBuffer<float> imag  : register(u2);
// 並び替え用インデックス
RWStructuredBuffer<float> index : register(u3);

// 円周率
#define PI 3.14159265f

// ハニング
float Hanning(float i, uint size)
{
    float tmp = 0.0f;

    tmp = (size % 2 == 0) ?
		//偶数
		0.5f - 0.5f * cos(2.0f * PI * i / size) :
		//奇数
		0.5f - 0.5f * cos(2.0f * PI * (i + 0.5f) / size);

    if (tmp == 0.0f)
    {
        tmp = 1.0f;
    }

    return tmp;
}

// 離散フーリエ変換
void DFT(uint id)
{
    uint2 size;
    input.GetDimensions(size.x, size.y);

    real[id] = input[id];
    imag[id] = 0.0f;

    for (uint i = 0; i < size.x; ++i)
    {
        float re =  cos(2.0f * PI * id * i / size.x);
        float im = -sin(2.0f * PI * id * i / size.x);

        real[id] += re * (input[i] * Hanning(i, size.x)) - im * 0.0f;
        imag[id] += re * 0.0f + im * (input[i] * Hanning(i, size.x));
    }
}

// 高速フーリエ変換
void FFT(uint id)
{
    float2 size = 0.0f;
    input.GetDimensions(size.x, size.y);

    uint st = id + 1;

    float stage = log2(size.x);
    for (uint i = 0; i < pow(id, 2.0f); ++i)
    {
        for (uint n = 0; n < pow(stage - st, 2.0f); ++n)
        {
            uint index1 = pow(stage - st + 1.0f, 2.0f) * i + n;
            uint index2 = pow(stage - st, 2.0f) + index1;

            float p = pow(id, 2.0f) * n;

            float re1 = input[index1];
            float re2 = input[index2];
            float re3 = cos((2.0f * PI * p) / size.x);

            float im1 = imag[index1];
            float im2 = imag[index2];
            float im3 = -sin((2.0f * PI * p) / size.x);

            if(st < stage)
            {
                real[index1] =  re1 + re2;
                real[index2] = (re1 - re2) * re3 - (im1 - im2) * im3;
                imag[index1] =  im1 + im2;
                imag[index2] = (im1 - im2) * re3 + (re1 - re2) * im3;
            }
            else
            {
                real[index1] = re1 + re2;
                real[index2] = re1 - re2;
                imag[index1] = im1 + im2;
                imag[index2] = im1 - im2;
            }
        }

        index[pow(id, 2.0f) + i] = (uint) index[i] + (uint) pow(stage - st, 2.0f);
    }
}


[RootSignature(RS)]
[numthreads(1, 1, 1)]
void CS(uint3 gID : SV_GroupID, uint3 gtID : SV_GroupThreadID, uint3 disID : SV_DispatchThreadID)
{
    FFT(gID.x);

    AllMemoryBarrierWithGroupSync();
}