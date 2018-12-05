// ルートシグネチャの定義
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
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

SamplerState smp : register(s0);

cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
}

// 入力
struct Input
{
    float4 pos : POSITION;
};

// 出力
struct Out
{
    float4 svpos : SV_POSITION;
    float4 pos   : POSITION;
};

// 頂点シェーダ
[RootSignature(RS)]
Out VS(Input input)
{
    input.pos = mul(world, input.pos);
    input.pos = mul(view, input.pos);
    input.pos = mul(projection, input.pos);

    Out o;
    o.svpos = input.pos;
    o.pos   = input.pos;

    return o;
}

// ピクセルシェーダ
float4 PS(Out o) : SV_TARGET
{
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
}