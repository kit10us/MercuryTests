// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <me/render/RenderMethod.h>
#include <me/render/VertexUtil.h>

class MainScene : public me::scene::Scene
{
	me::render::BufferSet::ptr m_cubes[5];

	unify::Quaternion m_q;
public:
	MainScene( me::game::Game * gameInstance );

	void OnStart() override;
	void OnUpdate( const me::UpdateParams & params ) override;
	void OnRender( me::scene::RenderGirl renderGirl ) override;
};
