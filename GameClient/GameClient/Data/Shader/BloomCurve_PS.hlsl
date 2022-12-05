
struct BloomCurveIn
{
	float4 pos		   : SV_POSITION;
	float3 posW		   : POSITION;
	float4 color	   : COLOR;
	float2 uv		   : UV;
	float3 normal	   : NORMAL;
	float3 tangent	   : TANGENT;
};

Texture2D TargetTexture : register(t0);
SamplerState samLinearClamp : register(s0);

float Bloom_Curve(float intensity, float threshold)
{
	float result = intensity;

	intensity *= 2.0f;

	result = intensity * 0.05 + max(0, intensity - threshold) * 0.5; // default threshold = 1.26

	return result * 0.5f;
}

float4 BloomCurve_PS(BloomCurveIn input) : SV_Target
{
	float4 color = TargetTexture.Sample(samLinearClamp, input.uv);

	float intensity = dot(color.rgb, float3(0.3f, 0.3f, 0.3f));

	float bloom_Intensity = Bloom_Curve(intensity, 1.26f);

	if (intensity == (float)0.f)
		return float4(0.f, 0.f, 0.f, 0.f);

	return color * (bloom_Intensity / intensity);
}