// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>
#include <me/render/RenderMethod.h>
#include <me/render/VertexUtil.h>
#include <me/scene/SceneManager.h>
#include <me/object/Object.h>
#include <me/render/Mesh.h>
#include <sg/ShapeCreators.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/object/component/BBoxRendererComponent.h>
#include <me/scene/component/AutoBBoxSceneComponent.h>
#include <me/object/component/CameraComponent.h>

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
	AddResources( unify::Path( "resources/Standard.me_res" ) );
	auto colorEffect = GetAsset< Effect >( "ColorSimple" );
	auto textureEffect = GetAsset< Effect>( "TextureSimple" );
	auto textureAndColorEffect = GetAsset< Effect>( "TextureAndColor" );
	auto fourColorsEffect = GetAsset< Effect >( "FourColors" );
	auto colorAmbientEffect = GetAsset< Effect >( "ColorAmbient" );
	auto textureAmbientEffect = GetAsset< Effect >( "TextureAmbient" );
	auto colorInstancedAmbientEffect = GetAsset< Effect >( "ColorAmbientInstanced" );

	// Load effect...
	auto colorAnimInstancedEfect = GetManager< Effect>()->Add( "ColorAnimInstanced", unify::Path( "ColorAnimInstanced.effect" ) );

	// Add a camera...
	Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->AddComponent( object::component::IObjectComponent::ptr( new object::component::CameraComponent() ) );
	auto * cameraComponent = unify::polymorphic_downcast< object::component::CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800 / 600, 1, 1000 ) );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -12 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	auto object = GetObjectAllocator()->NewObject( "object" );

	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", unify::Path( "Mickey_Mouse/Mickey_Mouse.dae" ) ) );
	Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", unify::Path( "windmill1_01.dae" ) ) );
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", unify::Path( "CubeSphereCylinder.dae" ) ) );
	
	
	Mesh* mesh = dynamic_cast<Mesh *>( meshDAE.get() );
	{
		// Add a skeleton effect, so we can see the skeleton.
		mesh->SetSkeletonEffect( colorEffect );

		using namespace unify;
		Matrix modelMatrix = MatrixIdentity();

		// Fix mesh orientation and scale...
		unify::V3< float > sizeV3{ meshDAE->GetBBox().Size().width, meshDAE->GetBBox().Size().height, meshDAE->GetBBox().Size().depth };
		modelMatrix.Scale( sizeV3 / 4.0f );
		modelMatrix.RotateAboutAxis( unify::V3< float >( 1.0f, 0, 0 ), unify::AngleInDegrees( 270.0f ) );
		modelMatrix.RotateAboutAxis( unify::V3< float >( 0, 1.0f, 0 ), unify::AngleInDegrees( -90.0f ) );

		//modelMatrix.Scale( 8.0f / meshDAE->GetBBox().Size().Length() );


		AddGeometryComponent( object, meshDAE, modelMatrix );

		// TODO: While debugging the skeleton animation.
		//mesh->SetRenderMesh( false );
	}

	object->GetFrame().SetPosition( unify::V3< float >( 0, 0, 0 ) );
	object->AddComponent( object::component::IObjectComponent::ptr( new object::component::BBoxRendererComponent( GetOS(), colorEffect ) ) );
	const unify::BBox< float > & bboxD = meshDAE->GetBBox();
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	// Use of camera controls to simplify camera movement...
	Object * camera = FindObject( "camera" );
	assert( camera );

	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );


	// object...
	static float x = 0.0f;
	auto object = GetObjectAllocator()->FindObject( "object" );
	object->GetFrame().SetPosition( unify::V3< float >( x < 10.0f ? (-5.0f + x) : (15.0f - x) , 0, 0 ) );
	x += params.GetDelta().GetSeconds();
	if( x >= 20.0f ) x = 0.0f;
}
