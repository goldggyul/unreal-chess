// Copyright Epic Games, Inc. All Rights Reserved.


#include "ChessGameModeBase.h"
#include "ChessPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "ChessPlayerController.h"
#include "Sound/SoundCue.h"

AChessGameModeBase::AChessGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> MenuPawn(TEXT("Blueprint'/Game/Blueprints/BP_MenuPawn.BP_MenuPawn_C'"));
	DefaultPawnClass = MenuPawn.Class;
	PlayerControllerClass = AChessPlayerController::StaticClass();
}

void AChessGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SetFolderPath("/GameInfo");
}