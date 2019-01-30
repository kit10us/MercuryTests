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

	// Add an object to act as a camera...
	Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	// Add a camera component to the camera object
	auto * cameraComponent = new object::component::CameraComponent();
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800 / 600, 1, 1000 ) );
	camera->AddComponent( object::component::IObjectComponent::ptr( cameraComponent ) );

	auto createObject = [&]( sg::ShapeBaseParameters parameters, int depth )->me::object::Object*
	{
		const unify::V3< float > startPos{ -2.5f, 2.5f, -1.25f + ( 2.5f * depth )  };
		const int itemsPerRow = 4;
		const int itemsPerColumn = 4;
		const int itemsPerPage = itemsPerRow * itemsPerColumn;
		const unify::V3< float > changePerRow { 0.0f, -2.5f, 0.0f };
		const unify::V3< float > changePerColumn { 2.5f, 0.0f, 0.0f };
		const unify::V3< float > changePerPage { 0.0f, 0.0f, 2.5f };

		static int objectIndex[2] = {};
		int page = objectIndex[ depth ] / itemsPerPage;
		int row = (objectIndex[depth] % itemsPerPage) / itemsPerRow;
		int column = (objectIndex[depth] % itemsPerPage) % itemsPerRow;
			
		std::string objectName = "object " + unify::Cast< std::string >( objectIndex[depth]++ );
		auto object = GetObjectAllocator()->NewObject( objectName );
		
		unify::V3< float > pos = startPos + unify::V3< float >{ 
			(changePerRow * (float)row) + (changePerColumn * (float)column) + (changePerPage * (float)page ) };
		object->GetFrame().SetPosition( pos );

		Geometry::ptr meshProg(sg::CreateShape(GetOS()->GetRenderer(0), parameters));
		PrimitiveList & plProg = ((Mesh*)meshProg.get())->GetPrimitiveList();
		AddGeometryComponent(object, meshProg);
		
		return object;
	};

	// Create objects...
	for( int depth = 0; depth < 2; depth++ )
	{
		sg::ShapeBaseParameters base;
		if ( depth == 0 )
		{
			base.SetEffect( colorEffect );
		}
		else
		{
			base.SetEffect( textureEffect );
		}
		{
			sg::CubeParameters parameters{ base };
			parameters.SetSize(unify::Size3< float >(1, 1, 1));
			parameters.SetDiffuseFaces(unify::ColorRed(), unify::ColorGreen(), unify::ColorBlue(), unify::ColorYellow(), unify::ColorCyan(), unify::ColorMagenta());
			auto object = createObject( parameters, depth );
		}

		{
			sg::PointFieldParameters parameters{ base };
			parameters.SetMajorRadius( 0.5f );
			parameters.SetMinorRadius( 0.5f );
			parameters.SetCount( 1000 );
			parameters.SetDiffuse( unify::ColorRed() );
			auto object = createObject( parameters, depth );
		}

		{
			sg::PointRingParameters parameters{ base };
			parameters.SetMajorRadius( 0.5f );
			parameters.SetMinorRadius(0.25f);
			parameters.SetCount(1000);
			parameters.SetDiffuse( unify::ColorGreen() );
			auto object = createObject( parameters, depth );
		}

		{
			sg::DashRingParameters parameters{ base };
			parameters.SetMajorRadius(1);
			parameters.SetMinorRadius(0.9f);
			parameters.SetSize( 0.5f );
			parameters.SetCount(12);
			parameters.SetDiffuse( unify::ColorBlue() );
			auto object = createObject( parameters, depth );
		}

		{
			sg::PyramidParameters parameters{ base };
			parameters.SetEffect( colorEffect );
			parameters.SetSize( { 1, 1, 1 } );
			parameters.SetDiffuse( unify::ColorRed( 255 / 2 ) );
			auto object = createObject( parameters, depth );
		}

		{
			sg::CircleParameters parameters{ base };
			parameters.SetRadius( 1.0f );
			parameters.SetDiffuse( unify::ColorGreen( 255 / 2 ) );
			auto object = createObject( parameters, depth );
		}

		{
			sg::SphereParameters parameters{ base };
			parameters.SetRadius( 0.5f );
			parameters.SetDiffuse( unify::ColorBlue( 255 / 2 ) );
			auto object = createObject( parameters, depth );
		}

		{
			sg::CylinderParameters parameters{ base };
			parameters.SetRadius(0.25f);
			parameters.SetHeight(1.0f);
			parameters.SetDiffuse( unify::ColorCyan( 255 / 2 ) );
			auto object = createObject( parameters, depth );
		}

		{
			sg::TubeParameters parameters{ base };
			parameters.SetMajorRadius(0.5f);
			parameters.SetMinorRadius(0.25f );
			parameters.SetHeight(1.0f);
			parameters.SetDiffuse( unify::ColorMagenta( 255 / 2 ) );
			auto object = createObject( parameters, depth );
		}

		{
			sg::PlaneParameters parameters{ base };
			parameters.SetSize( {1, 1} );
			parameters.SetDiffuse( unify::ColorRed( 255 ) );
			auto object = createObject( parameters, depth );
		}

		{
			sg::ConeParameters parameters{ base };
			parameters.SetRadius( 0.25f );
			parameters.SetHeight( 1 );
			parameters.SetDiffuse( unify::ColorGreen( 255 ) );
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
