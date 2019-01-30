// Input vertex format (into vertex shader
struct VS_IN
{
	// Per-vertex data.
	float3 					position			: POSITION;
	float4 					color				: COLOR;
	float4 					matrix_influence	: MatrixInfluence;
	
	// Per-instance data.
	matrix <float, 4, 4> 	world[2]	: Matrix;
	
	// Per-instance ID.
	uint 					instanceID    : SV_InstanceID;
};

// Output vertex format (from vertex shader, into pixel shader).
struct VS_OUT
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
};

// Per render constants.
cbuffer Constants : register(b0)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

// Per render constants.
cbuffer Constants : register(b1)
{
	float4x4 bones;
};


// Vertex shader main entrance point.
VS_OUT vs_main( in VS_IN vs_in )
{
	VS_OUT vs_out;

	// Initial position from input.
	vs_out.position = float4( vs_in.position, 1.0f );

	// Affect position as necessary by skinning...
	
	for( uint i = 1; i <= 1; i++ )
	{
		int matrix_index = floor( vs_in.matrix_influence[i] );
		if( matrix_index > -1 )
		{
			float1 matrix_influence = (vs_in.matrix_influence[i] - matrix_index ) * 10.0f;
			//matrix_index += vs_in.instanceID * 2;
			vs_out.position += mul( vs_out.position, transpose( vs_in.world[ matrix_index ] ) * matrix_influence );
		}	
	}

	// Affect position by 0, as is world transform.
	vs_out.position = mul( vs_out.position, transpose( vs_in.world[0] ) );

	vs_out.position = mul( vs_out.position, transpose( viewMatrix ) );
	vs_out.position = mul( vs_out.position, transpose( projectionMatrix ) );	
	vs_out.color = vs_in.color;
	return vs_out;
}

// Pixel shader main entrance point.
float4 ps_main( in VS_OUT ps_in ) : SV_TARGET
{
	return saturate( ps_in.color );
}
