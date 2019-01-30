// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <me/render/Mesh.h>
#include <me/frameanimation/FrameAnimation.h>

class MainScene : public me::scene::Scene
{
	std::shared_ptr< me::render::Mesh > m_mesh;
	unify::Quaternion m_q;
	me::frameanimation::Animation::ptr m_animation;
public:
	MainScene( me::game::Game * gameInstance );

public: // me::game::IGame
	void OnStart() override;
	void OnUpdate( const me::UpdateParams & params ) override;
	void OnRender( me::scene::RenderGirl renerGirl ) override;

public:
	void SyncRenderData();
};
