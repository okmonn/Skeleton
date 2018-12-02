// ルートシグネチャの定義
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "StaticSampler(s0, "\
                                  "filter         = FILTER_MIN_MAG_MIP_LINEAR, "\
                                  "addressU       = TEXTURE_ADDRESS_WRAP, "\
                                  "addressV       = TEXTURE_ADDRESS_WRAP, "\
                                  "addressW       = TEXTURE_ADDRESS_WRAP, "\
                                  "mipLodBias     = 0.0f, "\
                                  "maxAnisotropy  = 0, "\
                                  "comparisonFunc = COMPARISON_NEVER, "\
                                  "borderColor    = STATIC_BORDER_COLOR_TRANSPARENT_BLACK, "\
                                  "minLOD         = 0.0f, "\
                                  "maxLOD         = 3.402823466e+38f, "\
                                  "space          = 0, "\
                                  "visibility     = SHADER_VISIBILITY_ALL)"

Texture2D<float4> tex : register(t0);
SamplerState smp      : register(s0);

// 情報
cbuffer Info : register(b0)
{
    float4x4 mtx;
    float2 window;
    float2 uvPos;
    float2 uvSize;
    float2 reverse;
    float alpha;
}

// 入力
struct Input
{
    float4 pos  : POSITION;
    float2 uv   : TEXCOORD;
};

// 出力
struct Out
{
    float4 svpos : SV_POSITION;
    float4 pos   : POSITION;
    float2 uv    : TEXCOORD;
};

// 頂点シェーダ
[RootSignature(RS)]
Out VS(Input input)
{
    //テクスチャサイズの取得
    float2 size = float2(0.0f, 0.0f);
    tex.GetDimensions(size.x, size.y);

    input.pos    = mul(mtx, input.pos);
    input.pos.xy = float2(-1.0f, 1.0f) + (input.pos.xy / float2((window.x / 2.0f), -(window.y / 2.0f)));
    input.uv = (input.uv * reverse * uvSize + uvPos) / size;

    Out o;
    o.svpos = input.pos;
    o.pos   = input.pos;
    o.uv    = input.uv;

    return o;
}

// ピクセルシェーダ
float4 PS(Out o) : SV_TARGET
{
    float a = tex.Sample(smp, o.uv).a * alpha;
    if (a <= 0.0f)
    {
        discard;
    }

    return float4(tex.Sample(smp, o.uv).rgb, a);
}