// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>
#include <me/render/VertexUtil.h>

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

	auto programmaticTextureEffect( textureEffect->Duplicate() );

	// Programmatic texture.
	{
		TextureParameters parameters;
		parameters.lockAccess = { unify::DataLockAccess::Writeonly, unify::DataLockAccess::Readonly };
		parameters.size = unify::Size< unsigned int >( 512, 512 );

		auto programmaticTexture = GetOS()->GetRenderer( 0 )->ProduceT( parameters );

		TextureLock lock;
		programmaticTexture->LockRect( 0, lock, 0, unify::DataLockAccess::Writeonly );

		unify::ColorUnit * colorBuffer = reinterpret_cast< unify::ColorUnit * >(lock.pBits);
		unify::ColorUnit color{};
		for( unsigned int y = 0; y < lock.uHeight; y++ )
		{
			for( unsigned int x = 0; x < lock.uWidth; x++ )
			{
				color.component.r = (float)x / (float)lock.uWidth;
				color.component.g = 1.0f - (float)y / (float)lock.uHeight;
				color.component.b = (float)x / (float)lock.uWidth * (float)y / (float)lock.uHeight;
				color.component.a = 1.0f;
				colorBuffer[x + y * lock.uWidth] = color;
			}
		}

		programmaticTexture->UnlockRect( 0 );

		programmaticTextureEffect->SetTexture( 0, programmaticTexture );
	}

	// Cube with colors transitioning between corners.
	m_cubes[0].reset( new BufferSet( GetOS()->GetRenderer( 0 ), colorEffect ) );

	// Cube with a programmatic texture.
	m_cubes[1].reset( new BufferSet( GetOS()->GetRenderer( 0 ), programmaticTextureEffect ) );

	// Cube with a texture.
	m_cubes[2].reset( new BufferSet( GetOS()->GetRenderer( 0 ), textureEffect ) );

	// Cube with a texture and color.
	m_cubes[3].reset( new BufferSet( GetOS()->GetRenderer( 0 ), textureAndColorEffect ) );

	// Cube with a texture,color, and transparency.
	m_cubes[4].reset( new BufferSet( GetOS()->GetRenderer( 0 ), textureAndColorEffect ) );

	float xscalar = 10.0f;
	float yscalar = 10.0f;
	float zscalar = 10.0f;

	for( size_t i = 0; i < 5; i++ )
	{
		using namespace unify;
		auto & cube = *m_cubes[ i ];

		auto vd = m_cubes[ i ]->GetEffect()->GetVertexShader()->GetVertexDeclaration();

		unsigned short stream = 0;
		VertexElement positionE = CommonVertexElement::Position( stream );
		VertexElement colorE = CommonVertexElement::Diffuse( stream );
		VertexElement textureE = CommonVertexElement::TexCoords( stream );

		size_t faceCount = 6;
		size_t verticesPerFace = 6;
		size_t vertexCount = faceCount * verticesPerFace;
		std::shared_ptr< char * > vertices( new char *[ vd->GetSizeInBytes( 0 ) * vertexCount] );

		unify::DataLock lock( vertices.get(), vd->GetSizeInBytes( 0 ), vertexCount, unify::DataLockAccess::ReadWrite, 0 );

		size_t v = 0;
		// Z-Near...
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { -xscalar, yscalar, zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { xscalar, yscalar, zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { -xscalar, -yscalar, zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { xscalar, yscalar, zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { xscalar, -yscalar, zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { -xscalar, -yscalar, zscalar } );

		// Z-Near...
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { xscalar, yscalar, -zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { -xscalar, yscalar, -zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { xscalar, -yscalar, -zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { -xscalar, yscalar, -zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { -xscalar, -yscalar, -zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { xscalar, -yscalar, -zscalar } );

		// X-Min...
		WriteVertex< V3< float > >( *vd, lock, v++, positionE,{ -xscalar, yscalar, zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE,{ -xscalar, yscalar, -zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE,{ -xscalar, -yscalar, zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE,{ -xscalar, yscalar, -zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE,{ -xscalar, -yscalar, -zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE,{ -xscalar, -yscalar, zscalar } );

		// X-Max...
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { xscalar, yscalar, -zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { xscalar, yscalar, zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { xscalar, -yscalar, -zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { xscalar, yscalar, zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { xscalar, -yscalar, zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { xscalar, -yscalar, -zscalar } );

		// Y-Min...
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { -xscalar, yscalar, zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { xscalar, yscalar, zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { -xscalar, yscalar, -zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { xscalar, yscalar, zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { xscalar, yscalar, -zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { -xscalar, yscalar, -zscalar } );

		// Y-Max...
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { -xscalar, -yscalar, -zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { xscalar, -yscalar, -zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { -xscalar, -yscalar, zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { xscalar, -yscalar, -zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { xscalar, -yscalar, zscalar } );
		WriteVertex< V3< float > >( *vd, lock, v++, positionE, { -xscalar, -yscalar, zscalar } );

		for( int face = 0; face < 6; face++ )
		{
			WriteVertex< Color >( *vd, lock, face * 6 + 0, colorE, ColorRed() );
			WriteVertex< Color >( *vd, lock, face * 6 + 1, colorE, ColorGreen() );
			WriteVertex< Color >( *vd, lock, face * 6 + 2, colorE, ColorBlue() );
			WriteVertex< Color >( *vd, lock, face * 6 + 3, colorE, ColorGreen() );
			WriteVertex< Color >( *vd, lock, face * 6 + 4, colorE, ColorWhite() );
			WriteVertex< Color >( *vd, lock, face * 6 + 5, colorE, ColorBlue() );

			WriteVertex< TexCoords >( *vd, lock, face * 6 + 0, textureE, { 0.0f, 0.0f } );
			WriteVertex< TexCoords >( *vd, lock, face * 6 + 1, textureE, { 1.0f, 0.0f } );
			WriteVertex< TexCoords >( *vd, lock, face * 6 + 2, textureE, { 0.0f, 1.0f } );
			WriteVertex< TexCoords >( *vd, lock, face * 6 + 3, textureE, { 1.0f, 0.0f } );
			WriteVertex< TexCoords >( *vd, lock, face * 6 + 4, textureE, { 1.0f, 1.0f } );
			WriteVertex< TexCoords >( *vd, lock, face * 6 + 5, textureE, { 0.0f, 1.0f } );
		}

		cube.AddVertexBuffer( { vd, { { vertexCount, vertices.get() } }, BufferUsage::Default } );
		cube.AddMethod( RenderMethod( RenderMethod::CreateTriangleList( 0, 12 ) ) );
	}
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	static unify::Angle rotation( unify::AngleInRadians( 0.0f ) );
	static int axisIndex = 0;

	const float width = params.renderer->GetViewport().GetSize().width;
	const float height = params.renderer->GetViewport().GetSize().height;

	rotation += unify::AngleInDegrees( params.GetDelta().GetSeconds() * 360.0f );
	if( rotation.Normalize() != 0 )
	{
		++axisIndex;
		if( axisIndex >= 3 )
		{
			axisIndex = 0;
		}
	}

	unify::V3< float > eye( 0.0f, 0.0f, -60.0f );
	unify::V3< float > at( 0.0f, 0.0f, 0.0f );
	unify::V3< float > up( 0.0f, 1.0f, 0.0f );

	unify::V3< float > axis( (axisIndex == 0) ? 1.0f : 0.0f, (axisIndex == 1) ? 1.0f : 0.0f, (axisIndex == 2) ? 1.0f : 0.0f );

	unify::Matrix worldMatrixA = unify::MatrixIdentity();
	worldMatrixA *= unify::MatrixRotationAboutAxis( axis, rotation );

	m_q = unify::Quaternion( axis, rotation );

	params.renderInfo.SetViewMatrix( unify::MatrixLookAtLH( eye, at, up ) );
	params.renderInfo.SetProjectionMatrix( unify::MatrixPerspectiveFovLH( 3.1415926535f / 4.0f, width / height, 0.01f, 100.0f ) );
}

void MainScene::OnRender( RenderGirl renderGirl )
{
	using namespace unify;

	float f = 14.0f;
	Matrix instances[] = 
	{
		Matrix( m_q ) * MatrixTranslate( { -f,  f, f } ),
		Matrix( m_q ) * MatrixTranslate( {  f,  f, f } ),
		Matrix( m_q ) * MatrixTranslate( { -f, -f, f } ),
		Matrix( m_q ) * MatrixTranslate( {  f, -f, f } ),
		Matrix( m_q ) * MatrixTranslate( {  0,  0, 0 } )
	};

	for( size_t i = 0; i < 5; i++ )
	{
		m_cubes[i]->Render( *renderGirl.GetParams(), render::MatrixFeed( render::MatrixFood_Matrices{ &instances[i], 1 }, 1 ) );
	}
}
