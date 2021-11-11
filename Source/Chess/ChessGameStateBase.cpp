// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameStateBase.h"
#include "Board.h"


void AChessGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	Board = GetWorld()->SpawnActor<ABoard>(ABoard::StaticClass());

}
