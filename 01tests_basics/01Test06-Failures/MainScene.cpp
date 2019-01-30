// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>

using namespace me;
using namespace scene;
using namespace render;

MainScene::MainScene( me::game::Game * gameInstance )
	:Scene( gameInstance, "main" )
{	
}

void MainScene::OnStart()
{
	// Get the display's width and height.
	const float width = (float)GetOS()->GetRenderer( 0 )->GetDisplay().GetSize().width;
	const float height = (float)GetOS()->GetRenderer( 0 )->GetDisplay().GetSize().height;

	// Add a color effect.
	m_effect = GetManager< Effect >()->Add( "color2d", unify::Path( "EffectColor2D.effect" ) );
	m_vertexCB = m_effect->GetVertexShader()->CreateConstantBuffer( BufferUsage::Dynamic );
	m_pixelCB = m_effect->GetPixelShader()->CreateConstantBuffer( BufferUsage::Dynamic );

	// Get a vertex declaration.
	auto vd = m_effect->GetVertexShader()->GetVertexDeclaration();

	// Calculate vertex count and buffer size.
	unsigned int vertexCount = 4;
	size_t sizeOfBufferInBytes = vd->GetSizeInBytes( 0 ) * vertexCount;

	// Allocate a temporary buffer for vertices.
	std::shared_ptr< unsigned char > vertices( new unsigned char[ sizeOfBufferInBytes ] );

	// Lock buffer for writing.
	unify::DataLock lock( vertices.get(), vd->GetSizeInBytes( 0 ), vertexCount, unify::DataLockAccess::ReadWrite, 0 );

	// Set a depth value to be used with all vertices.
	float depth = 0.5f;

	// Vertex elements allow us to write to vertices.
	VertexElement positionE = CommonVertexElement::Position(0);
	VertexElement colorE = CommonVertexElement::Diffuse(0);

	// Write to the vertex buffer elements...

	WriteVertex( *vd, lock, 0, positionE, unify::V3< float >( 0, 0, depth ) );
	WriteVertex( *vd, lock, 0, colorE, unify::ColorUnit( 1, 0, 0, 1 ) );

	WriteVertex( *vd, lock, 1, positionE, unify::V3< float >( width, 0, depth ) );
	WriteVertex( *vd, lock, 1, colorE, unify::ColorUnit( 0, 1, 0, 1 ) );

	WriteVertex( *vd, lock, 2, positionE, unify::V3< float >( 0, height, depth ) );
	WriteVertex( *vd, lock, 2, colorE, unify::ColorUnit( 0, 0, 1, 1 ) );

	WriteVertex( *vd, lock, 3, positionE, unify::V3< float >( width, height, depth ) );
	WriteVertex( *vd, lock, 3, colorE, unify::ColorUnit( 1, 1, 1, 1 ) );

	// Create a vertex buffer from our temporary buffer...
	m_vertexBuffer = GetOS()->GetRenderer( 0 )->ProduceVB( 
	{ 
		m_effect->GetVertexShader()->GetVertexDeclaration(), 
		{ 
			{ vertexCount, vertices.get() } 
		}, 
		BufferUsage::Default 
	} );

	// Add a test for an asset that doesn't exist, that we can resume from.
	{ 
		m_effect = GetManager< Effect >()->Add( "invalid", unify::Path( "Invalid.effect" ) );
	}
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	const float width = (float)params.renderer->GetDisplay().GetSize().width;
	const float height = (float)params.renderer->GetDisplay().GetSize().height;
	params.renderInfo.SetProjectionMatrix( unify::MatrixOrthoOffCenterLH( 0, width, height, 0, 0.01f, 100.0f ) );
}

void MainScene::OnRender( me::scene::RenderGirl renderGirl )
{
	m_vertexBuffer->Use();

	RenderMethod method( RenderMethod::CreateTriangleStrip( 0, 2 ) );

	unify::Matrix instance{ unify::MatrixIdentity() };
	renderGirl.GetParams()->renderer->Render( renderGirl.GetParams()->renderInfo, method, m_effect, m_vertexCB.get(), m_pixelCB.get(), render::MatrixFeed( MatrixFood_Matrices{ &instance, 1 }, 1 ) );
}
