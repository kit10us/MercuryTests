// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>
#include <me/render/Mesh.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/scene/component/AutoBBoxSceneComponent.h>
#include <me/object/component/CameraComponent.h>
#include <me/canvas/CanvasComponent.h>
#include <me/canvas/FPS.h>

using namespace me;
using namespace render;
using namespace scene;
using namespace object;

MainScene::MainScene( me::game::Game * gameInstance )
	:Scene( gameInstance, "main" )
{	
}

void MainScene::OnStart()
{
	Effect::ptr color3DEffect = GetManager< Effect >()->Add("ColorInstancedAmbient", unify::Path("ColorAmbientInstanced.me_effect"));

	AddComponent(scene::component::SceneComponent::ptr(new scene::component::AutoBBoxSceneComponent(GetOS(), color3DEffect)));

	// Add a camera...
	Object * camera = GetObjectAllocator()->NewObject("camera");
	camera->AddComponent(object::component::IObjectComponent::ptr(new object::component::CameraComponent()));
	auto * cameraComponent = unify::polymorphic_downcast< object::component::CameraComponent * >(camera->GetComponent("camera").get());
	cameraComponent->SetProjection(unify::MatrixPerspectiveFovLH(3.141592653589f / 4.0f, GetOS()->GetRenderer(0)->GetDisplay().GetSize().AspectRatioWH(), 1.0f, 1000.0f));

	auto shapeCreator = GetManager< Geometry >()->GetFactory("me_shape");

	// Geo1
	Geometry::ptr geo1;
	{
		unify::Parameters parameters{
			{ "type", (std::string)"box" },
			{ "effect", color3DEffect },
			{ "size3", unify::Size3< float >(1, 1, 1) },
			{ "diffuse", unify::ColorBlue() }
		};
		geo1 = shapeCreator->Produce(parameters);
	}

	// Geo2
	Geometry::ptr geo2;
	{
		unify::Parameters parameters{
			{ "type", (std::string)"sphere" },
			{ "effect", color3DEffect },
			{ "radius", 1.0f },
			{ "diffuse", unify::ColorRed() }
		};
		geo2 = shapeCreator->Produce(parameters);
	}

	// Geo3
	Geometry::ptr geo3;
	{
		unify::Parameters parameters{
			{ "type", (std::string)"cone" },
			{ "effect", color3DEffect },
			{ "radius", 1.0f },
			{ "height", 1.0f },
			{ "diffuse", unify::ColorGreen() }
		};
		geo3 = shapeCreator->Produce(parameters);
	}

	size_t depth = 10;
	size_t columns = 10;
	size_t rows = 10;
	float spacing = 2.0f;
	for( size_t d = 0; d < depth; ++d )
	{
		for( size_t c = 0; c < columns; c++ )
		{
			for( size_t r = 0; r < rows; ++r )
			{			
				size_t shape = (columns + d + r) % 3;

				auto object = GetObjectAllocator()->NewObject( "geo" );
				AddGeometryComponent( object, (shape == 0 ) ? geo1 : (shape == 1) ? geo2 : geo3 );

				float x = (rows * spacing * -0.5f) + r * spacing;
				float y = (columns * spacing * -0.5f) + c * spacing;
				float z = (depth * spacing * -0.5f) + d * spacing;

				object->GetFrame().SetPosition( unify::V3< float >( x, y, z ) );
			}
		}
	}

	// Camera is based on number of objects
	float max = (float)std::max( std::max( depth, columns), rows );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, max * spacing * 0.5f, 0 - max * spacing * 2.0f ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );
																			   
	canvas::CanvasComponent::ptr canvas( new canvas::CanvasComponent( GetGame() ) );
	AddComponent( canvas );

	Effect::ptr font2 = GetManager< Effect>()->Add( "font2", unify::Path( "font2.me_effect" ) );
	canvas->GetLayer()->AddElement( canvas::IElement::ptr( new canvas::FPS( GetGame(), font2 ) ) );
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	// Use of camera controls to simplify camera movement...
	Object * camera = FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) );
	//camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
	
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}
