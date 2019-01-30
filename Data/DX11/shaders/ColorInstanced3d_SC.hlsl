struct VS_IN
{
	float3 position		: POSITION;
	float4 color		: COLOR;
	uint instanceID : SV_InstanceID;
};

struct VS_OUT
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
};

cbuffer Common
{
	float4x4 view;
	float4x4 projection;
};

cbuffer Instances
{
	float4x4 world[1000];
};


VS_OUT vs_main( in VS_IN vs_in )
{
	VS_OUT vs_out;

	vs_out.position = float4( vs_in.position, 1.0f );
	vs_out.position = mul( vs_out.position, transpose( world[vs_in.instanceID] ) );
	vs_out.position = mul( vs_out.position, transpose( view ) );
	vs_out.position = mul( vs_out.position, transpose( projection ) );	
	vs_out.color = vs_in.color;
	return vs_out;
}

float4 ps_main( in VS_OUT ps_in ) : SV_TARGET
{
	return saturate( ps_in.color );
}
