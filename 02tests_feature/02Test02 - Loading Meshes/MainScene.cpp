// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>
#include <me/render/RenderMethod.h>
#include <me/render/VertexUtil.h>
#include <me/scene/SceneManager.h>
#include <me/object/Object.h>
#include <me/render/Mesh.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/object/component/BBoxRendererComponent.h>
#include <me/scene/component/AutoBBoxSceneComponent.h>
#include <me/object/component/CameraComponent.h>

using namespace me;
using namespace render;

MainScene::MainScene( me::game::Game * gameInstance )
	:Scene( gameInstance, "main" )
{	
}

void MainScene::OnStart()
{
	using namespace unify;
	using namespace object;

	AddResources( unify::Path( "resources/Standard.me_res" ) );
	auto colorEffect = GetAsset< Effect >( "ColorSimple" );
	auto textureEffect = GetAsset< Effect>( "TextureSimple" );
	auto textureAndColorEffect = GetAsset< Effect>( "TextureAndColor" );
	auto fourColorsEffect = GetAsset< Effect >( "FourColors" );


	// Add a camera...
	Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->AddComponent( component::IObjectComponent::ptr( new component::CameraComponent() ) );
	auto * cameraComponent = polymorphic_downcast< component::CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800/600, 1, 1000 ) );
	camera->GetFrame().SetPosition( { 0, 5, -17 } );
	camera->GetFrame().LookAt( { 0, 0, 0 } );

	auto shapeCreator = GetManager< Geometry >()->GetFactory( "me_shape" );

	// From dynamically generated geometry (shape creator)...
	{
		Parameters parameters {
			{ "type", "box" },
			{ "effect", colorEffect },
			{ "size3", Size3< float >( 2, 2, 2 ) },
			{ "diffuses", std::vector< Color >{ 
				ColorRed(), ColorGreen(), ColorBlue(), ColorYellow(), ColorCyan(), ColorMagenta() 
				}
			}
		};

		Geometry::ptr meshProg( shapeCreator->Produce( parameters ) );
		PrimitiveList & plProg = ((Mesh*)meshProg.get())->GetPrimitiveList();
		auto object = GetObjectAllocator()->NewObject( "cubeDyna" );
		AddGeometryComponent( object, meshProg );
		object->GetFrame().SetPosition( { 0 - 0.0f, 0, 0 } );
		object->AddComponent( component::IObjectComponent::ptr( new object::component::BBoxRendererComponent( GetOS(), colorEffect ) ) );
	}

	// From an XML file...
	{
		Geometry::ptr meshXML( GetManager< Geometry >()->Add( "cubeXML", Path( "cube.xml" ) ) );
		PrimitiveList & plXML = ((Mesh*)meshXML.get())->GetPrimitiveList();
		auto object = GetObjectAllocator()->NewObject( "XMLObject" );
	
		Matrix modelMatrix = MatrixIdentity();
		modelMatrix.Scale( 0.10f );
		modelMatrix.Translate( { 0 - 2.5f, 0, 0 } );
	
		AddGeometryComponent( object, meshXML, modelMatrix );
		object->AddComponent( component::IObjectComponent::ptr( new object::component::BBoxRendererComponent( GetOS(), colorEffect ) ) );
	}

	// From an ASE file...
	{
		Geometry::ptr meshASE( GetManager< Geometry >()->Add( "swordASE", Path( "ASE_SwordTextured.ASE" ) ) );
		PrimitiveList & plASE = ((Mesh*)meshASE.get())->GetPrimitiveList();
		auto object = GetObjectAllocator()->NewObject( "swordASE" );

		Matrix modelMatrix = MatrixIdentity();
		modelMatrix.Scale( 0.090f );
		modelMatrix.RotateAboutAxis( { -1.0f, 0.0f, 0.0f }, AngleInDegrees( 90 ) );
		modelMatrix.Translate( { 0, 1.0f, 0.0f } );

		AddGeometryComponent( object, meshASE, modelMatrix  );
		object->GetFrame().SetPosition( { 0 + 2.5f, 0, 0 } );
		object->AddComponent( component::IObjectComponent::ptr( new object::component::BBoxRendererComponent( GetOS(), colorEffect ) ) );
	}

	{ 
		auto object = GetObjectAllocator()->NewObject( "daeModel" );

		// DISABLED: Various other dae models we can/should test.
		//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", Path( "USSVoyager.dae" ) ) );
		//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", Path( "models/Death Star II/models/Death Star II.dae" ) ) );
		//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", Path( "enterprise.dae" ) ) );
		//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", Path( "cube.dae" ) ) );
		//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", Path( "borgcube.dae" ) ) );
		//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", Path( "torus.dae" ) ) );

		// Rigged...
		Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", Path( "Mickey_Mouse/Mickey_Mouse.dae" ) ) );
		//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "SuperMarioGalaxy_Mario/mario.dae" ) );

		Matrix modelMatrix = MatrixIdentity();
		unify::V3< float > sizeV3{ meshDAE->GetBBox().Size().width, meshDAE->GetBBox().Size().height, meshDAE->GetBBox().Size().depth };
		modelMatrix.Scale( sizeV3 / 4.0f );
		modelMatrix.RotateAboutAxis( { 1.0f, 0, 0 }, AngleInDegrees( 270.0f ) );
		modelMatrix.RotateAboutAxis( { 0, 1.0f, 0 }, AngleInDegrees( -90.0f ) );
		AddGeometryComponent( object, meshDAE, modelMatrix );
		object->GetFrame().SetPosition( { 0 - 5.0f, 0, 0 } );
		object->AddComponent( component::IObjectComponent::ptr( new object::component::BBoxRendererComponent( GetOS(), colorEffect ) ) );
	}
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	using namespace object;

	// Use of camera controls to simplify camera movement...
	Object * camera = FindObject( "camera" );
	
	camera->GetFrame().Orbit( { 0, 0, 0 }, { 1, 0 }, unify::AngleInRadians( params.GetDelta().GetSeconds() ) );
	camera->GetFrame().LookAt( { 0, 0, 0 }, { 0, 1, 0 } );
}
