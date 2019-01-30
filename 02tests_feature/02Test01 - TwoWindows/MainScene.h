// Copyright (c) 2002 - 2011, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <me/RenderMethod.h>
#include <me/VertexUtil.h>

class MainScene : public me::scene::Scene
{
public:
	MainScene( me::Game * game );

	void OnStart() override;
	void OnUpdate( me::UpdateParams params ) override;
	void OnRender( me::RenderParams params ) override;
};
