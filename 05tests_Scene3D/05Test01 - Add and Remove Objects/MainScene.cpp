// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>
#include <me/render/Mesh.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/object/component/BBoxRendererComponent.h>
#include <me/object/component/CameraComponent.h>
#include <sg/ShapeCreators.h>
#include <me/render/LOD.h>

using namespace me;
using namespace render;
using namespace scene;

MainScene::MainScene( me::game::Game * gameInstance )
	:Scene( gameInstance, "main" )
	, m_objectIndex{ -1 }
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

	// Add a camera...
	object::Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->AddComponent( object::component::IObjectComponent::ptr( new object::component::CameraComponent() ) );
	auto * cameraComponent = unify::polymorphic_downcast< object::component::CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800/600, 1, 1000 ) );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	// Use of camera controls to simplify camera movement...
	object::Object * camera = FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );

	static unify::TimeDelta timer = {};
	timer += params.GetDelta();
	if( timer > unify::TimeDeltaInSeconds( 1.0f ) )
	{
		timer = unify::TimeDelta();

		// Delete existing object...
		if( m_objectIndex > 0 )
		{
			std::string objectName = "object " + unify::Cast< std::string >( m_objectIndex );
			auto object = GetObjectAllocator()->FindObject( objectName );
			GetObjectAllocator()->DestroyObject( object );
		}

		// Add new object...
		m_objectIndex++;
		if( m_objectIndex > 2 )
		{
			m_objectIndex = 0;
		}

		// Add object...
		if( m_objectIndex > 0 )
		{
			std::string objectName = "object " + unify::Cast< std::string >( m_objectIndex );
			auto object = GetObjectAllocator()->NewObject( objectName );
			object->GetFrame().SetPosition( unify::V3< float >( 0 - 0.0f, 0, 0 ) );

			// Add geometry
			Effect::ptr colorEffect = GetManager< Effect >()->Find( "color3d" );

			if( m_objectIndex == 1 )
			{
				sg::CubeParameters cubeParameters;
				cubeParameters.SetEffect( colorEffect );
				cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
				cubeParameters.SetDiffuseFaces( unify::ColorRed(), unify::ColorGreen(), unify::ColorBlue(), unify::ColorYellow(), unify::ColorCyan(), unify::ColorMagenta() );
				Geometry::ptr meshProg( sg::CreateShape( GetOS()->GetRenderer( 0 ), cubeParameters ) );
				PrimitiveList & plProg = ( (Mesh*)meshProg.get() )->GetPrimitiveList();

				auto component = AddGeometryComponent( object, meshProg );
				component->GetLookup()->SetValue( "alias", "cube" );

			}
			else
			{
				sg::SphereParameters sphereParameters;
				sphereParameters.SetEffect( colorEffect );
				sphereParameters.SetRadius( 2 );
				sphereParameters.SetDiffuse( unify::ColorRed() );
				Geometry::ptr meshProg( sg::CreateShape( GetOS()->GetRenderer( 0 ), sphereParameters ) );
				PrimitiveList & plProg = ( (Mesh*)meshProg.get() )->GetPrimitiveList();

				auto component = AddGeometryComponent( object, meshProg );
				component->GetLookup()->SetValue( "alias", "sphere" );
			}
		}
	}
}
