// ルートシグネチャの宣言
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

// 適応前データ
RWStructuredBuffer<float> origin : register(u0);
// 適応データ
RWStructuredBuffer<float> real   : register(u1);

#define PI 3.14159265f

// ディストーション
void Distortion(uint index)
{
    //増幅率
    float gain = 5.0f;
    //音量レベル
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

// フィルタ用入力バッファ
groupshared float input[2];
// フィルタ用出力バッファ
groupshared float output[2];

// IIR ローパス
void LowPass(uint index)
{
    float cutoff = 44100.0f / 4.0f;
    float q = 1.0f / sqrt(2.0f);

    //フィルタ係数計算で使用する中間値
    float omega = 2.0f * PI * cutoff / 44100.0f;
    float alpha = sin(omega) / (2.0f * q);

	//フィルタ係数
    float a0 =  1.0f + alpha;
    float a1 = -2.0f * cos(omega);
    float a2 =  1.0f - alpha;
    float b0 = (1.0f - cos(omega)) / 2.0f;
    float b1 =  1.0f - cos(omega);
    float b2 = (1.0f - cos(omega)) / 2.0f;

    real[index] = (b0 / a0 * origin[index]) + (b1 / a0 * input[0]) + (b2 / a0 * input[1]) - (a1 / a0 * output[0]) - (a2 / a0 * output[1]);

    //入力バッファ
    input[1] = input[0];
    input[0] = origin[index];

    //出力バッファ
    output[1] = output[0];
    output[0] = real[index];

    //計算が終わるまでほかのスレッドは待機
    GroupMemoryBarrierWithGroupSync();
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void CS(uint3 gID : SV_GroupID, uint3 gtID : SV_GroupThreadID, uint3 disID : SV_DispatchThreadID)
{
    LowPass(gID.x);

    AllMemoryBarrierWithGroupSync();
}