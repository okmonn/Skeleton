// ルートシグネチャの宣言
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

// 適応前データ
RWStructuredBuffer<float> input  : register(u0);
// 適応データ
RWStructuredBuffer<float> output : register(u1);

// 円周率
#define PI 3.14159265f

// パラメータ
cbuffer Param : register(b0)
{
    //音量
    float volume;
    //増幅率
    float amp;
    //音量レベル
    float level;
    //閾値
    float threshold;
    //圧縮比率
    float ratio;
    //ノイズゲート用閾値
    float noiseThd;
};

// ディストーション(ハードクリッピング)
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

// ノイズゲート
void NoiseGate(uint id)
{
    if(output[id] <= noiseThd && output[id] >= -noiseThd)
    {
        output[id] = 0.0f;
    }
}

// リミッタ
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

// コンプレッサ
void Compressor(uint id)
{
    //増幅率
    float gain= 1.0f / (threshold + (1.0f - threshold) * ratio);

    //圧縮
    if(output[id] > threshold)
    {
        output[id] = threshold + (output[id] - threshold) * ratio;
    }
    else if(output[id] < -threshold)
    {
        output[id] = -threshold + (input[id] + threshold) * ratio;
    }

    //増幅
    output[id] *= gain;
}

// トレモロ
void Tremolo(uint index)
{
    //変調深度
    float depth = 0.5f;
    //変調周波数
    float rate = 5.0f;

    //変調信号
    float signal = 1.0f + depth * sin((2.0f * PI * rate * index) / 44100.0f);

    output[index] = signal * input[index];
}

// 音量調節
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