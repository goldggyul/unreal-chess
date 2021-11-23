// Fill out your copyright notice in the Description page of Project Settings.


#include "ThreatMap.h"
#include "Piece.h"
#include "Kismet/GameplayStatics.h"


UThreatMap::UThreatMap()
{
	//static ConstructorHelpers::FClassFinder<APaperSpriteActor> PS(TEXT("Blueprint'/Game/Blueprints/FocusBox/BS_ThreatBox.BS_ThreatBox'"));
	//if (PS.Succeeded())
	//{
	//	ThreatBoxClass = PS.Class;
	//}
}

void UThreatMap::SetPlayerColor(EPieceColor Color)
{
	PlayerColor = Color;
}

void UThreatMap::Update()
{
	AddPieces();

	for (auto& PlayerPiece : PlayerPieces)
	{
		PlayerPiece->UpdateMoves(); // 기본 움직임
		PlayerPiece->RemoveCheckMoves(); // 체크 되는 상황의 움직임 제거
	}
	for (auto& EnemyPiece : EnemyPieces)
	{
		EnemyPiece->UpdateMoves(); // 기본 움직임
	}

	// threat map 업데이트
	for (auto& Piece : PlayerPieces)
	{
		for (auto& Move : Piece->GetMoves())
		{
			FPieceCount& PieceCount = SquareStates.FindOrAdd(Move);
			SquareStates[Move].AddCount(Piece->GetPieceColor());
		}
	}
	for (auto& Piece : EnemyPieces)
	{
		for (auto& Move : Piece->GetMoves())
		{
			FPieceCount& PieceCount = SquareStates.FindOrAdd(Move);
			SquareStates[Move].AddCount(Piece->GetPieceColor());
		}
	}
}

void UThreatMap::Show()
{
	for (auto& SquareState : SquareStates)
	{
		FVector& Location = SquareState.Key;
		FPieceCount& PieceCount = SquareState.Value;

	}
}

void UThreatMap::AddPieces()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APiece::StaticClass(), Actors);
	for (auto& Actor : Actors)
	{
		APiece* Piece = Cast<APiece>(Actor);
		if (IsValid(Piece))
		{
			if (Piece->GetPieceColor() == PlayerColor)
			{
				PlayerPieces.Add(Piece);
			}
			else
			{
				EnemyPieces.Add(Piece);
			}
		}
	}
}
