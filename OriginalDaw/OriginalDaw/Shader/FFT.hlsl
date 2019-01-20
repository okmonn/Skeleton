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
// 配列インデックス
RWStructuredBuffer<float> index : register(u3);


// 円周率
#define PI 3.14159265f

// ハニング
float Haninng(uint i, uint size)
{
    float tmp = 0.0f;

    tmp = (size % 2 == 0) ?
		//偶数
		0.5f - 0.5f * cos(2.0f * PI * i / size) :
		//奇数
		0.5f - 0.5f * cos(2.0f * PI * (i + 0.5f) / size);

    return tmp;
}

// 高速フーリエ変換
void FFT(uint id)
{
    uint2 size;
    input.GetDimensions(size.x, size.y);

    uint stage = ceil(log2(size.x));
    uint st = id + 1;

    for (uint i = 0; i < pow(2.0f, id); ++i)
    {
        for (uint n = 0; n < pow(2.0f, stage - st); ++n)
        {
            uint index1 = pow(2.0f, stage - st + 1) * i + n;
            uint index2 = pow(2.0f, stage - st) + index1;

            float p = pow(2.0f, id) * n;

            float re0 =  input[index1];
            float im0 =  imag[index1];
            float re1 =  input[index2];
            float im1 =  imag[index2];
            float re2 =  cos((2.0f * PI * p) / size.x);
            float im2 = -sin((2.0f * PI * p) / size.x);

            real[index1] = re0 + re1;
            imag[index1] = im0 + im1;
            if (st < stage)
            {
                real[index2] = (re0 - re1) * re2 - (im0 - im1) * im2;
                imag[index2] = (im0 - im1) * re2 + (re0 - re1) * im2;
            }
            else
            {
                real[index2] = re0 - re1;
                imag[index2] = im0 - im1;
            }
        }

        uint m = pow(2.0f, id) + i;
        index[m] = index[i] + pow(2.0f, stage - st);
    }
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void CS(uint3 gID : SV_GroupID, uint3 gtID : SV_GroupThreadID, uint3 disID : SV_DispatchThreadID)
{
    FFT(gID.x);

    AllMemoryBarrierWithGroupSync();
}