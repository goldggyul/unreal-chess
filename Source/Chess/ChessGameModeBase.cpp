// Copyright Epic Games, Inc. All Rights Reserved.


#include "ChessGameModeBase.h"
#include "ChessGameStateBase.h"
//#include "ChessPlayer.h"

AChessGameModeBase::AChessGameModeBase()
{
	//DefaultPawnClass = AChessPlayer::StaticClass();
	GameStateClass = AChessGameStateBase::StaticClass();
}
