// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>
#include <me/render/Mesh.h>
#include <me/render/Terra.h>
#include <me/factory/EffectFactories.h>
#include <me/object/component/GeometryComponent.h>				
#include <me/object/component/CameraComponent.h>

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
	AddResources( unify::Path( "resources/TestTextures.me_res" ) );

	Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->AddComponent( component::IObjectComponent::ptr( new component::CameraComponent() ) );
	auto * cameraComponent = unify::polymorphic_downcast< component::CameraComponent * >( camera->GetComponent( "Camera" ).get() );
	cameraComponent->SetProjection(	unify::MatrixPerspectiveFovLH( 3.1415926535f / 4.0f, GetOS()->GetRenderer(0)->GetViewport().GetSize().AspectRatioWH(), 1, 1000 ) );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 12, -12 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	// Add textures we will need for our effects, and terrain generation/modification.
	// Load an effect, then modify it to fit our needs.
	Effect::ptr landEffect = GetAsset< Effect >( "TextureSimple" )->Duplicate();
	{
		TextureParameters parameters;
		parameters.source = unify::Path( "4.bmp" );
		parameters.lockAccess = { unify::DataLockAccess::Readonly, unify::DataLockAccess::None };
		parameters.min = Filtering::Point;
		parameters.mag = Filtering::Point;
		parameters.mip = Filtering::Point;
		parameters.usage = BufferUsage::Default;
		auto lockable = GetManager< ITexture >()->Add( "Lockable4", unify::Path( "4.bmp" ), unify::Path(), &parameters );
		auto t = lockable->GetParameters()->lockAccess;
		landEffect->SetTexture( 0, lockable );
	}


	Terra::Parameters parameters;
	Terra * terra;
	parameters.Reset();
	parameters.SetSize( unify::Size< float >( 10, 7 ) );
	parameters.SetEffect( landEffect ); // Note the use of FindEffect to simplify interaction with the Effect Manager.
	parameters.SetPoints( unify::RowColumn< unsigned int >( 200, 140 ) );
	parameters.SetConstant( 0.0f );
	parameters.SetTexArea( unify::TexAreaFull() );

	//parameters.SetHeightMap( Terra::TextureOpMap( GetManager< ITexture >()->Find( "land" ), unify::ColorUnit::ColorUnitARGB( 0.0f, 0.7f, 0.25f, -1.0f ) ) );	
	parameters.SetHeightMap( Terra::TextureOpMap(landEffect->GetTexture( 0 ), unify::ColorUnitARGB( 0.0f, 3.0f, 2.0f, 1.0f ) ) );

	//parameters.SetDiffuse( unify::ColorUnit::ColorUnitRed() );
	//parameters.SetDiffuses( unify::ColorUnit::ColorUnitBlack(), unify::ColorUnit::ColorUnitRed(), unify::ColorUnit::ColorUnitBlue(), unify::ColorUnit::ColorUnitWhite() );

	terra = new Terra( GetOS()->GetRenderer( 0 ), parameters );
	auto land = GetObjectAllocator()->NewObject( "land" );
	AddGeometryComponent( land, Geometry::ptr( terra ) );
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	using namespace object;

	Object * camera = FindObject( "camera" );
	
	//camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}
