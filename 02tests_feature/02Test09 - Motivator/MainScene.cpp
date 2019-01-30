// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>
#include <me/render/Mesh.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/scene/component/AutoBBoxSceneComponent.h>
#include <me/object/component/BBoxRendererComponent.h>
#include <me/object/component/CameraComponent.h>
#include <sg/ShapeCreators.h>

using namespace me;
using namespace render;
using namespace scene;

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

	// Load shaders.
	IPixelShader::ptr ps = textureEffect->GetPixelShader();
	IVertexShader::ptr vs = textureEffect->GetVertexShader();
																									  
	// Add a texture.
	GetManager< ITexture >()->Add( "borgcube", unify::Path( "borgcube.bmp" ) );

	// Create an effect.
	Effect::ptr borgCubeEffect = GetManager< Effect >()->Add( "borgcube", new Effect );
	borgCubeEffect->SetVertexShader( vs );
	borgCubeEffect->SetPixelShader( ps );
	borgCubeEffect->SetTexture( 0, GetManager< ITexture >()->Find( "borgcube" ) );

	//AddComponent( scene::SceneComponent::ptr( new scene::AutoBBoxSceneComponent( GetOS(), color3DEffect ) ) );

	// Add a camera...
	object::Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->AddComponent( object::component::IObjectComponent::ptr( new object::component::CameraComponent() ) );
	auto * cameraComponent = unify::polymorphic_downcast< object::component::CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800/600, 1, 1000 ) );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	// From dynamically generated geometry (shape creator)...
	sg::CubeParameters cubeParameters;
	cubeParameters.SetEffect( colorEffect );
    cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
	cubeParameters.SetDiffuseFaces( unify::ColorRed(), unify::ColorGreen(), unify::ColorBlue(), unify::ColorYellow(), unify::ColorCyan(), unify::ColorMagenta() );
	Geometry::ptr meshProg( sg::CreateShape( GetOS()->GetRenderer(0), cubeParameters ) );
	PrimitiveList & plProg = ((Mesh*)meshProg.get())->GetPrimitiveList();
	auto progObject = GetObjectAllocator()->NewObject( "cubeDyna" );
	AddGeometryComponent( progObject, meshProg );
	progObject->GetFrame().SetPosition( unify::V3< float >( 0 - 0.0f, 0, 0 ) );
	progObject->AddComponent( object::component::IObjectComponent::ptr( new object::component::BBoxRendererComponent( GetOS(), colorEffect ) ) );

	// From an XML file...
	Geometry::ptr meshXML( GetManager< Geometry >()->Add( "cubeXML", unify::Path( "cube.xml" ) ) );
	PrimitiveList & plXML = ((Mesh*)meshXML.get())->GetPrimitiveList();
	auto xmlObject = GetObjectAllocator()->NewObject( "XMLObject" );
	xmlObject->GetFrame().SetPosition( unify::V3< float >( 0 - 2.5f, 0, 0 ) );
	{ 
		using namespace unify;
		unify::Matrix modelMatrix = MatrixIdentity();
		modelMatrix.Scale( 0.10f );
		AddGeometryComponent( xmlObject, meshXML, modelMatrix );
	}
	xmlObject->AddComponent( object::component::IObjectComponent::ptr( new object::component::BBoxRendererComponent( GetOS(), colorEffect ) ) );

	// From an ASE file...
	Geometry::ptr meshASE( GetManager< Geometry >()->Add( "swordASE", unify::Path( "ASE_SwordTextured.ASE" ) ) );
	PrimitiveList & plASE = ((Mesh*)meshASE.get())->GetPrimitiveList();
	auto aseObject = GetObjectAllocator()->NewObject( "swordASE" );
	aseObject->GetFrame().SetPosition( unify::V3< float >( 0 + 2.5f, 0, 0 ) );
	{ 
		using namespace unify;
		unify::Matrix modelMatrix = MatrixIdentity();
		modelMatrix.Scale( 0.090f );
		modelMatrix.RotateAboutAxis( unify::V3< float >( -1.0f, 0.0f, 0.0f ), unify::AngleInDegrees( 90 ) );
		modelMatrix.Translate( unify::V3< float >( 0, 1.0f, 0.0f ) );
		AddGeometryComponent( aseObject, meshASE, modelMatrix );
	}
	aseObject->AddComponent( object::component::IObjectComponent::ptr( new object::component::BBoxRendererComponent( GetOS(), colorEffect ) ) );

	Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", unify::Path( "torus.dae" ) ) );
	
	auto daeModel = GetObjectAllocator()->NewObject( "daeModel" );
	daeModel->GetFrame().SetPosition( unify::V3< float >( 0 - 5.0f, 0, 0 ) );
	daeModel->AddComponent( object::component::IObjectComponent::ptr( new object::component::BBoxRendererComponent( GetOS(), colorEffect ) ) );
	const unify::BBox< float > & bboxD = meshDAE->GetBBox();
	{ 
		using namespace unify;
		unify::Matrix modelMatrix = MatrixIdentity();
		unify::V3< float > sizeV3{ meshDAE->GetBBox().Size().width, meshDAE->GetBBox().Size().height, meshDAE->GetBBox().Size().depth };
		modelMatrix.Scale( sizeV3 / 4.0f );
		modelMatrix.RotateAboutAxis( unify::V3< float >( 1.0f, 0, 0 ), unify::AngleInDegrees( 270.0f ) );
		modelMatrix.RotateAboutAxis( unify::V3< float >( 0, 1.0f, 0 ), unify::AngleInDegrees( -90.0f ) );
		AddGeometryComponent( daeModel, meshDAE, modelMatrix );
	}
}
