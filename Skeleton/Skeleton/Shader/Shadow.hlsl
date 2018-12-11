// ���[�g�V�O�l�`���̒�`
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(SRV(t1, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(SRV(t2, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(SRV(t3, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(CBV(b1, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(CBV(b2, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
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
Texture2D<float4> spa : register(t1);
Texture2D<float4> sph : register(t2);
Texture2D<float4> toon : register(t3);
SamplerState smp : register(s0);

cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
    float4x4 lightView;
    float4x4 lightProjection;
    float3 eyePos;
    float3 lightPos;
}

cbuffer Mat : register(b1)
{
    float3 diffuse;
    float alpha;
    float specularity;
    float3 specula;
    float3 mirror;
    int texFlag;
    int spaFlag;
    int sphFlag;
}

cbuffer Born : register(b2)
{
    matrix mtx[256];
}

// ����
struct Input
{
    float4 pos       : POSITION;
    float4 normal    : NORMAL;
    float2 uv        : TEXCOORD;
    min16uint2 born  : BORN;
    min16uint weight : WEIGHT;
};

// �o��
struct Out
{
    float4 svpos     : SV_POSITION;
    float4 pos       : POSITION;
    float4 normal    : NORMAL;
    float2 uv        : TEXCOORD;
    min16uint2 born  : BORN;
    min16uint weight : WEIGHT;
};

// ���_�V�F�[�_
[RootSignature(RS)]
Out VS(Input input)
{
    Out o;
    o.svpos  = input.pos;
    o.pos    = input.pos;
    o.normal = input.normal;
    o.uv     = input.uv;
    o.born   = input.born;
    o.weight = input.weight;

    return o;
}

#define MAX_VERTEX_COUNT 9

// �W�I���g���[�V�F�[�_�[
[maxvertexcount(MAX_VERTEX_COUNT)]
void GS(triangle Out vertex[3], inout TriangleStream<Out> stream)
{
    for (int i = -1; i <= MAX_VERTEX_COUNT / 3 / 2; ++i)
    {
        stream.RestartStrip();
        for (int n = 0; n < 3; ++n)
        {
            float4 pos = vertex[n].pos;
            pos.x += 10.0f * i;

            Out o;
            o.pos    = pos;
            o.normal = mul(world, vertex[n].normal);
            o.uv     = vertex[n].uv;
            o.born   = vertex[n].born;
            o.weight = vertex[n].weight;

            float w = o.weight / 100.0f;
            matrix m = mtx[o.born.x] * w + mtx[o.born.y] * (1.0f - w);
            //���_�ɕ��s�ړ�
            matrix vec1 = float4x4(1.0f, 0.0f, 0.0f, vertex[n].pos.x - pos.x,
                                   0.0f, 1.0f, 0.0f, vertex[n].pos.y - pos.y,
                                   0.0f, 0.0f, 1.0f, vertex[n].pos.z - pos.z,
                                   0.0f, 0.0f, 0.0f, 1.0f);
            //�w��ʒu�ɕ��s�ړ�
            matrix vec2 = float4x4(1.0f, 0.0f, 0.0f, pos.x - vertex[n].pos.x,
                                   0.0f, 1.0f, 0.0f, pos.y - vertex[n].pos.y,
                                   0.0f, 0.0f, 1.0f, pos.z - vertex[n].pos.z,
                                   0.0f, 0.0f, 0.0f, 1.0f);

            pos = mul(vec1, pos);
            pos = mul(m, pos);
            pos = mul(vec2, pos);
            pos = mul(world, pos);
            pos = mul(lightView, pos);
            pos = mul(lightProjection, pos);
            o.svpos = pos;

            stream.Append(o);
        }
    }
    
    stream.RestartStrip();
}

// �~����
#define PI 3.14159265f

// �s�N�Z���V�F�[�_
float4 PS(Out o) : SV_TARGET
{
    //���C�g�̐F
    float3 lightColor = float3(1.0f, 1.0f, 1.0f);
    //�����x�N�g��
    float3 eyeVec = normalize(o.pos.xyz - eyePos);
    //�����x�N�g��
    float3 light = normalize(lightPos);
    //���˃x�N�g��
    float3 ref = reflect(-light, o.normal.xyz);
    //�X�؃L����
    float3 spec = pow(saturate(dot(ref, eyeVec)), specularity);
    //�����x�N�g���Ɩ@���Ƃ̓���
    float bright = saturate(dot(light, o.normal.xyz));
    //�����o�[�g
    bright = saturate(acos(bright) / PI);
    float3 toonColor = toon.Sample(smp, float2(0.0f, 1.0f - bright)).rgb;

    float3 color = (texFlag == false) ? saturate((toonColor * diffuse) * bright + specula * spec + mirror * lightColor) : tex.Sample(smp, o.uv).rgb;
    color *= (sphFlag == false) ? 1.0f : sph.Sample(smp, eyeVec.xy / 2.0f * float2(1.0f, -1.0f) + float2(0.5f, 0.5f)).rgb;
    color += (spaFlag == false) ? 0.0f : spa.Sample(smp, eyeVec.xy / 2.0f * float2(1.0f, -1.0f) + float2(0.5f, 0.5f)).rgb;
    

    return float4(color, alpha);
}