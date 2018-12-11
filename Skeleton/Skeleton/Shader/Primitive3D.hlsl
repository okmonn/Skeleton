// ルートシグネチャの定義
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "StaticSampler(s0, "\
                                  "filter         = FILTER_MIN_MAG_MIP_LINEAR, "\
                                  "addressU       = TEXTURE_ADDRESS_CLAMP, "\
                                  "addressV       = TEXTURE_ADDRESS_CLAMP, "\
                                  "addressW       = TEXTURE_ADDRESS_CLAMP, "\
                                  "mipLodBias     = 0.0f, "\
                                  "maxAnisotropy  = 0, "\
                                  "comparisonFunc = COMPARISON_NEVER, "\
                                  "borderColor    = STATIC_BORDER_COLOR_TRANSPARENT_BLACK, "\
                                  "minLOD         = 0.0f, "\
                                  "maxLOD         = 3.402823466e+38f, "\
                                  "space          = 0, "\
                                  "visibility     = SHADER_VISIBILITY_ALL)"

Texture2D<float> depth : register(t0);
SamplerState smp       : register(s0);

// 情報
cbuffer Info : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
    float4x4 lightView;
    float4x4 lightProjection;
}

// 入力
struct Input
{
    float4 pos   : POSITION;
    float2 uv    : TEXCOORD;
    float4 color : COLOR;
};

// 出力
struct Out
{
    float4 svpos    : SV_POSITION;
    float4 pos      : POSITION;
    float4 lightPos : LIGHTPOS;
    float2 uv       : TEXCOORD;
    float4 color    : COLOR;
};

// 頂点シェーダ
[RootSignature(RS)]
Out VS(Input input)
{
    Out o;
    o.pos      = mul(world, input.pos);
    o.lightPos = mul(lightProjection, mul(lightView, input.pos));
    o.uv       = input.uv;
    o.color    = input.color;

    input.pos = mul(world, input.pos);
    input.pos = mul(view, input.pos);
    input.pos = mul(projection, input.pos);
    o.svpos = input.pos;

    return o;
}

#define EPSILON 0.0005f

// ピクセルシェーダ
float4 PS(Out o) : SV_TARGET
{
    if (o.color.w <= 0.0f)
    {
        discard;
    }

    float2 uv = (float2(1.0f, -1.0f) + o.lightPos.xy) * float2(0.5f, -0.5f);

   
    float bright = 1.0f;
    if (o.lightPos.z > depth.Sample(smp, uv) + EPSILON)
    {
        bright *= 0.0f;
    }

    return float4(o.color.rgb * bright, o.color.a);
}