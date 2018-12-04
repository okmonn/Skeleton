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
    //全体の波形数の相対数
    uint waveIndex;
    //サンプリング周波数
    int sample;

    //LowPass適応フラグ
    bool low;
    //HightPass適応フラグ
    bool hight;
    //BundPass適応フラグ
    bool bund;
    //カットオフ周波数
    float cutoff;
    //クオリティ
    float q;
    //帯域幅
    float octave;
};

// 適応前データ
RWStructuredBuffer<float> origin : register(u0);
// 適応データ
RWStructuredBuffer<float> real : register(u1);

#define PI 3.14159265f

// ディレイ
void Delay(uint index)
{
    //real[index] = origin[index];
    //uint2 size;
    //origin.GetDimensions(size.x, size.y);

    //uint num = size.x * waveIndex + index;

    //for (int i = 1; i <= 10; ++i)
    //{
    //    int m = (int) (num - i * (sample * 0.05f));

    //    real[index] += (m >= 0) ? pow(0.75f, i) * origin[index] : 0.0f;
    //}

    real[index] = 0.0f;

    //減衰率
    float amplitude = 0.5f;
    //遅延時間
    int delayTime = 441;
    //ループ回数
    int loop = 10;

    for (int i = 1; i <= loop; ++i)
    {
        int m = index - i * delayTime;
        if(m >= 0)
        {
            real[index] += pow(amplitude, i) * origin[m];
        }
    }

    if(real[index] > 1.0f)
    {
        real[index] = 1.0f;
    }
}

// ディストーション
void Distortion(uint index)
{
    //増幅率
    float gain = 2.0f;
    //音量レベル
    float level = 0.9f;

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
    
    //real[index] *= gain;
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

// フェードイン
void FadeIn(uint index)
{
    real[index] = origin[index];
    uint2 size;
    origin.GetDimensions(size.x, size.y);

    //適応時間
    float duration = 0.5f;
    if(index < size.x * duration)
    {
        real[index] *= index / (size.x * duration);
    }
}

// フェードアウト
void FadeOut(uint index)
{
    real[index] = origin[index];
    uint2 size;
    origin.GetDimensions(size.x, size.y);

    //適応時間
    float duration = 0.5f;
    if (index < size.x * duration)
    {
        real[size.x - index - 1] *= index / (size.x * duration);
    }
}

// フィルタ用入力バッファ
groupshared float input[2];
// フィルタ用出力バッファ
groupshared float output[2];

// IIR ローパス
void LowPass(uint index)
{
    //フィルタ係数計算で使用する中間値
    float omega = 2.0f * PI * cutoff / sample;
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

// IIR ハイパス
void HightPass(uint index)
{
    //フィルタ係数計算で使用する中間値
    float omega = 2.0f * PI * cutoff / sample;
    float alpha = sin(omega) / (2.0f * q);

	//フィルタ係数
    float a0 =   1.0f + alpha;
    float a1 =  -2.0f * cos(omega);
    float a2 =   1.0f - alpha;
    float b0 =  (1.0f + cos(omega)) / 2.0f;
    float b1 = -(1.0f + cos(omega));
    float b2 =  (1.0f + cos(omega)) / 2.0f;

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

// IIR バンドパス
void BundPass(uint index)
{
    //フィルタ係数計算で使用する中間値
    float omega = 2.0f * PI * cutoff / sample;
    float alpha = sin(omega) * sinh(log(2.0f)) / 2.0f * octave * omega / sin(omega);

	//フィルタ係数
    float a0 =  1.0f + alpha;
    float a1 = -2.0f * cos(omega);
    float a2 =  1.0f - alpha;
    float b0 =  alpha;
    float b1 =  0.0f;
    float b2 = -alpha;

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

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void CS(uint3 gID : SV_GroupID, uint3 gtID : SV_GroupThreadID, uint3 disID : SV_DispatchThreadID)
{
    if(low)
    {
        LowPass(gID.x);
    }
    else if(hight)
    {
        HightPass(gID.x);
    }
    else if(bund)
    {
        BundPass(gID.x);
    }
    else
    {
        //Delay(gID.x);
        real[gID.x] = origin[gID.x];
    }

    AllMemoryBarrierWithGroupSync();
}