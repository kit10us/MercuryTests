// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>
#include <me/render/Mesh.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/object/component/BBoxRendererComponent.h>
#include <me/object/component/CameraComponent.h>
#include <sg/ShapeCreators.h>

#include <me/render/Billboard.h>

using namespace me;
using namespace render;
using namespace scene;
using namespace object;

#include <SolarBody.h>

MainScene::MainScene( me::game::Game * gameInstance )
	:Scene( gameInstance, "main" )
{	
}

void MainScene::OnStart()
{
	using namespace unify;

	AddResources( unify::Path( "resources/Standard.me_res" ) );
	auto colorEffect = GetAsset< Effect >( "ColorSimple" );
	auto textureEffect = GetAsset< Effect>( "TextureSimple" );
	auto textureAndColorEffect = GetAsset< Effect>( "TextureAndColor" );
	auto fourColorsEffect = GetAsset< Effect >( "FourColors" );
	auto colorAmbientEffect = GetAsset< Effect >( "ColorAmbient" );
	auto textureAmbientEffect = GetAsset< Effect >( "TextureAmbient" );
	auto colorInstancedAmbientEffect = GetAsset< Effect >( "ColorAmbientInstanced" );

	auto AddPlanetEffect = [&]( std::string name, std::string path )
	{
		auto effect{ textureEffect->Duplicate() };
		effect->SetTexture(0, GetManager< ITexture >()->Add( name, unify::Path(path)));
		return effect;
	};

	// Add an object to act as a camera...
	Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->GetFrame().SetPosition( V3< float >( 0, 5, -17 ) );
	camera->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );

	// Add a camera component to the camera object
	auto * cameraComponent = new object::component::CameraComponent();
	cameraComponent->SetProjection( MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800 / 600, 1, 1000 ) );
	camera->AddComponent( object::component::IObjectComponent::ptr( cameraComponent ) );

	auto createObject = [&]( sg::ShapeBaseParameters parameters ) //->me::object::Object*
	{
		const unify::V3< float > startPos{ V3< float >::V3Zero() };
		
		static int objectIndex = 0;
		std::string objectName = "object " + unify::Cast< std::string >( objectIndex++ );
		auto object = GetObjectAllocator()->NewObject( objectName );
		
		object->GetFrame().SetPosition(startPos);

		Geometry::ptr meshProg(sg::CreateShape(GetOS()->GetRenderer(0), parameters));
		PrimitiveList & plProg = ((Mesh*)meshProg.get())->GetPrimitiveList();
		AddGeometryComponent(object, meshProg);
		
		return object;
	};

	// Create solar bodies...

	{
		sg::SphereParameters parameters;
		parameters.SetEffect( AddPlanetEffect( "00sun", "solar\\00sun.jpg" ) );
		parameters.SetRadius( 1.0f );
		auto object = createObject(parameters);

		m_rootSolarBody.reset( 
			new SolarBody( "Sun", object, 
				SpacialData( 
					MatrixIdentity(), 
					V3< float >::V3Y( 1.0f ), 
					TimeDeltaInSeconds( 10.0f ),
					TimeDeltaInSeconds( 10.0f ),
					0.0f
				) 
			) 
		);
	}

	{
		sg::SphereParameters parameters;
		parameters.SetEffect( AddPlanetEffect( "01Mercury", "solar\\01Mercury.bmp" ) );
		parameters.SetRadius(0.5f);
		auto object = createObject(parameters);

		m_rootSolarBody->AddChild( 
			SolarBody::ptr( 
				new SolarBody( "Mercury", object, 
					SpacialData( 
						MatrixTranslate( { 3.0f, 0.0f, 0.0f } ), 
						V3< float >::V3Y( 1.0f ),
						TimeDeltaInSeconds( 0.0f ),
						TimeDeltaInSeconds( 0.0f ),
						87.969f
						) 
					) 
				)
			);
	}

	{
		sg::SphereParameters parameters;
		parameters.SetEffect( AddPlanetEffect( "02Venus", "solar\\02Venus.bmp" ) );
		parameters.SetRadius( 0.5f );
		auto object = createObject( parameters );

		m_rootSolarBody->AddChild(
			SolarBody::ptr(
				new SolarBody( "Venus", object,
					SpacialData(
						MatrixTranslate( { 5.0f, 0.0f, 0.0f } ),
						V3< float >::V3Y( 1.0f ),
						TimeDeltaInSeconds( 5.0f ),
						TimeDeltaInSeconds( 0.0f ),
						224.7f
						)
					)
				)
			);
	}

	{
		sg::SphereParameters parameters;
		parameters.SetEffect( AddPlanetEffect( "03Earth", "solar\\03Earth.bmp" ) );
		parameters.SetRadius( 0.5f );
		auto object = createObject( parameters );

		SolarBody::ptr earth(
				new SolarBody( "Earth", object,
					SpacialData(
						MatrixTranslate( { 7.0f, 0.0f, 0.0f } ),
						V3< float >( 0.0f, 1.0f, 0.0f ),
						TimeDeltaInSeconds( 5.0f ),
						TimeDeltaInSeconds( 0.0f ),
						365.2564f
						)
					)
				);
		m_rootSolarBody->AddChild( earth );
		/*
		{
			sg::SphereParameters parameters;
			parameters.SetEffect( AddPlanetEffect( "03.01Moon", "solar\\03.01Moon.jpg" ) );
			parameters.SetRadius( 0.25f );
			auto object = createObject( parameters );

			earth->AddChild(
				SolarBody::ptr(
					new SolarBody( "Earth's Moon", object,
						SpacialData(
							unify::MatrixTranslate( { 2.0f, 0.0f, 0.0f } ),
							V3< float >( 0.0f, 1.0f, 0.0f ),
							TimeDeltaInSeconds( 5.0f ),
							TimeDeltaInSeconds( 0.0f ),
							27.3f
							)
						)
					)
				);

		}
		*/
	}
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	// Use of camera controls to simplify camera movement...
	Object * camera = FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.GetDelta().GetSeconds() * 0.015f ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );

	// Update solar system...
	if ( m_rootSolarBody )
	{
		m_rootSolarBody->Update( params, nullptr, 10.0f );
	}
}
