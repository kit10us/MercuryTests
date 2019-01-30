// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>
#include <me/scene/SceneManager.h>
#include <me/object/Object.h>
#include <me/render/Mesh.h>
#include <sg/ShapeCreators.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/object/component/BBoxRendererComponent.h>
#include <me/scene/component/AutoBBoxSceneComponent.h>
#include <me/object/component/CameraComponent.h>

#include <sculpter/Shape.h>
#include <sculpter/Sheet.h>
#include <sculpter/spline/ISpline.h>

#include <algorithm>
#include <math.h>

using namespace me;
using namespace render;

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

	// Create our textured effect with an image.
	auto textureImageEffect = textureEffect->Duplicate();
	textureImageEffect->SetTexture( 0, GetAsset< ITexture >( "4" ) );

	//scene->AddComponent( scene::SceneComponent::ptr( new scene::AutoBBoxSceneComponent( GetOS(), colorEffect ) ) );

	// Add a camera...
	object::Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->AddComponent( object::component::IObjectComponent::ptr( new object::component::CameraComponent() ) );
	auto * cameraComponent = unify::polymorphic_downcast< object::component::CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800/600, 1, 1000 ) );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	// From dynamically generated geometry (shape creator)...
	{
		sg::CubeParameters cubeParameters;
		cubeParameters.SetEffect( colorEffect );
		cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
		cubeParameters.SetDiffuseFaces( unify::ColorRed(), unify::ColorGreen(), unify::ColorBlue(), unify::ColorYellow(), unify::ColorCyan(), unify::ColorMagenta() );
		Geometry::ptr meshProg( sg::CreateShape( GetOS()->GetRenderer( 0 ), cubeParameters ) );
		PrimitiveList & plProg = ( (Mesh*)meshProg.get() )->GetPrimitiveList();
		auto object = GetObjectAllocator()->NewObject( "cubeDyna" );
		AddGeometryComponent( object, meshProg );
		object->GetFrame().SetPosition( unify::V3< float >( 0 - 5.0f, 0, 0 ) );
		object->AddComponent( object::component::IObjectComponent::ptr( new object::component::BBoxRendererComponent( GetOS(), colorEffect ) ) );
	}


	// Sculpter...
	{
		using namespace sculpter;

		class Line : public spline::ISpline
		{
		public:
			Line( float length )
				: m_length{ length }
			{
			}

			Point GetPoint( float delta ) override
			{
				// Clamp delta to within limits (it's either this or assert, but in case we do over or inder our limits a smudge, like with rounding errors)...
				delta = std::min< float >( delta, 1.0f );
				delta = std::max< float >( delta, 0.0f );

				using namespace unify;
				Matrix o = MatrixIdentity();
				V3< float > translation{ o.GetLeft() * ( m_length * delta ) };
				o.Translate( translation );
				return o;
			}

		private:
			float m_length;
		};

		class Sine : public spline::ISpline
		{
		public:
			Sine( float length, float height, unify::Angle a, unify::Angle b )
				: m_length{ length }
				, m_height{ height }
				, m_angleA{ a }
				, m_angleB{ b }
			{
			}

			Point GetPoint( float delta ) override
			{
				// Clamp delta to within limits (it's either this or assert, but in case we do over or inder our limits a smudge, like with rounding errors)...
				delta = std::min< float >( delta, 1.0f );
				delta = std::max< float >( delta, 0.0f );

				using namespace unify;
				Matrix o = MatrixIdentity();
				float PI2 = AnglePI2().ToRadians();
				//y: V3< float > translation{ delta * m_length, sinf( delta * PI2 ) * m_height, 0.0f };
				//z: V3< float > translation{ delta * m_length, 0.0f, sinf( delta * PI2 ) * m_height };
				
				float rad = m_angleA.ToRadians() + delta * m_angleB.ToRadians();
				V3< float > translation{ delta * m_length, sinf( rad ) * m_height, 0.0f };

				o.Translate( translation );
				return o;
			}

		private:
			float m_length;
			float m_height;
			unify::Angle m_angleA;
			unify::Angle m_angleB;
		};


		Sheet::ptr sheet(
			new Sheet(
				unify::MatrixIdentity(),
				{
					spline::ISpline::ptr{ new Line( 3 ) },
					//spline::ISpline::ptr{ new Sine( 3, 1, unify::AngleInDegrees( 0 ), unify::AngleInDegrees( 360 ) ) }
					spline::ISpline::ptr{ new Sine( 3, 3, unify::AngleInDegrees( 0 ), unify::AngleInDegrees( 180 ) ) }
				}, 
				{ 30, 30 } 
			) 
		);

		Mesh * mesh = new Mesh( GetOS()->GetRenderer( 0 ) );
		PrimitiveList & pl = mesh->GetPrimitiveList();

		sheet->Create( pl, { 2.0f, 2.0f }, colorEffect );

		auto object = GetObjectAllocator()->NewObject( "sculpterCube" );
		AddGeometryComponent( object, Geometry::ptr{ mesh } );
		object->GetFrame().SetPosition( unify::V3< float >( 0 + 5.0f, 0, 0 ) );
		//object->AddComponent( IObjectComponent::ptr( new object::BBoxRendererComponent( GetOS(), colorEffect ) ) );


		auto  shape( sculpter::Shape::ptr( new sculpter::Shape() ) );
	}
	
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	// Use of camera controls to simplify camera movement...
	object::Object * camera = FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) );
	//camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
	
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}
