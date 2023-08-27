struct VSOut
{
	float4 color : Color;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float4 color : Color)
{
	VSOut output;
	output.pos = float4(pos, 1.0f);
	output.color = color;
	return output;
}
