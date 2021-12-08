// Fill out your copyright notice in the Description page of Project Settings.


#include "ThreatMap.h"
#include "Piece.h"
#include "ThreatBox.h"
#include "Kismet/GameplayStatics.h"

void UThreatMap::SetPlayerColor(EPieceColor Color)
{
	PlayerColor = Color;
	InitPieces();
}

void UThreatMap::UpdateMap()
{
	bIsCheckmate = false;
	bIsStalemate = false;
	bIsCheck = false;

	if (PlayerColor != EPieceColor::None)
	{
		UpdatePiecesMove();
		UpdateSquareStates();
		UpdateGameResult();
	}
}

void UThreatMap::ShowMap()
{
	if (SquareStates.Num() == 0) return;
	if (ThreatBoxes.Num() != 0)
	{
		DestroyThreatBoxes();
	}

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

void UThreatMap::DestroyThreatBoxes()
{
	for (auto It = ThreatBoxes.CreateConstIterator(); It; ++It)
	{
		if (IsValid(*It))
		{
			(*It)->Destroy();
		}
	}
	ThreatBoxes.Empty();
}

void UThreatMap::InitPieces()
{
	PlayerPieces.Empty();
	EnemyPieces.Empty();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APiece::StaticClass(), OUT Actors);
	for (auto& Actor : Actors)
	{
		APiece* Piece = Cast<APiece>(Actor);
		if (Piece->GetPieceColor() == PlayerColor)
		{
			PlayerPieces.Add(Piece);

			if (Piece->GetPieceType() == EPieceType::King)
			{
				PlayerKing = Piece;	
			}
		}
		else
		{
			EnemyPieces.Add(Piece);
		}	
	}
}

void UThreatMap::UpdatePiecesMove()
{
	for (auto& PlayerPiece : PlayerPieces)
	{
		if (!IsValid(PlayerPiece)) continue;
		PlayerPiece->UpdateBasicMoves(); // 기본 움직임
		PlayerPiece->RemoveMoveKingCheckedByEnemies(PlayerKing, EnemyPieces); // 체크 되는 상황의 움직임 제거
	}

	for (auto& EnemyPiece : EnemyPieces)
	{
		if (!IsValid(EnemyPiece)) continue;
		EnemyPiece->UpdateBasicMoves(); // 기본 움직임
	}
}

void UThreatMap::UpdateSquareStates()
{
	SquareStates.Empty();

	for (auto& PlayerPiece : PlayerPieces)
	{
		if (!IsValid(PlayerPiece)) continue;

		for (auto& Move : PlayerPiece->GetMoves())
		{
			FPieceCount& PieceCount = SquareStates.FindOrAdd(Move);
			SquareStates[Move].AddCount(PlayerPiece->GetPieceColor());
		}
	}

	for (auto& EnemyPiece : EnemyPieces)
	{
		if (!IsValid(EnemyPiece)) continue;

		for (auto& Move : EnemyPiece->GetMoves())
		{
			FPieceCount& PieceCount = SquareStates.FindOrAdd(Move);
			SquareStates[Move].AddCount(EnemyPiece->GetPieceColor());
		}
	}
}

void UThreatMap::UpdateGameResult()
{
	bool bCanMove = false;
	for (auto& PlayerPiece : PlayerPieces)
	{
		if (!IsValid(PlayerPiece)) continue;

		if (PlayerPiece->GetMoves().Num() > 0)
		{
			bCanMove = true;
			break;
		}
	}

	FPieceCount* KingSquareCount = SquareStates.Find(PlayerKing->GetActorLocation());
	if (KingSquareCount != nullptr)
	{
		if (KingSquareCount->GetEnemyCount(PlayerColor) > 0)
		{
			bIsCheck = true;
		}
	}

	if (!bCanMove)
	{
		if (bIsCheck)
		{
			bIsCheckmate = true;
		}
		else
		{
			bIsStalemate = true;
		}
	}
}
