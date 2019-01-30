// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>
#include <me/render/Mesh.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/object/component/BBoxRendererComponent.h>
#include <me/object/component/CameraComponent.h>
#include <me/scene/SceneManager.h>
#include <me/canvas/CanvasComponent.h>
#include <me/canvas/FPS.h>
#include <sg/ShapeCreators.h>

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
	AddResources( unify::Path( "resources/TestTextures.me_res" ) );
	auto colorEffect = GetAsset< Effect >( "ColorSimple" );
	auto textureEffect = GetAsset< Effect>( "TextureSimple" );
	auto textureAndColorEffect = GetAsset< Effect>( "TextureAndColor" );
	auto fourColorsEffect = GetAsset< Effect >( "FourColors" );
	auto colorAmbientEffect = GetAsset< Effect >( "ColorAmbient" );
	auto textureAmbientEffect = GetAsset< Effect >( "TextureAmbient" );
	auto colorInstancedAmbientEffect = GetAsset< Effect >( "ColorAmbientInstanced" );

	// Add a camera...
	Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->AddComponent( object::component::IObjectComponent::ptr( new object::component::CameraComponent() ) );
	auto * cameraComponent = unify::polymorphic_downcast< object::component::CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, GetOS()->GetRenderer(0)->GetDisplay().GetSize().AspectRatioWH(), 1.0f, 1000.0f ) );

	// Geo1
	sg::CubeParameters cubeParameters;
	cubeParameters.SetEffect( colorInstancedAmbientEffect );
    cubeParameters.SetSize( unify::Size3< float >( 1, 1, 1 ) );
	cubeParameters.SetDiffuse( unify::ColorBlue() );
	Geometry::ptr geo1( sg::CreateShape( GetOS()->GetRenderer(0), cubeParameters ) );

	// Geo2
	sg::SphereParameters sphereParameters;
	sphereParameters.SetEffect( colorInstancedAmbientEffect );
	sphereParameters.SetRadius( 1 );
	sphereParameters.SetDiffuse( unify::ColorRed() );
	Geometry::ptr geo2( sg::CreateShape( GetOS()->GetRenderer( 0 ), sphereParameters ) );

	// Geo3
	sg::ConeParameters coneParameters;
	coneParameters.SetEffect( colorInstancedAmbientEffect );
	coneParameters.SetHeight( 1 );
	coneParameters.SetRadius( 1 );
	coneParameters.SetDiffuse( unify::ColorGreen() );
	Geometry::ptr geo3( sg::CreateShape( GetOS()->GetRenderer( 0 ), coneParameters ) );

	size_t depth = 3;
	size_t columns = 3;
	size_t rows = 3;
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

	Effect::ptr font2 = GetManager< Effect>()->Add( "font2", unify::Path( "font2.effect" ) );
	canvas->GetLayer()->AddElement( canvas::IElement::ptr( new canvas::FPS( GetGame(), font2 ) ) );
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	// Use of camera controls to simplify camera movement...
	Object * camera = FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) );	
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );

/*
	static const float time_limit = 5.0f;
	static float time = 0.0f;
	static std::vector< Object* > moving;
	static std::vector< unify::Matrix > originalPositions;
	static std::vector< unify::V3< float > > originPoint;
	time -= params.GetDelta();
	if( time <= 0.0f )
	{
		for( size_t i = 0; i < moving.size(); i++ )
		{
			auto object = moving[i];
			object->GetFrame().SetMatrix( originalPositions[ i ] );
		}

		moving.clear();
		originalPositions.clear();
		originPoint.clear();

		for( size_t i = 0; i < 3000; i++ )
		{
			size_t count = GetObjectAllocator()->Count();
			auto chunks = ( count / RAND_MAX ) + 1;
			size_t offset = (rand() % chunks) * RAND_MAX;

			size_t index = offset + RAND_MAX > count ? rand() % ( count - offset ) : rand();
			index += offset;
			auto object = GetObjectAllocator()->GetObject( index );
			moving.push_back( object );
			originalPositions.push_back( object->GetFrame().GetMatrix() );

			int axis = rand() % 6;
			float inv = -1.0f + (float)( (int)axis % 2 ) * 2.0f;
			axis = axis / 2;
			float size = 230.0f;
			originPoint.push_back( { ( axis == 0 ) * inv * size, ( axis == 1 ) * inv * size, ( axis == 2 ) * inv * size} );
		}
		time = time_limit;
	}

	for( size_t i = 0; i < moving.size(); i++ )
	{
		auto object = moving[i];

		unify::Angle angle( unify::AngleInDegrees( ( 360 / time_limit ) * params.GetDelta() ) );
		angle *= (rand() % 2 ) ? 1 : -1;
		object->GetFrame().Orbit( originPoint[i], unify::Quaternion( unify::V3< float >::V3Normalized( originPoint[i] ), angle ) );
	}
	*/

	float collectGeometriesTimeS = GetOS()->Debug()->DebugGetTimeStamp( "CollectGeometries" );
	float renderTimeS = GetOS()->Debug()->DebugGetTimeStamp( "Render" );
	int x( 0 ); x;
}
