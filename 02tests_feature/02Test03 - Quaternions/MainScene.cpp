// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>
#include <me/render/Mesh.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/object/component/BBoxRendererComponent.h>
#include <me/object/component/CameraComponent.h>

using namespace me;
using namespace render;

MainScene::MainScene( me::game::Game * gameInstance )
	: Scene( gameInstance, "main" )
{	
}

void MainScene::OnStart()
{
	using namespace object;
	AddResources( unify::Path( "resources/Standard.me_res" ) );
	auto colorEffect = GetAsset< Effect >( "ColorSimple" );
	auto textureEffect = GetAsset< Effect>( "TextureSimple" );
	auto textureAndColorEffect = GetAsset< Effect>( "TextureAndColor" );
	auto fourColorsEffect = GetAsset< Effect >( "FourColors" );

	// Load shaders.
	IPixelShader::ptr ps = GetAsset< IPixelShader >( "TextureSimple" );
	IVertexShader::ptr vs = GetAsset< IVertexShader >( "TextureSimple" );

	// Add a texture.
	GetManager< ITexture >()->Add( "borgcube", unify::Path( "borgcube.bmp" ) );

	// Create an effect.
	Effect::ptr borgCubeEffect = GetManager< Effect >()->Add( "borgcube", new Effect );
	borgCubeEffect->SetVertexShader( vs );
	borgCubeEffect->SetPixelShader( ps );
	borgCubeEffect->SetTexture( 0, GetManager< ITexture >()->Find( "borgcube" ) );

	// Add a camera...
	object::Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->AddComponent( component::IObjectComponent::ptr( new object::component::CameraComponent()  ) );
	component::CameraComponent * cameraComponent = unify::polymorphic_downcast< object::component::CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800/600, 1, 1000 ) );

	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -10 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	auto shapeCreator = GetManager< Geometry >()->GetFactory( "me_shape" );

	// From dynamically generated geometry (shape creator)...
	unify::Parameters parameters { 
		{ "type", "box" },
		{ "effect", colorEffect },
		{ "size3", unify::Size3< float >( 1, 1, 1 ) },
		{ "diffuses", std::vector< unify::Color > { 
			unify::ColorRed(), unify::ColorGreen(), unify::ColorBlue(), unify::ColorYellow(), unify::ColorCyan(), unify::ColorMagenta() } 
		}
	};
	Geometry::ptr meshProg( shapeCreator->Produce( parameters ) );
	PrimitiveList & plProg = ((Mesh*)meshProg.get())->GetPrimitiveList();
	auto progObject = GetObjectAllocator()->NewObject( "cubeDyna" );
	auto gc = AddGeometryComponent( progObject, meshProg );
	progObject->GetFrame().SetPosition( unify::V3< float >( 0 - 0.0f, 0, 0 ) );
	progObject->AddComponent( component::IObjectComponent::ptr( new object::component::BBoxRendererComponent( GetOS(), colorEffect ) ) );

	// From an ASE file...
	Geometry::ptr meshASE( GetManager< Geometry >()->Add( "SwordASE", unify::Path( "ASE_SwordTextured.ASE" ) ) );
	PrimitiveList & plASE = ((Mesh*)meshASE.get())->GetPrimitiveList();
	{
		auto aseObject = GetObjectAllocator()->NewObject( "sword1" );
		aseObject->GetFrame().SetPosition( unify::V3< float >( 0 + 2.5f, 0, 0 ) );
		unify::Matrix matrix = unify::MatrixIdentity();
		matrix.Scale( 0.090f );
		matrix.RotateAboutAxis( unify::V3< float >( -1.0f, 0.0f, 0.0f ), unify::AngleInDegrees( 90 ) );
		matrix.Translate( unify::V3< float >( 0, 1.0f, 0.0f ) );
		gc = AddGeometryComponent( aseObject, meshASE, matrix );
		aseObject->AddComponent( component::IObjectComponent::ptr( new object::component::BBoxRendererComponent( GetOS(), colorEffect ) ) );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInDegrees( 0 ) ) );
	}
	{
		auto aseObject = GetObjectAllocator()->CopyObject( FindObject( "sword1" ), "sword2" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = GetObjectAllocator()->CopyObject( FindObject( "sword2" ), "sword3" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = GetObjectAllocator()->CopyObject( FindObject( "sword3" ), "sword4" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInDegrees( 90 ) ) );
	}


	{
		auto aseObject = GetObjectAllocator()->CopyObject( FindObject( "sword1" ), "sword5" );
	}
	{
		auto aseObject = GetObjectAllocator()->CopyObject( FindObject( "sword5" ), "sword6" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = GetObjectAllocator()->CopyObject( FindObject( "sword6" ), "sword7" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = GetObjectAllocator()->CopyObject( FindObject( "sword7" ), "sword8" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInDegrees( 90 ) ) );
	}

	
	{
		auto aseObject = GetObjectAllocator()->CopyObject( FindObject( "sword1" ), "sword9" );
	}
	{
		auto aseObject = GetObjectAllocator()->CopyObject( FindObject( "sword9" ), "sword10" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = GetObjectAllocator()->CopyObject( FindObject( "sword10" ), "sword11" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = GetObjectAllocator()->CopyObject( FindObject( "sword11" ), "sword12" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = GetObjectAllocator()->CopyObject( FindObject( "sword12" ), "sword13" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 180 ) ) );
	}
	{
		auto aseObject = GetObjectAllocator()->CopyObject( FindObject( "sword13" ), "sword14" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 180 ) ) );
	}
	{
		auto aseObject = GetObjectAllocator()->CopyObject( FindObject( "sword14" ), "sword15" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 180 ) ) );
	}
	{
		auto aseObject = GetObjectAllocator()->CopyObject( FindObject( "sword15" ), "sword16" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 180 ) ) );
	}
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	using namespace unify;
	{
		auto sword = FindObject( "sword1" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) ) );	
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = FindObject( "sword2" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = FindObject( "sword3" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = FindObject( "sword4" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}


	{
		auto sword = FindObject( "sword5" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = FindObject( "sword6" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = FindObject( "sword7" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = FindObject( "sword8" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}


	{
		auto sword = FindObject( "sword9" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 1 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = FindObject( "sword10" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 1, 1 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = FindObject( "sword11" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 1 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = FindObject( "sword12" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 1, 0 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = FindObject( "sword13" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 1 ), unify::AngleInRadians( -params.GetDelta().GetSeconds() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = FindObject( "sword14" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 1, 1 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = FindObject( "sword15" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 1 ), unify::AngleInRadians( -params.GetDelta().GetSeconds() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = FindObject( "sword16" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 1, 0 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}


	auto camera = FindObject( "camera" );
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( params.GetDelta().GetSeconds() * - 1.0f) ) );
	camera->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
}
