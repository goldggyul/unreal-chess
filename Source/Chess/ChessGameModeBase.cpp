// Copyright Epic Games, Inc. All Rights Reserved.


#include "ChessGameModeBase.h"
#include "ChessGameStateBase.h"
#include "ChessPlayer.h"
#include "Board.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "ChessPlayerController.h"

AChessGameModeBase::AChessGameModeBase()
{
	DefaultPawnClass = AChessPlayer::StaticClass();
	GameStateClass = AChessGameStateBase::StaticClass();
	PlayerControllerClass = AChessPlayerController::StaticClass();

}

void AChessGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SetFolderPath("/GameInfo");
	GameState->SetFolderPath("/GameInfo");
	
	//GetWorld()->SpawnActor<ABoard>(FVector::ZeroVector, FRotator::ZeroRotator);

	CreateChessWidget();
}

void AChessGameModeBase::CreateChessWidget()
{

}

//AActor* AChessGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
//{
//	TArray<AActor*> Starts;
//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Starts);
//	UE_LOG(LogTemp, Warning, TEXT("SELECTED %d"), Starts.Num());
//
//	APlayerStart* CurStart = Cast<APlayerStart>(Starts[0]);
//	Starts.RemoveAt(0);
//	return CurStart;
//}

