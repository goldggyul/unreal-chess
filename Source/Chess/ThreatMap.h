// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChessInfo.h"
#include "ThreatMap.generated.h"

/**
 * 
 */

USTRUCT()
struct FPieceCount
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	int WhiteCount = 0;

	UPROPERTY(VisibleAnywhere)
	int BlackCount = 0;

	void AddCount(EPieceColor PieceColor)
	{
		if (PieceColor == EPieceColor::White)
		{
			WhiteCount++;
		}
		else if (PieceColor == EPieceColor::Black)
		{
			BlackCount++;
		}
	}
};

UCLASS()
class CHESS_API UThreatMap : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UThreatMap();
	
public:
	void SetPlayerColor(EPieceColor Color);

	void UpdateMap();
	void ShowMap();
	void DestroyMap();

private:
	void InitPieces();
	void UpdatePieces();

private:
	UPROPERTY(VisibleAnywhere, Category = ThreatMap)
	EPieceColor PlayerColor;

	UPROPERTY(VisibleAnywhere, Category = ThreatMap)
	TSet<class APiece*> PlayerPieces;

	UPROPERTY(VisibleAnywhere, Category = ThreatMap)
	TSet<class APiece*> EnemyPieces;

	UPROPERTY(VisibleAnywhere, Category = ThreatMap)
	TMap<FVector, FPieceCount> SquareStates;

	UPROPERTY(VisibleAnywhere, Category = ThreatMap)
	TSet<class AThreatBox*> ThreatBoxes;
};
