// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <me/render/RenderMethod.h>
#include <me/render/VertexUtil.h>

class MainScene : public me::scene::Scene
{
	struct {
		me::render::Effect::ptr effect;
		me::render::IConstantBuffer::ptr vertexCB;
		me::render::IConstantBuffer::ptr pixelCB;
	} m_borg, m_4;
	
	me::render::IVertexBuffer::ptr m_vertexBuffer;
	unify::Quaternion m_q;

public:
	MainScene( me::game::Game * gameInstance );

public: // me::game::IGame
	void OnStart() override;
	void OnUpdate( const me::UpdateParams & params ) override;
	void OnRender( me::scene::RenderGirl renderGirl ) override;
};
