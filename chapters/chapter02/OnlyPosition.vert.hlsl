struct Input
{
	float x : TEXCOORD0;
	float y : TEXCOORD1;
	float z : TEXCOORD2;
};

struct Output
{
	float4 Position : SV_Position;
};

Output main(Input input)
{
	Output output;
	output.Position = float4(input.x, input.y, input.z, 1.0f);

	return output;
}
