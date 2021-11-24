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
	UE_LOG(LogTemp, Log, TEXT("SetPlayerColor"));

	PlayerColor = Color;
	InitPieces();
}

void UThreatMap::UpdateMap()
{
	UE_LOG(LogTemp, Log, TEXT("UpdateMap"));

	DestroyMap();
	UpdatePieces();

	UE_LOG(LogTemp, Log, TEXT("UpdatePlayerMoves"));

	bool bCanMove = false;
	for (auto& PlayerPiece : PlayerPieces)
	{
		PlayerPiece->UpdateBasicMoves(); // 기본 움직임
		PlayerPiece->RemoveMoveKingCheckedByEnemies(PlayerKing, EnemyPieces); // 체크 되는 상황의 움직임 제거

		// threat map 업데이트
		for (auto& Move : PlayerPiece->GetMoves())
		{
			if (!bCanMove)
			{
				bCanMove = true;
			}
			FPieceCount& PieceCount = SquareStates.FindOrAdd(Move);
			SquareStates[Move].AddCount(PlayerPiece->GetPieceColor());
		}
	}

	UE_LOG(LogTemp, Log, TEXT("UpdateEnemyMoves"));

	for (auto& EnemyPiece : EnemyPieces)
	{
		EnemyPiece->UpdateBasicMoves(); // 기본 움직임

		// threat map 업데이트
		for (auto& Move : EnemyPiece->GetMoves())
		{
			FPieceCount& PieceCount = SquareStates.FindOrAdd(Move);
			SquareStates[Move].AddCount(EnemyPiece->GetPieceColor());
		}
	}

	// 게임 상태 업데이트
	UE_LOG(LogTemp, Log, TEXT("UpdateGameState"));

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

void UThreatMap::ShowMap()
{
	UE_LOG(LogTemp, Log, TEXT("ShowMap"));

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
	UE_LOG(LogTemp, Log, TEXT("DestroyMap"));

	for (auto It = ThreatBoxes.CreateConstIterator(); It; ++It)
	{
		if (IsValid(*It))
		{
			(*It)->Destroy();
		}
	}
	ThreatBoxes.Empty();

	SquareStates.Empty();
}

void UThreatMap::InitPieces()
{
	UE_LOG(LogTemp, Log, TEXT("InitPieces"));

	PlayerPieces.Empty();
	EnemyPieces.Empty();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APiece::StaticClass(), Actors);
	for (auto& Actor : Actors)
	{
		APiece* Piece = Cast<APiece>(Actor);
		if (!IsValid(Piece))
			continue;

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

void UThreatMap::UpdatePieces()
{
	UE_LOG(LogTemp, Log, TEXT("UpdatePieces"));

	for (auto It = PlayerPieces.CreateConstIterator(); It; ++It)
	{
		if (!IsValid(*It))
		{
			PlayerPieces.Remove(*It);
		}
	}

	for (auto It = EnemyPieces.CreateConstIterator(); It; ++It)
	{
		if (!IsValid(*It))
		{
			EnemyPieces.Remove(*It);
		}
	}
}
