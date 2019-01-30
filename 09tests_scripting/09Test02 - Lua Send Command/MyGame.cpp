// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

///<summary>
/// Load lua extension.
///</summary>

#include <MEWinMain.h>
#include <me/game/Game.h>

using namespace me;

class MyGame : public game::Game
{
public:
	MyGame() : Game( unify::Path( "LuaGo_SendCommand.xml" ) ) {}
} mygame;

RegisterGame(mygame);
