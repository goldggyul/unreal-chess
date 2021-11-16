// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPlayerController.h"
#include "ChessPlayer.h"

AChessPlayerController::AChessPlayerController()
{

}

void AChessPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CurPlayer = GetWorld()->SpawnActor<AChessPlayer>(FVector(1200.f, 2100.f, 2200.f), FRotator(-70.f, -90.f, 0.f));
	PrevPlayer = GetWorld()->SpawnActor<AChessPlayer>(FVector(1200.f, 300.f, 2200.f), FRotator(-70.f, 90.f, 0.f));

	CurPlayer->SetActorLabel(FString(TEXT("WhitePlayer")));
	PrevPlayer->SetActorLabel(FString(TEXT("BlackPlayer")));

	CurPlayer->SetFolderPath("/Player");
	PrevPlayer->SetFolderPath("/Player");
	SetFolderPath("/Player");

	CurPlayer->SetBoxStart(FVector(1350.f,1950.f,10.f));
	PrevPlayer->SetBoxStart(FVector(1050.f,450.f,10.f));

	CurPlayer->SpawnCurBox();

	OnPossess(CurPlayer);
}

void AChessPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

}

void AChessPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UE_LOG(LogTemp, Warning, TEXT("POSSESS!"));
	
	if (IsValid(CurPlayer))
	{
		CurPlayer->SpawnCurBox();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cur Player is not valid!"));
	}
}
