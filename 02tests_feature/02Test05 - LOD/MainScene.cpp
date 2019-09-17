// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>
#include <me/render/Mesh.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/object/component/BBoxRendererComponent.h>
#include <me/object/component/CameraComponent.h>
#include <me/render/LOD.h>

using namespace me;
using namespace render;

MainScene::MainScene( me::game::Game * gameInstance )
	:Scene( gameInstance, "main" )
{	
}

void MainScene::OnStart()
{
	using namespace object;

	AddResources( unify::Path( "resources/Standard.me_res" ) );

	Effect::ptr colorSimple = GetAsset< Effect >( "ColorSimple" );

	// Add a camera...
	Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->AddComponent( component::IObjectComponent::ptr( new component::CameraComponent() ) );
	auto * cameraComponent = unify::polymorphic_downcast< component::CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800/600, 1, 1000 ) );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	auto shapeCreator = GetManager< Geometry >()->GetFactory("me_shape");

	// LOD...
	//QLOD 

	// Closest...

	// Middle...

	// Furthest...
	unify::Parameters parameters { 
		{ "type", "box" },
		{ "effect", colorSimple },
		{ "size3", unify::Size3< float >( 2, 2, 2 ) },
		{ "diffuses", std::vector< unify::Color >{ 
			unify::ColorRed(), unify::ColorGreen(), unify::ColorBlue(), unify::ColorYellow(), unify::ColorCyan(), unify::ColorMagenta()
		}
		}
	};

	auto progObject = GetObjectAllocator()->NewObject( "boxDyna" );
	AddGeometryComponent( progObject, shapeCreator->Produce( parameters ) );
	progObject->GetFrame().SetPosition( unify::V3< float >( 0 - 0.0f, 0, 0 ) );
	progObject->AddComponent( component::IObjectComponent::ptr( new object::component::BBoxRendererComponent( GetOS(), colorSimple ) ) );
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	using namespace object;

	// Use of camera controls to simplify camera movement...
	Object * camera = FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) );
	//camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
	
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}
