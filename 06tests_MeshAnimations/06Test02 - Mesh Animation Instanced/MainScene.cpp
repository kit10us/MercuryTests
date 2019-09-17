// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>

using namespace me;

MainScene::MainScene( me::game::Game * gameInstance )
	:Scene( gameInstance, "main" )
{	
}

void MainScene::OnStart()
{
	using namespace render;
	using namespace unify;

	AddResources( unify::Path( "resources/Standard.me_res" ) );
	auto colorEffect = GetAsset< Effect >( "ColorSimple" );
	auto textureEffect = GetAsset< Effect>( "TextureSimple" );
	auto textureAndColorEffect = GetAsset< Effect>( "TextureAndColor" );
	auto fourColorsEffect = GetAsset< Effect >( "FourColors" );
	auto colorAmbientEffect = GetAsset< Effect >( "ColorAmbient" );
	auto textureAmbientEffect = GetAsset< Effect >( "TextureAmbient" );
	auto colorInstancedAmbientEffect = GetAsset< Effect >( "ColorAmbientInstanced" );

	// Load effect...
	auto colorAnimInstancedEfect = GetManager< Effect>()->Add( "ColorAnimInstanced", unify::Path( "ColorAnimInstanced.me_effect" ) );

	auto effect = Effect::ptr{ new me::render::Effect( 
		GetAsset< IVertexShader >( "ColorAnimInstanced" ), 
		GetAsset< IPixelShader >( "ColorAnimInstanced" )
	) };

	// Create mesh...
	m_mesh.reset( new Mesh( GetOS()->GetRenderer( 0 ) ) );

	auto & bs = m_mesh->GetPrimitiveList().AddBufferSet();
	bs.SetEffect( effect );

	struct Vertex
	{
		float x, y, z;
		unify::Color diffuse;
		float matrix_inlfuence[4];
	};

	size_t segments = 30;
	size_t vertexPerTriangle = 3;
	size_t trianglesPerSegment = 8;
	size_t indicesPerTriangle = 3;
	unsigned int numberOfVertices = (segments + 1) * 4;
	unsigned int numberOfIndices = segments * trianglesPerSegment * indicesPerTriangle;
	Vertex * vbRaw = new Vertex[ numberOfVertices ];
	std::vector< Index32 > ibRaw( numberOfIndices );

	class MatrixInfluence
	{
		int m_index;
		float m_influence; 
	public:
		MatrixInfluence(int index = -1, float influence = 1.0f)
			: m_index( index )
			, m_influence( influence )
		{														
		}

		/// <summary>
		/// Matrix index, if negative, then ignored.
		/// </summary>
		void SetIndex( int index = -1 )
		{
			m_index = index;
		}

		void SetInfluence(float influence = 1.0f)
		{
			m_influence = std::min( influence, 1.0f );
			m_influence = std::max( m_influence, 0.0f );
		}

		float Result() const
		{	
			if (m_index < 0.0f || m_influence < 0.0001f) return -1.0f;
			float influence = m_influence * 0.1f;
			return m_index + influence;
		}
	};
	
	{
		for( size_t i = 0; i < segments + 1; ++i )
		{
			float x = 1.5f * sin(((float)(segments - i) / (segments) ) * 3.14159f * 0.5f);
			float z = 1.5f * sin(((float)(segments - i) / (segments) ) * 3.14159f * 0.5f);
			float y = -10 + 20.0f * ((float)i / (segments) );
			unsigned char color = (unsigned char )(255.0f * ((float)i / (segments) ));
		
			MatrixInfluence matrix_influence [] {
				{ 0, 1.0f },
				{ 1, (float)i / (segments) },
				{ 2, 0.0f }
			};

			float result_influence [] =
			{
				matrix_influence[0].Result(),
				matrix_influence[1].Result(),
				matrix_influence[2].Result()
			};

  			vbRaw[ i * 4 + 0 ] = { 
				-x,  y, -z, unify::ColorRed( color ),
				result_influence[0], result_influence[1], result_influence[2], 0 };
			vbRaw[ i * 4 + 1 ] = {  
				x,  y, -z, unify::ColorGreen( color ),
				result_influence[0], result_influence[1], result_influence[2], 0 };
			vbRaw[ i * 4 + 2 ] = {  
				x,  y, z, unify::ColorBlue( color ),
				result_influence[0], result_influence[1], result_influence[2], 0 };
			vbRaw[ i * 4 + 3 ] = { 
				-x,  y, z, unify::ColorWhite( color ),
				result_influence[0], result_influence[1], result_influence[2], 0 };
		};

		size_t index = 0;
		for(  size_t i = 0; i < segments; ++i )
		{
			unsigned int base = i * 4;
			auto AddTriangle = [&] ( auto a, auto b, auto c ) { 
				ibRaw[ index++ ] = base + a;
				ibRaw[ index++ ] = base + b;
				ibRaw[ index++ ] = base + c;
 			};

			AddTriangle( 0, 4, 1 );
			AddTriangle( 4, 5, 1 );

			AddTriangle( 1, 5, 2 );
			AddTriangle( 5, 6, 2 );

			AddTriangle( 2, 6, 3 );
			AddTriangle( 6, 7, 3 );

			AddTriangle( 3, 7, 0 );
			AddTriangle( 7, 4, 0 );
		}

		bs.AddVertexBuffer( { effect->GetVertexShader()->GetVertexDeclaration(), { { numberOfVertices, vbRaw } }, BufferUsage::Default } );
	}

	bs.AddIndexBuffer( { { { numberOfIndices, &ibRaw[0] } } } );
	bs.AddMethod( RenderMethod::CreateTriangleListIndexed( numberOfVertices, numberOfIndices, 0, 0 ) );

	// Create animation...
	using namespace frameanimation;		
	m_animation.reset( new Animation( "test", 1.0f ) );
	m_animation->AddRotationKey( 0, RotationKey( 0.25f, unify::Quaternion( V3< float >( 0, 0, 1 ), AngleInDegrees( 180 ) ) ) );
	m_animation->AddRotationKey( 0, RotationKey( 0.5f, unify::Quaternion( V3< float >( 0, 0, 1 ), AngleInDegrees( 0 ) ) ) );
	m_animation->AddRotationKey( 0, RotationKey( 0.75f, unify::Quaternion( V3< float >( 0, 0, 1 ), AngleInDegrees( -180 ) ) ) );
	m_animation->AddRotationKey( 0, RotationKey( 1.0f, unify::Quaternion( V3< float >( 0, 0, 1 ), AngleInDegrees( 0 ) ) ) );
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	using namespace unify;

	static unify::Angle rotation( unify::AngleInRadians( 0.0f ) );
	static int axisIndex = 1;

	const float width = (float)params.renderer->GetViewport().GetSize().width;
	const float height = (float)params.renderer->GetViewport().GetSize().height;

	rotation += unify::AngleInDegrees( params.GetDelta().GetSeconds() * 90.0f );

	V3< float > eye( 0.0f, 1.0f, -50.0f );
	V3< float > at( 0.0f, 0.0f, 0.0f );
	V3< float > up( 0.0f, 1.0f, 0.0f );

	V3< float > axis( (axisIndex == 0) ? 1.0f : 0.0f, (axisIndex == 1) ? 1.0f : 0.0f, (axisIndex == 2) ? 1.0f : 0.0f );
	
	m_q = Quaternion( axis, rotation );
	m_q = QuaternionIdentity();
	
	params.renderInfo.SetViewMatrix( MatrixLookAtLH( eye, at, up ) );
	params.renderInfo.SetProjectionMatrix( MatrixPerspectiveFovLH( 3.1415926535f / 4.0f, width / height, 0.01f, 100.0f ) );
}

void MainScene::OnRender( scene::RenderGirl renderGirl )
{
	using namespace unify;

	// Apply animation...
	static float progress = 0.0f;
	FrameSet frameSet;
	frameSet.Add( unify::MatrixIdentity(), 0 );
	
	FrameSetInstance frameSetInstance1( &frameSet );
	FrameSetInstance frameSetInstance2( &frameSet );
	m_animation->ApplyToFrames( progress, frameSetInstance1 );
	m_animation->ApplyToFrames( progress, frameSetInstance2 );
	frameSetInstance1.UpdateLocals();
	frameSetInstance2.UpdateLocals();
	
	render::Params params = *renderGirl.GetParams();
	progress += params.GetDelta().GetSeconds();

	Matrix matrices[] =
	{
		// First object.
		MatrixTranslate( { -10, 0, 0 } ), frameSetInstance1.Local( 0 ),
		
		// Second object.
		MatrixTranslate( { 10, 0, 0 } ), frameSetInstance2.Local( 0 )
	};

	m_mesh->Render( params, nullptr,
		render::MatrixFeed(
			render::MatrixFood_Matrices{ matrices, 4 },
			2
		)
	);

}
