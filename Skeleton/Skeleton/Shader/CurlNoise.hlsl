// ルートシグネチャの宣言
#define CRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
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

// 適応データ
RWStructuredBuffer<float3> real : register(u0);

// ランダム
float Rand(float3 co)
{
    return frac(sin(dot(co.xyz, float3(12.9898, 78.233, 56.787))) * 43758.5453);
}

// ノイズ
float Noise(float3 pos)
{
    float3 ip = floor(pos);
    float3 fp = smoothstep(0, 1, frac(pos));
    float4 a = float4(
		Rand(ip + float3(0, 0, 0)),
		Rand(ip + float3(1, 0, 0)),
		Rand(ip + float3(0, 1, 0)),
		Rand(ip + float3(1, 1, 0)));
    float4 b = float4(
		Rand(ip + float3(0, 0, 1)),
		Rand(ip + float3(1, 0, 1)),
		Rand(ip + float3(0, 1, 1)),
		Rand(ip + float3(1, 1, 1)));
 
    a = lerp(a, b, fp.z);
    a.xy = lerp(a.xy, a.zw, fp.y);
    return lerp(a.x, a.y, fp.x);
}

// パーリンノイズ
float Perlin(float3 pos)
{
    return (Noise(pos) * 32 + Noise(pos * 2) * 16 + Noise(pos * 4) * 8 +
		Noise(pos * 8) * 4 + Noise(pos * 16) * 2 + Noise(pos * 32)) / 63;
}

[RootSignature(CRS)]
[numthreads(1, 1, 1)]
void CS(uint3 gID : SV_GroupID, uint3 gtID : SV_GroupThreadID, uint3 disID : SV_DispatchThreadID)
{
    real[gID.x].x = Perlin(real[gID.x]);

    AllMemoryBarrierWithGroupSync();
}