// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>
#include <me/render/Mesh.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/object/component/BBoxRendererComponent.h>
#include <me/object/component/CameraComponent.h>
#include <me/render/Billboard.h>

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
	using namespace unify;

	AddResources( unify::Path( "resources/Standard.me_res" ) );
	auto colorEffect = GetAsset< Effect >( "ColorSimple" );
	auto textureEffect = GetAsset< Effect>( "TextureSimple" );
	auto textureAndColorEffect = GetAsset< Effect>( "TextureAndColor" );
	auto fourColorsEffect = GetAsset< Effect >( "FourColors" );
	auto colorAmbientEffect = GetAsset< Effect >( "ColorAmbient" );
	auto textureAmbientEffect = GetAsset< Effect >( "TextureAmbient" );
	auto colorInstancedAmbientEffect = GetAsset< Effect >( "ColorAmbientInstanced" );

	// Add an object to act as a camera...
	Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->GetFrame().SetPosition( V3< float >( 0, 5, -17 ) );
	camera->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );

	// Add a camera component to the camera object
	auto * cameraComponent = new object::component::CameraComponent();
	cameraComponent->SetProjection( MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800 / 600, 1, 1000 ) );
	camera->AddComponent( object::component::IObjectComponent::ptr( cameraComponent ) );

	// Create shapes...
	auto shapeCreator = GetManager< Geometry >()->GetFactory( "me_shape" );

	auto createObject = [&]( Parameters parameters, int depth )->me::object::Object*
	{
		const V3< float > startPos{ -2.5f, 2.5f, -1.25f + ( 2.5f * depth )  };
		const int itemsPerRow = 4;
		const int itemsPerColumn = 4;
		const int itemsPerPage = itemsPerRow * itemsPerColumn;
		const V3< float > changePerRow { 0.0f, -2.5f, 0.0f };
		const V3< float > changePerColumn { 2.5f, 0.0f, 0.0f };
		const V3< float > changePerPage { 0.0f, 0.0f, 2.5f };

		static int objectIndex[2] = {};
		int page = objectIndex[ depth ] / itemsPerPage;
		int row = (objectIndex[depth] % itemsPerPage) / itemsPerRow;
		int column = (objectIndex[depth] % itemsPerPage) % itemsPerRow;
			
		std::string objectName = "object " + Cast< std::string >( objectIndex[depth]++ );
		auto object = GetObjectAllocator()->NewObject( objectName );
		
		unify::V3< float > pos = startPos + V3< float >{ 
			(changePerRow * (float)row) + (changePerColumn * (float)column) + (changePerPage * (float)page ) };
		object->GetFrame().SetPosition( pos );

		Geometry::ptr meshProg( shapeCreator->Produce(parameters) );
		PrimitiveList & plProg = ((Mesh*)meshProg.get())->GetPrimitiveList();
		AddGeometryComponent(object, meshProg);
		
		return object;
	};

	// Create objects...
	for( int depth = 0; depth < 2; depth++ )
	{
		Parameters base;
		if ( depth == 0 )
		{
			base.Set( { "effect", colorEffect } );
		}
		else
		{
			base.Set( { "effect", textureEffect } );
		}
		{
			Parameters parameters{ base };
			parameters.Set( "type", "box" );
			parameters.Set( "size3", Size3< float >( 1, 1, 1 ) );
			parameters.Set( "diffuses", std::vector< Color >{
				ColorRed(), ColorGreen(), ColorBlue(), ColorYellow(), ColorCyan(), ColorMagenta()
			} );
			auto object = createObject( parameters, depth );
		}

		{
			Parameters parameters{ base };
			parameters.Set( "type", "pointfield" );
			parameters.Set( "MajorRadius", 0.5f );
			parameters.Set( "MinorRadius", 0.5f );
			parameters.Set( "Count", 1000 );
			parameters.Set( "Diffuse", ColorRed() );
			auto object = createObject( parameters, depth );
		}

		{
			Parameters parameters{ base };
			parameters.Set( "type", "pointring" );
			parameters.Set( "MajorRadius", 0.5f );
			parameters.Set( "MinorRadius", 0.25f );
			parameters.Set( "Count", 1000 );
			parameters.Set( "Diffuse", ColorGreen() );
			auto object = createObject( parameters, depth );
		}

		{
			Parameters parameters{ base };
			parameters.Set( "type", "dashring" );
			parameters.Set( "MajorRadius", 1.0f );
			parameters.Set( "MinorRadius", 0.9f );
			parameters.Set( "Size1", 0.5f );
			parameters.Set( "Count", 12 );
			parameters.Set( "Diffuse", ColorBlue() );
			auto object = createObject( parameters, depth );
		}

		{
			Parameters parameters{ base };
			parameters.Set( "type", "pyramid" );
			parameters.Set( "Size3", Size3<float>{ 1, 1, 1 } );
			parameters.Set( "Diffuse", ColorRed( 255 / 2 ) );
			auto object = createObject( parameters, depth );
		}

		{
			Parameters parameters{ base };
			parameters.Set( "type", "circle" );
			parameters.Set( "Radius", 1.0f );
			parameters.Set( "Diffuse", ColorGreen( 255 / 2 ) );
			auto object = createObject( parameters, depth );
		}

		{
			Parameters parameters{ base };
			parameters.Set( "type", "sphere" );
			parameters.Set( "Radius", 0.5f );
			parameters.Set( "Diffuse", ColorBlue( 255 / 2 ) );
			auto object = createObject( parameters, depth );
		}

		{
			Parameters parameters{ base };
			parameters.Set( "type", "cylinder" );
			parameters.Set( "Radius", 0.25f );
			parameters.Set( "Height", 1.0f );
			parameters.Set( "Diffuse", ColorCyan( 255 / 2 ) );
			auto object = createObject( parameters, depth );
		}

		{
			Parameters parameters{ base };
			parameters.Set( "type", "tube" );
			parameters.Set( "MajorRadius", 0.5f );
			parameters.Set( "MinorRadius", 0.25f );
			parameters.Set( "Height", 1.0f );
			parameters.Set( "Diffuse", ColorMagenta( 255 / 2 ) );
			auto object = createObject( parameters, depth );
		}

		{
			Parameters parameters{ base };
			parameters.Set( "type", "plane" );
			parameters.Set( "Size2", Size<float>{ 1, 1 } );
			parameters.Set( "Diffuse", ColorRed( 255 ) );
			auto object = createObject( parameters, depth );
		}

		{
			Parameters parameters{ base };
			parameters.Set( "type", "cone" );
			parameters.Set( "Radius", 0.25f );
			parameters.Set( "Height", 1.0f );
			parameters.Set( "Diffuse", ColorGreen( 255 ) );
			auto object = createObject( parameters, depth );
		}
	}
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	// Use of camera controls to simplify camera movement...
	Object * camera = FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}
