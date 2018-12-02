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

// パラメータ
cbuffer Param : register(b0)
{
    //減衰率
    float attenuation;
    //遅延時間
    float time;
    //ループ回数
    int loop;
    //全体の波形数の相対数
    uint waveIndex;
    //サンプリング周波数
    int sample;
};

// 適応前データ
RWStructuredBuffer<float> origin : register(u0);
// 適応データ
RWStructuredBuffer<float> real : register(u1);

#define PI 3.14159265f

// ディレイ
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

// ディストーション
void Distortion(uint index)
{
    //増幅率
    float gain = 10.0f;
    //音量レベル
    float level = 0.5f;

    real[index] = origin[index] * gain;
    
    //クリッピング
    if(real[index] >= 0.0f)
    {
        real[index] = atan(real[index]) / (PI / 2.0f);
    }
    else if(real[index] < -0.0f)
    {
        real[index] = atan(real[index]) / (PI / 2.0f) * 0.1f;
    }

    //音量調節
    real[index] *= level;
}

// コンプレッサ
void Compressor(uint index)
{
    //しきい値
    float threshold = 0.2f;
    //レシオ
    float ratio = 1.0f / 10.0f;
    //増幅率
    float gain = 1.0f / (threshold + (1.0f - threshold) * ratio);

    real[index] = origin[index];

    //振幅の圧縮
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

// リミッタ
void Limiter(uint index)
{
    //しきい値
    float threshold = 0.2f;
    //レシオ
    float ratio = 1.0f / 10.0f;

    real[index] = origin[index];

    //振幅の圧縮
    if (real[index] > threshold)
    {
        real[index] = threshold;
    }
    else if (real[index] < -threshold)
    {
        real[index] = -threshold;
    }
}

// フェード
void Fade(uint index)
{
    real[index] = origin[index];
    uint2 size;
    origin.GetDimensions(size.x, size.y);

    //適応時間
    float duration = 0.5f;
    if(index < size.x * duration)
    {
        real[index] *= index / (size.x * duration);
        real[size.x - index - 1] *= index / (size.x * duration);
    }
}

// ハニング窓
float Hanning(int index, uint size)
{
    return (size % 2 == 0) ?
    //偶数
    0.5f - 0.5f * cos(2.0f * PI * index / size) :
    //奇数
    0.5f - 0.5f * cos(2.0f * PI * (index + 0.5f) / size);
}

// シンク関数
float Sinc(float index)
{
    return (index == 0.0f) ? 1.0f : sin(index) / index;
}

// FIRローパスフィルタ
void FIR_LPF(uint index)
{
    //エッジ周波数
    float edge = 1000.0f / sample;
    //遷移帯域幅
    float delta = 1000.0f / sample;
    //遅延器の数
    int num = (int) (3.1f / delta + 0.5f) - 1;
    if(num % 2 != 0)
    {
        ++num;
    }

    float data[32];
    int offset = num / 2;
    for (int i = -offset; i <= offset; ++i)
    {
        data[offset + i] = 2.0f * edge * Sinc(2.0f * PI * edge * i);
    }

    for (int n = 0; n <= num; ++n)
    {
        data[n] *= Hanning(n, num + 1);
        real[index] += (index - n >= 0) ? data[n] * origin[index - n] : 0.0f;
    }

    //クリッピング
    if(real[index] > 1.0f)
    {
        real[index] = 1.0f;
    }
    else if(real[index] < -1.0f)
    {
        real[index] = -1.0f;
    }
}

// FIRハイパスフィルタ
void FIR_HPF(uint index)
{
    //エッジ周波数
    float edge = 1000.0f / sample;
    //遷移帯域幅
    float delta = 1000.0f / sample;
    //遅延器の数
    int num = (int) (3.1f / delta + 0.5f) - 1;
    if (num % 2 != 0)
    {
        ++num;
    }

    float data[32];
    int offset = num / 2;
    for (int i = -offset; i <= offset; ++i)
    {
        data[offset + i] = Sinc(PI * i) - 2.0f * edge * Sinc(2.0f * PI * edge * i);
    }

    for (int n = 0; n <= num; ++n)
    {
        data[n] *= Hanning(n, num + 1);
        real[index] += (index - n >= 0) ? data[n] * origin[index - n] : 0.0f;
    }

     //クリッピング
    if (real[index] > 1.0f)
    {
        real[index] = 1.0f;
    }
    else if (real[index] < -1.0f)
    {
        real[index] = -1.0f;
    }
}

// トレモロ
void Tremolo(uint index)
{
    //変調深度
    float depth = 0.5f;
    //変調周波数
    float rate = 5.0f;

    //変調信号
    float signal = 1.0f + depth * sin((2.0f * PI * rate * index) / sample);

    real[index] = signal * origin[index];
}

// ビブラート
void Vibrato(uint index)
{
     //変調深度
    float depth = sample * 0.002f;
    //変調周波数
    float rate = 5.0f;

    float tau = (sample * 0.002f) + depth * sin((2.0f * PI * rate * index) / sample);

    float t = (float) index - tau;
    int m = (int) t;
    float delta = t - (float) m;

    uint2 size;
    origin.GetDimensions(size.x, size.y);

    if (m >= 0 && m + 1 < size.x)
    {
        real[index] = delta * origin[m + 1] + (1.0f - delta) * origin[m];
    }
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void CS(uint3 gID : SV_GroupID, uint3 gtID : SV_GroupThreadID, uint3 disID : SV_DispatchThreadID)
{
    Fade(gID.x);

    AllMemoryBarrierWithGroupSync();
}