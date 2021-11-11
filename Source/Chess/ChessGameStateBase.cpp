// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameStateBase.h"
#include "Board.h"
#include "PawnPiece.h"

void AChessGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnChessSet();
}

void AChessGameStateBase::SpawnChessSet()
{
	SpawnBoard();
	SpawnPieces();
}

void AChessGameStateBase::SpawnBoard()
{
	Board = GetWorld()->SpawnActor<ABoard>(ABoard::StaticClass());
}

void AChessGameStateBase::SpawnPieces()
{
	FVector BoardLocation = Board->GetActorLocation();
	const uint8 SquareSize = Board->GetSquareSize();
	// TODO: 기준점 옮기기
	// BoardLocation = FVector(BoardLocation.X - ??, BoardLocation.Y - ??);
	//for (int i = 0; i < 64; i++)
	//{

	//}

	//// TEST
	//APawnPiece* PawnPiece = GetWorld()->SpawnActor<APawnPiece>(APawnPiece::StaticClass());
	//PawnPiece->SetColor(EPieceColor::Black);
}
