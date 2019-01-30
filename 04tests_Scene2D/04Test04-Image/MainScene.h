// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <me/render/Effect.h>
#include <me/render/IVertexBuffer.h>

class MainScene : public me::scene::Scene
{
	me::render::Effect::ptr effect;
	me::render::IVertexBuffer::ptr vertexBuffer;
	unify::Quaternion q;

public:
	MainScene( me::game::Game * gameInstance );

	void OnStart() override;
	void OnUpdate( const me::UpdateParams & params ) override;
};
