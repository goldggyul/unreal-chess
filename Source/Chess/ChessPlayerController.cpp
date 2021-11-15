// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPlayerController.h"
#include "ChessPlayer.h"

AChessPlayerController::AChessPlayerController()
{

}

void AChessPlayerController::BeginPlay()
{
	CurPlayer = GetWorld()->SpawnActor<AChessPlayer>(FVector(1200.f, 2100.f, 2200.f), FRotator(-70.f, -90.f, 0.f));
	PrevPlayer = GetWorld()->SpawnActor<AChessPlayer>(FVector(1200.f, 300.f, 2200.f), FRotator(-70.f, 90.f, 0.f));
	Possess(CurPlayer);
}
