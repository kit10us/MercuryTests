// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <me/render/RenderMethod.h>
#include <me/render/VertexUtil.h>

class MainScene : public me::scene::Scene
{
	me::render::Effect::ptr m_effect;
	me::render::IConstantBuffer::ptr m_vertexCB;
	me::render::IConstantBuffer::ptr m_pixelCB;

	me::render::IVertexBuffer::ptr m_vertexBuffer;

public:
	MainScene( me::game::Game * gameInstance );

	void OnStart() override;
	void OnUpdate( const me::UpdateParams & params ) override;
	void OnRender( me::scene::RenderGirl renderGirl ) override;
};
