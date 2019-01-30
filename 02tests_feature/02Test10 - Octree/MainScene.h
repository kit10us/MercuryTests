// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <me/canvas/TextElement.h>

class MainScene : public me::scene::Scene
{
	me::object::Object * m_camera;
	me::canvas::TextElement * m_text;

public:
	MainScene( me::game::Game * gameInstance );

	void OnStart() override;
	void OnUpdate( const me::UpdateParams & params ) override;
};
