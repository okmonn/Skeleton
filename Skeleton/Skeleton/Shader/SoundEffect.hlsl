// ルートシグネチャの宣言
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
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

// パラメータ
cbuffer Param : register(b0)
{
    float sample;
    float gain;
    float volume;
    float depth;
    float rate;
    float left;
    float right;
}

// 適応前データ
RWStructuredBuffer<float> origin : register(u0);
// 適応データ
RWStructuredBuffer<float> real   : register(u1);

#define PI 3.14159265f

// トレモロ
void Tremolo(uint index)
{
    //変調信号
    float signal = 1.0f + depth * sin((2.0f * PI * rate * index) / sample);

    real[index] *= signal;
}

// ディストーション
void Distortion(uint index)
{
    real[index] *= gain;
    if (real[index] > 1.0f)
    {
        real[index] = 1.0f;
    }
    else if (real[index] < -1.0f)
    {
        real[index] = -1.0f;
    }
}

// パンニング
void Pann(uint index)
{
    int flag = index % 2 == 0;
    real[index] = lerp(origin[index - 1] + origin[index] * right, origin[index] + origin[index + 1] * left, step(true, flag));
}

// ボリューム
void Volume(uint index)
{
    real[index] *= volume;
}

// コンプレッサ
void Compressor(uint index)
{
    //しきい値
    float threshold = 0.2f;
    ///レシオ
    float ratio = 1.0f / 10.0f;

    if(real[index] > threshold)
    {
        real[index] = threshold + (real[index] - threshold) * ratio;

    }
    else if(real[index] < -threshold)
    {
        real[index] = -threshold + (real[index] + threshold) * ratio;
    }
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void CS(uint3 gID : SV_GroupID, uint3 gtID : SV_GroupThreadID, uint3 disID : SV_DispatchThreadID)
{
    real[gID.x] = origin[gID.x];
    Pann(gID.x);
    //Tremolo(gID.x);
    //Distortion(gID.x);
    //Volume(gID.x);

    AllMemoryBarrierWithGroupSync();
}