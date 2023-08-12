cbuffer ConstBuff
{
	float4 tint;
};

float4 main(float4 color : Color) : SV_Target
{
	return color * tint;
}