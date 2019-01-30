// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <Windows.h>
#include <MainScene.h>
#include <me/render/RenderMethod.h>
#include <me/object/component/CameraComponent.h>
#include <me/canvas/CanvasComponent.h>
#include <me/canvas/Layer.h>
#include <me/canvas/TextElement.h>
#include <me/canvas/Image.h>
#include <me/canvas/VList.h>
#include <me/canvas/Grid.h>

using namespace me;
using namespace render;
using namespace scene;

MainScene::MainScene( me::game::Game * gameInstance )
	:Scene( gameInstance, "main" )
{	
}

void MainScene::OnStart()
{
	using namespace scene;

	effect = GetManager< Effect>()->Add( "texture3d", unify::Path( "EffectTexture.effect" ) );

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
	vertexBuffer = GetOS()->GetRenderer( 0 )->ProduceVB( { effect->GetVertexShader()->GetVertexDeclaration(), { { numberOfVertices, vbRaw } }, BufferUsage::Default } );

	// Add Canvas component...
	// A canvas component takes care of drawing all 2d elements.
	canvas::CanvasComponent::ptr canvas( new canvas::CanvasComponent( GetGame() ) );
	AddComponent( canvas );

	// Load a font enabled texture...
	// That is a tecture with an internal description of how to treat is as a font.
	Effect::ptr font2 = GetManager< Effect>()->Add( "font2", unify::Path( "font2.effect" ) );

	// A layer is where we place all canvas elements (typically things we want drawn).
	{ // Create a new layer, and add it to the canvas...
		canvas::Layer::ptr layer( new canvas::Layer( GetGame() ) );
		canvas->GetLayer()->AddElement( layer, "layer1" );

		// Add grid...
		auto vlist = new canvas::VList( GetGame(), 10, { 100, 600 }, me::canvas::Anchor::TopLeft );
		layer->AddElement( canvas::IElement::ptr{ vlist } );

		// Add text elements to the layer.
		vlist->AddItem( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "First item" ) ) );
		vlist->AddItem( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "Second item" ) ) );
		vlist->AddItem( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "Thrid item" ) ) );
		vlist->AddItem( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "Fourth item" ) ) );
		vlist->AddItem( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "Fifth item" ) ) );
	}

	/*
	{ // Add the heart layer...
		Effect::ptr heart = GetManager< Effect >()->Add( "heart", unify::Path( "heart.effect" ) );
		canvas::Layer::ptr layerHeart( new canvas::Layer( GetGame() ) );
		canvas->GetLayer()->AddElement( layerHeart, "layerHeart" );
		layerHeart->AddElement( 
			canvas::IElement::ptr( 
				new canvas::Image( GetGame(), heart, canvas::Anchor::TopRight, { 0.25f, 0.25f } ) 
			) 
		);
	}
	*/
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
	
	// Cycle layers...
	static int layer = 0;
	static float time = 0.0f;
	const float cycleAt = 3.0f;
	time += params.GetDelta().GetSeconds();
	while( time >= cycleAt )
	{
		time -= cycleAt;
		layer++;
		if ( layer > 4 ) layer = 0;
	}

	/*
	// Enable current layer, disable all the others...
	auto canvas = GetComponentT< canvas::CanvasComponent >();
	canvas->GetLayer()->FindElement( "layer1" )->SetEnabled( layer == 0 ? true : false );
	canvas->GetLayer()->FindElement( "layer2" )->SetEnabled( layer == 1 ? true : false );
	canvas->GetLayer()->FindElement( "layer3" )->SetEnabled( layer == 2 ? true : false );
	canvas->GetLayer()->FindElement( "layer4" )->SetEnabled( layer == 3 ? true : false );
	canvas->GetLayer()->FindElement( "layer5" )->SetEnabled( layer == 4 ? true : false );
	*/
}
