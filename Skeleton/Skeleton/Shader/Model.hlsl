// ルートシグネチャの定義
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(SRV(t1, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(CBV(b1, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
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
Texture2D<float4> toon: register(t1);
SamplerState smp      : register(s0);

cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
}

cbuffer Mat : register(b1)
{
    float3 diffuse;
    float alpha;
    float specularity;
    float3 specula;
    float3 mirror;
    int texFlag;
}

// 入力
struct Input
{
    float4 pos    : POSITION;
    float4 normal : NORMAL;
    float2 uv     : TEXCOORD;
};

// 出力
struct Out
{
    float4 svpos  : SV_POSITION;
    float4 pos    : POSITION;
    float4 normal : NORMAL;
    float2 uv     : TEXCOORD;
};

// 頂点シェーダ
[RootSignature(RS)]
Out VS(Input input)
{
    Out o;
    o.svpos = input.pos;
    o.pos = input.pos;
    o.normal = input.normal;
    o.uv = input.uv;

    return o;
}

#define MAX_VERTEX_COUNT 9

// ジオメトリーシェーダー
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
            o.pos = pos;
            o.normal = mul(world, vertex[n].normal);
            o.uv = vertex[n].uv;

            pos = mul(world, pos);
            pos = mul(view, pos);
            pos = mul(projection, pos);
            o.svpos = pos;

            stream.Append(o);
        }
    }
    
    stream.RestartStrip();
}

// ピクセルシェーダ
float4 PS(Out o) : SV_TARGET
{
    //ライトの色
    float3 lightColor = float3(1.0f, 1.0f, 1.0f);
    //視線ベクトル
    float3 eye = float3(0.0f, 10.0f, -15.0f);
    float3 eyeVec = normalize(o.pos.xyz - eye);
    //光源ベクトル
    float3 light = normalize(float3(-1.0f, 1.0f, -1.0f));
    //反射ベクトル
    float3 ref = reflect(-light, o.normal.xyz);
    //スぺキュラ
    float3 spec = pow(saturate(dot(ref, eyeVec)), specularity);
    //光源ベクトルと法線との内積
    float bright = saturate(dot(light, o.normal.xyz));
    //ランバート
    bright = saturate(acos(bright) / 3.14159265f);
    float3 toonColor = toon.Sample(smp, float2(0.0f, 1.0f - bright)).rgb;

    //return float4(toonColor, 1.0f);

    float3 color = (texFlag == false) ? saturate((toonColor * diffuse) * bright + specula * spec + mirror * lightColor) : tex.Sample(smp, o.uv).rgb;

    return float4(color, alpha);
}