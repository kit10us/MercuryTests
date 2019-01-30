struct VS_IN
{
	float3 position		: POSITION;
	float4 color		: COLOR;
	matrix <float, 4, 4> world	: Matrix;
};

struct VS_OUT
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
};

cbuffer Constants : register(b0)
{
	float4x4 view;
	float4x4 projection;
};

VS_OUT vs_main( in VS_IN vs_in )
{
	VS_OUT vs_out;
	vs_out.position = float4( vs_in.position, 1.0f );
	vs_out.position = mul( vs_out.position, transpose( vs_in.world ) );
	vs_out.position = mul( vs_out.position, transpose( view ) );
	vs_out.position = mul( vs_out.position, transpose( projection ) );	
	vs_out.color = vs_in.color;
	return vs_out;
}

float4 ps_main( in VS_OUT ps_in ) : SV_TARGET
{
	return saturate( ps_in.color );
}
