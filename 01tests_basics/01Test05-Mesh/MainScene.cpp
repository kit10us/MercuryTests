// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <Windows.h>
#include <MainScene.h>

using namespace me;
using namespace render;
using namespace scene;

MainScene::MainScene( me::game::Game * gameInstance )
	:Scene( gameInstance, "main" )
{	
}

void MainScene::OnStart()
{
	AddResources( unify::Path( "resources/Standard.me_res" ) );
	auto colorEffect = GetAsset< Effect >( "ColorSimple" );
	auto textureEffect = GetAsset< Effect>( "TextureSimple" );
	auto textureAndColorEffect = GetAsset< Effect>( "TextureAndColor" );
	auto fourColorsEffect = GetAsset< Effect >( "FourColors" );

	// Create mesh...
	mesh.reset( new Mesh( GetOS()->GetRenderer( 0 ) ) );

	float xscalar = 10.0f;
	float yscalar = 10.0f;
	float zscalar = 10.0f;

	struct Vertex
	{
		float x, y, z;
		float u, v;
	};

	Vertex vbRaw[] =
	{
		// Z-Near...
		{ -xscalar, yscalar, zscalar, 0.0f, 0.0f },
		{ xscalar, yscalar, zscalar, 1.0f, 0.0f },
		{ -xscalar, -yscalar, zscalar, 0.0f, 1.0f },
		{ xscalar, yscalar, zscalar, 1.0f, 0.0f },
		{ xscalar, -yscalar, zscalar, 1.0f, 1.0f },
		{ -xscalar, -yscalar, zscalar, 0.0f, 1.0f },

		// Z-Near...
		{ xscalar, yscalar, -zscalar, 0.0f, 0.0f },
		{ -xscalar, yscalar, -zscalar, 1.0f, 0.0f },
		{ xscalar, -yscalar, -zscalar, 0.0f, 1.0f },
		{ -xscalar, yscalar, -zscalar, 1.0f, 0.0f },
		{ -xscalar, -yscalar, -zscalar, 1.0f, 1.0f },
		{ xscalar, -yscalar, -zscalar, 0.0f, 1.0f },

		// X-Min...
		{ -xscalar, yscalar, zscalar, 0.0f, 0.0f },
		{ -xscalar, yscalar, -zscalar, 1.0f, 0.0f },
		{ -xscalar, -yscalar, zscalar, 0.0f, 1.0f },
		{ -xscalar, yscalar, -zscalar, 1.0f, 0.0f },
		{ -xscalar, -yscalar, -zscalar, 1.0f, 1.0f },
		{ -xscalar, -yscalar, zscalar, 0.0f, 1.0f },

		// X-Max...
		{ xscalar, yscalar, -zscalar, 0.0f, 0.0f },
		{ xscalar, yscalar, zscalar, 1.0f, 0.0f },
		{ xscalar, -yscalar, -zscalar, 0.0f, 1.0f },
		{ xscalar, yscalar, zscalar, 1.0f, 0.0f },
		{ xscalar, -yscalar, zscalar, 1.0f, 1.0f },
		{ xscalar, -yscalar, -zscalar, 0.0f, 1.0f },

		// Y-Min...
		{ -xscalar, yscalar, zscalar, 0.0f, 0.0f },
		{ xscalar, yscalar, zscalar, 1.0f, 0.0f },
		{ -xscalar, yscalar, -zscalar, 0.0f, 1.0f },
		{ xscalar, yscalar, zscalar, 1.0f, 0.0f },
		{ xscalar, yscalar, -zscalar, 1.0f, 1.0f },
		{ -xscalar, yscalar, -zscalar, 0.0f, 1.0f },

		// Y-Max...
		{ -xscalar, -yscalar, -zscalar, 0.0f, 0.0f },
		{ xscalar, -yscalar, -zscalar, 1.0f, 0.0f },
		{ -xscalar, -yscalar, zscalar, 0.0f, 1.0f },
		{ xscalar, -yscalar, -zscalar, 1.0f, 0.0f },
		{ xscalar, -yscalar, zscalar, 1.0f, 1.0f },
		{ -xscalar, -yscalar, zscalar, 0.0f, 1.0f }
	};
	unsigned int numberOfVertices = sizeof( vbRaw ) / sizeof( Vertex );

	auto & bs = mesh->GetPrimitiveList().AddBufferSet();
	bs.SetEffect( textureEffect );
	bs.AddVertexBuffer( { textureEffect->GetVertexShader()->GetVertexDeclaration(), { { numberOfVertices, vbRaw } }, BufferUsage::Default } );
	bs.AddMethod( RenderMethod::CreateTriangleList( 0, 12 ) );
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	static unify::Angle rotation( unify::AngleInRadians( 0.0f ) );
	static int axisIndex = 0;

	HRESULT result = S_OK;

	const float width = (float)params.renderer->GetViewport().GetSize().width;
	const float height = (float)params.renderer->GetViewport().GetSize().height;

	rotation += unify::AngleInDegrees( params.GetDelta().GetSeconds() * 360.0f );
	if( rotation.Normalize() != 0 )
	{
		++axisIndex;
		if( axisIndex >= 3 )
		{
			axisIndex = 0;
		}
	}

	unify::V3< float > eye( 0.0f, 1.0f, -50.0f );
	unify::V3< float > at( 0.0f, 0.0f, 0.0f );
	unify::V3< float > up( 0.0f, 1.0f, 0.0f );

	unify::V3< float > axis( (axisIndex == 0) ? 1.0f : 0.0f, (axisIndex == 1) ? 1.0f : 0.0f, (axisIndex == 2) ? 1.0f : 0.0f );
	
	q = unify::Quaternion( axis, rotation );
	
	params.renderInfo.SetViewMatrix( unify::MatrixLookAtLH( eye, at, up ) );
	params.renderInfo.SetProjectionMatrix( unify::MatrixPerspectiveFovLH( 3.1415926535f / 4.0f, width / height, 0.01f, 100.0f ) );
}

void MainScene::OnRender( RenderGirl renderGirl )
{
	render::Params params = *renderGirl.GetParams();
	std::vector< const unify::FrameLite * > frames;
	unify::FrameLite frame( q, unify::V3< float >( 0, 0, 0 ) );
	frames.push_back( &frame );
	mesh->Render( params, nullptr, render::MatrixFeed( render::MatrixFood_Frames{ &frames[0], frames.size() }, 1 ) );
}
