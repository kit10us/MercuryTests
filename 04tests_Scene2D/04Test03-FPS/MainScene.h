// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <me/render/Effect.h>
#include <me/render/IVertexBuffer.h>
#include <me/render/BufferSet.h>
#include <unify/Quaternion.h>

class MainScene : public me::scene::Scene
{
	me::render::BufferSet::ptr m_set;
	/*me::render::Effect::ptr m_effect;
	me::render::IConstantBuffer::ptr
	me::render::IVertexBuffer::ptr m_vertexBuffer;*/

	unify::Quaternion m_q;
public:
	MainScene( me::game::Game * gameInstance );

	void OnStart() override;
	void OnUpdate( const me::UpdateParams & params ) override;
	void OnRender( me::scene::RenderGirl renderGirl ) override;
};
