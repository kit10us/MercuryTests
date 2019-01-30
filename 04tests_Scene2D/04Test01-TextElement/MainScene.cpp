// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>
#include <me/render/RenderMethod.h>
#include <me/object/component/CameraComponent.h>
#include <me/canvas/CanvasComponent.h>
#include <me/canvas/Layer.h>
#include <me/canvas/TextElement.h>

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
	
	AddResources( unify::Path( "resources/Standard.me_res" ) );
	auto colorEffect = GetAsset< Effect >( "ColorSimple" );
	auto textureEffect = GetAsset< Effect>( "TextureSimple" );
	auto textureAndColorEffect = GetAsset< Effect>( "TextureAndColor" );
	auto fourColorsEffect = GetAsset< Effect >( "FourColors" );
	auto colorAmbientEffect = GetAsset< Effect >( "ColorAmbient" );
	auto textureAmbientEffect = GetAsset< Effect >( "TextureAmbient" );
	auto colorInstancedAmbientEffect = GetAsset< Effect >( "ColorAmbientInstanced" );

	// Add font enabled assets.
	AddResources( unify::Path( "resources/Fonts.me_res" ) );
	auto font2Effect = GetAsset< Effect >( "Font2" );

	// Add Canvas component...
	// A canvas component takes care of drawing all 2d elements.
	canvas::CanvasComponent::ptr canvas( new canvas::CanvasComponent( GetGame() ) );
	AddComponent( canvas );

	// A layer is where we place all canvas elements (typically things we want drawn).
	{ // Create a new layer, and add it to the canvas...
		canvas::Layer::ptr layer1( new canvas::Layer( GetGame() ) );
		canvas->GetLayer()->AddElement( layer1, "layer1" );

		// Add text elements to the layer.
		layer1->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "0,0", canvas::Anchor::Free, { 0.5f, 0.5f }, { 0, 0 } ) ) );
		layer1->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "100,10", canvas::Anchor::Free, { 0.5f, 0.5f }, { 100, 10 } ) ) );
		layer1->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "10,100", canvas::Anchor::Free, { 0.5f, 0.5f }, { 10, 100 } ) ) );
		layer1->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "400,200", canvas::Anchor::Free, { 0.5f, 0.5f }, { 400, 200 } ) ) );
		layer1->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "200,400", canvas::Anchor::Free, { 0.5f, 0.5f }, { 200, 400 } ) ) );
	}

	{ // Add another layer with additional text elements...
		canvas::Layer::ptr layer2( new canvas::Layer( GetGame() ) );
		canvas->GetLayer()->AddElement( layer2, "layer2" );
		layer2->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "TL", canvas::Anchor::TopLeft ) ) );
		layer2->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "TOP", canvas::Anchor::Top ) ) );
		layer2->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "TR", canvas::Anchor::TopRight ) ) );
		layer2->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "L", canvas::Anchor::Left ) ) );
		layer2->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "CENTER", canvas::Anchor::Center ) ) );
		layer2->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "R", canvas::Anchor::Right ) ) );
		layer2->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "BL", canvas::Anchor::BottomLeft ) ) );
		layer2->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "BOTTOM", canvas::Anchor::Bottom ) ) );
		layer2->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "BR", canvas::Anchor::BottomRight ) ) );
	}
		
	{ // Add another layer with additional text elements...
		canvas::Layer::ptr layer3( new canvas::Layer( GetGame() ) );
		canvas->GetLayer()->AddElement( layer3, "layer3" );
		layer3->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "STRETCHTOP", canvas::Anchor::StretchTop, { 0.5f, 0.5f } ) ) );
		layer3->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "STRETCHLEFTRIGHT", canvas::Anchor::StretchLeftRight, { 0.5f, 0.5f } ) ) );
		layer3->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "STRETCHBOTTOM", canvas::Anchor::StretchBottom, { 0.5f, 0.5f } ) ) );
	}

	{ // Add another layer with additional text elements...
		canvas::Layer::ptr layer4( new canvas::Layer( GetGame() ) );
		canvas->GetLayer()->AddElement( layer4, "layer4" );
		layer4->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "STRETCHLEFT", canvas::Anchor::StretchLeft, { 0.35f, 0.35f } ) ) );
		layer4->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "STRETCHTOPBOTTOM", canvas::Anchor::StretchTopBottom, { 0.35f, 0.35f } ) ) );
		layer4->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "STRETCHRIGHT", canvas::Anchor::StretchRight, { 0.35f, 0.35f } ) ) );
	}

	{ // Add another layer with additional text elements...
		canvas::Layer::ptr layer5( new canvas::Layer( GetGame() ) );
		canvas->GetLayer()->AddElement( layer5, "layer5" );
		layer5->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2Effect, "STRETCHFULL", canvas::Anchor::StretchFull ) ) );
	}
}

void MainScene::OnUpdate( const UpdateParams & params )
{
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

	auto canvas =GetComponentT< canvas::CanvasComponent >();

	canvas->GetLayer()->FindElement( "layer1" )->SetEnabled( layer == 0 ? true : false );
	canvas->GetLayer()->FindElement( "layer2" )->SetEnabled( layer == 1 ? true : false );
	canvas->GetLayer()->FindElement( "layer3" )->SetEnabled( layer == 2 ? true : false );
	canvas->GetLayer()->FindElement( "layer4" )->SetEnabled( layer == 3 ? true : false );
	canvas->GetLayer()->FindElement( "layer5" )->SetEnabled( layer == 4 ? true : false );
}
