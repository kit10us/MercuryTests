struct VS_IN
{
	float3 position		: POSITION0;
	float2 texCoord 	: TEXCOORD0;
};

struct VS_OUT
{
	float4 position		: POSITION0;
	float2 texCoord 	: TEXCOORD0;
};

struct PS_OUT
{
	float4 color		: COLOR0;
};

sampler2D Tex0;

float4x4 worldMatrix;
float4x4 viewMatrix;
float4x4 projectionMatrix;

VS_OUT vs_main( in VS_IN vs_in )
{
	VS_OUT vs_out;
	vs_out.position = float4( vs_in.position, 1.0f );
	vs_out.position = mul( vs_out.position, worldMatrix );
	vs_out.position = mul( vs_out.position, viewMatrix );
	vs_out.position = mul( vs_out.position, projectionMatrix );	
	
	vs_out.texCoord = vs_in.texCoord;
	return vs_out;
}

PS_OUT ps_main( in VS_OUT ps_in )
{
	PS_OUT ps_out;
	ps_out.color = saturate( tex2D( Tex0, ps_in.texCoord ) );
	return ps_out;
}