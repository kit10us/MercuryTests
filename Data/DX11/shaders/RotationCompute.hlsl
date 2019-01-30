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
	vs_out.position = float4( vs_in.position, 1.0f );
	vs_out.position = mul( vs_out.position, transpose( worldMatrix ) );
	vs_out.position = mul( vs_out.position, transpose( viewMatrix ) );
	vs_out.position = mul( vs_out.position, transpose( projectionMatrix ) );	
	vs_out.color = vs_in.color;
	return vs_out;
}

float4 ps_main( in VS_OUT ps_in ) : SV_TARGET
{
	return saturate( ps_in.color );
}

struct RotationEntity
{
	float elapsed;
	int step;
	float4x4 rotationMatrix;
};

StructuredBuffer<RotationEntity> rotationBuffer;

[numthreads(1, 1, 1)]
void RotationMain( uint3 DTid : SV_DispatchThreadID )
{
/*
			using namespace DirectX;
			rotationInRadians += changeOverTime;
			if ( rotationInRadians >= XM_2PI )
			{
				rotationInRadians -= XM_2PI;
				++axisIndex;
				if ( axisIndex >= 3 )
				{
					axisIndex = 0;
				}
			}

			D3D11_MAPPED_SUBRESOURCE subResource = D3D11_MAPPED_SUBRESOURCE();
			result = dxContext->Map( vertexShaderConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &subResource );
			assert( ! FAILED( result  ) );

			vertexShaderConstants.worldMatrix = XMMatrixIdentity();
			XMVECTOR axis = XMVectorSet( axisIndex == 0, axisIndex == 1, axisIndex == 2, 1.0f );
			vertexShaderConstants.worldMatrix *= XMMatrixRotationAxis( axis, rotationInRadians );
		
			ConstantBuffer * target = (ConstantBuffer*)subResource.pData;
			*target = vertexShaderConstants;

			dxContext->Unmap( vertexShaderConstantBuffer, 0 );
*/
}

// Rotation shader

