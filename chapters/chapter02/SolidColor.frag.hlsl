struct Input
{
};

struct Output
{
	float4 Color : SV_Target0;
};

Output main(Input input)
{
	Output output;
	output.Color = float4(1.0f, 0.0f, 0.0f, 1.0f);

	return output;
}
