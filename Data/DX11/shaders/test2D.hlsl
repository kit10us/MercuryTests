struct VS_IN
{
	float3 position		: POSITION;
	float4 color		: COLOR;
};

struct VS_OUT
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
};

cbuffer Constants
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

VS_OUT vs_main( in VS_IN vs_in )
{
	VS_OUT vs_out;
	float4x4 finalMatrix = worldMatrix * viewMatrix * projectionMatrix;
	vs_out.position = mul( float4( vs_in.position, 1.0f ), finalMatrix );
	vs_out.position += float4( -1.0f, 1.0f, 0.0f, 0.0f ); // Offset for screen position.
	vs_out.color = vs_in.color;
	return vs_out;
}

float4 ps_main( in VS_OUT ps_in ) : SV_TARGET
{
	return saturate( ps_in.color );
}
