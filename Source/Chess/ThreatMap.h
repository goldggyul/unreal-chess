// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ChessInfo.h"
#include "ThreatMap.generated.h"

/**
 * 
 */

USTRUCT()
struct FPieceCount
{
	GENERATED_BODY()
	
	int WhiteCount = 0;
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
class CHESS_API UThreatMap : public UObject
{
	GENERATED_BODY()
	
public:
	UThreatMap();
	
public:
	void SetPlayerColor(EPieceColor Color);

	void Update();
	void Show();

private:
	void AddPieces();

private:
	UPROPERTY(VisibleAnywhere)
	EPieceColor PlayerColor;

	UPROPERTY(VisibleAnywhere)
	TSet<class APiece*> PlayerPieces;

	UPROPERTY(VisibleAnywhere)
	TSet<class APiece*> EnemyPieces;

	UPROPERTY(VisibleAnyWhere)
	TMap<FVector, FPieceCount> SquareStates;

	UPROPERTY(VisibleAnywhere, Category = ThreatBox)
	TSet<class APaperSpriteActor*> ThreatBoxes;

	UPROPERTY(VisibleAnywhere, Category = ThreatBox)
	TSubclassOf<class APaperSpriteActor> ThreatBoxClass;
};
