// ルートシグネチャの定義
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(SRV(t1, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
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

Texture2D<float4> tex  : register(t0);
Texture2D<float> depth : register(t1);
SamplerState smp       : register(s0);

// 入力
struct Input
{
    float4 pos : POSITION;
    float2 uv  : TEXCOORD;
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
    Out o;
    o.svpos = input.pos;
    o.pos   = input.pos;
    o.uv    = input.uv;

    return o;
}

// ピクセルシェーダ
float4 PS(Out o) : SV_TARGET
{
    float2 size;
    tex.GetDimensions(size.x, size.y);
    //隣接するピクセル
    float2 adjacent = float2(1.0f / size.x, 1.0f / size.y);
    float4 ret = 0.0f;

    //上
    ret += tex.Sample(smp, o.uv + float2(-2.0f * adjacent.x, 2.0f * adjacent.y)) / 9.0f;
    ret += tex.Sample(smp, o.uv + float2( 2.0f * adjacent.x, 2.0f * adjacent.y)) / 9.0f;
    ret += tex.Sample(smp, o.uv + float2( 0.0f * adjacent.x, 2.0f * adjacent.y)) / 9.0f;
    //真ん中
    ret += tex.Sample(smp, o.uv + float2(-2.0f * adjacent.x, 0.0f * adjacent.y)) / 9.0f;
    ret += tex.Sample(smp, o.uv + float2( 0.0f * adjacent.x, 0.0f * adjacent.y)) / 9.0f;
    ret += tex.Sample(smp, o.uv + float2( 2.0f * adjacent.x, 0.0f * adjacent.y)) / 9.0f;
    //下
    ret += tex.Sample(smp, o.uv + float2(-2.0f * adjacent.x, -2.0f * adjacent.y)) / 9.0f;
    ret += tex.Sample(smp, o.uv + float2( 2.0f * adjacent.x, -2.0f * adjacent.y)) / 9.0f;
    ret += tex.Sample(smp, o.uv + float2( 0.0f * adjacent.x, -2.0f * adjacent.y)) / 9.0f;

    float4 test = tex.Sample(smp, o.uv) + ret;

    return tex.Sample(smp, o.uv) * test;
}