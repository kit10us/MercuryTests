// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MEWinMain.h>
#include <me/game/Game.h>

using namespace me;

class MyGame : public game::Game
{
public: 
	MyGame() : Game( unify::Path( "setup/TwoWindows.me_setup" ) ) {}
} myGame;

RegisterGame( myGame );
