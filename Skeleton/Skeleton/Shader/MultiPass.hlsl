// ���[�g�V�O�l�`���̒�`
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

Texture2D<float4> tex : register(t0);
Texture2D<float> depth : register(t1);
SamplerState smp      : register(s0);

// ����
struct Input
{
    float4 pos : POSITION;
    float2 uv  : TEXCOORD;
};

// �o��
struct Out
{
    float4 svpos : SV_POSITION;
    float4 pos   : POSITION;
    float2 uv    : TEXCOORD;
};

// ���_�V�F�[�_
[RootSignature(RS)]
Out VS(Input input)
{
    Out o;
    o.svpos = input.pos;
    o.pos   = input.pos;
    o.uv    = input.uv;

    return o;
}

// �s�N�Z���V�F�[�_
float4 PS(Out o) : SV_TARGET
{
    return depth.Sample(smp, o.uv);
    return tex.Sample(smp, o.uv);
}