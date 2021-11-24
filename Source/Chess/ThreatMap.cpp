// Fill out your copyright notice in the Description page of Project Settings.


#include "ThreatMap.h"
#include "Piece.h"
#include "ThreatBox.h"
#include "Kismet/GameplayStatics.h"


UThreatMap::UThreatMap()
{
}

void UThreatMap::SetPlayerColor(EPieceColor Color)
{
	PlayerColor = Color;
	InitPieces();
}

void UThreatMap::UpdateMap()
{
	DestroyMap();
	UpdatePieces();

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

void UThreatMap::ShowMap()
{
	FRotator BoxRotation = UChessUtil::GetPlayerDirection(PlayerColor);

	for (auto& SquareState : SquareStates)
	{
		FVector& Location = SquareState.Key;
		Location.Z = 1;
		FPieceCount& PieceCount = SquareState.Value;

		if (PieceCount.BlackCount == 0 && PieceCount.WhiteCount == 0)
		{
			continue;
		}

		AThreatBox* ThreatBox = GetWorld()->SpawnActor<AThreatBox>(Location, BoxRotation);
		ThreatBox->SetFolderPath(TEXT("/ThreatBox"));
		if (PlayerColor == EPieceColor::White)
		{
			ThreatBox->SetShapeAndColor(PieceCount.WhiteCount, PieceCount.BlackCount);
		}
		else
		{
			ThreatBox->SetShapeAndColor(PieceCount.BlackCount, PieceCount.WhiteCount);
		}
		ThreatBoxes.Add(ThreatBox);
		
	}
}

void UThreatMap::DestroyMap()
{
	for (auto& ThreatBox : ThreatBoxes)
	{
		if (IsValid(ThreatBox))
		{
			ThreatBox->Destroy();
		}
	}
	ThreatBoxes.Empty();

	SquareStates.Empty();
}

void UThreatMap::InitPieces()
{
	PlayerPieces.Empty();
	EnemyPieces.Empty();

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

void UThreatMap::UpdatePieces()
{
	for (auto& Piece : PlayerPieces)
	{
		if (!IsValid(Piece))
		{
			PlayerPieces.Remove(Piece);
		}
	}
	for (auto& Piece : EnemyPieces)
	{
		if (!IsValid(Piece))
		{
			EnemyPieces.Remove(Piece);
		}
	}
}
