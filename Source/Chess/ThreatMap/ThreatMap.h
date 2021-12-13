// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Utility/ChessInfo.h"
#include "ThreatMap.generated.h"

/**
 * 
 */

USTRUCT()
struct FPieceCount
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	int32 WhiteCount = 0;

	UPROPERTY(VisibleAnywhere)
	int32 BlackCount = 0;

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

	int32 GetEnemyCount(EPieceColor PlayerColor)
	{
		if (PlayerColor == EPieceColor::White)
		{
			return BlackCount;
		}
		else if (PlayerColor == EPieceColor::Black)
		{
			return WhiteCount;
		}
		return 0;
	}
};

UCLASS()
class CHESS_API UThreatMap : public UActorComponent
{
	GENERATED_BODY()
	
public:
	void SetPlayerColor(EPieceColor Color);

	void UpdateMap();
	void ShowMap();
	void DestroyThreatBoxes();

	bool IsCheck() const { return bIsCheck; }
	bool IsCheckmate() const { return bIsCheckmate; }
	bool IsStalemate() const { return bIsStalemate; }

private:
	void InitPieces();
	void UpdatePiecesMove();
	void UpdateSquareStates();
	void UpdateGameResult();

private:
	UPROPERTY(VisibleAnywhere, Category = ThreatMap)
	EPieceColor PlayerColor;

	UPROPERTY(VisibleAnywhere, Category = ThreatMap)
	TSet<class APiece*> PlayerPieces;

	UPROPERTY(VisibleAnywhere, Category = ThreatMap)
	class APiece* PlayerKing;

	UPROPERTY(VisibleAnywhere, Category = ThreatMap)
	TSet<class APiece*> EnemyPieces;

	UPROPERTY(VisibleAnywhere, Category = ThreatMap)
	TMap<FVector, FPieceCount> SquareStates;

	UPROPERTY(VisibleAnywhere, Category = ThreatMap)
	TSet<class AThreatBox*> ThreatBoxes;

	UPROPERTY(VisibleAnywhere, Category = ThreatMap)
	bool bIsCheck;

	UPROPERTY(VisibleAnywhere, Category = ThreatMap)
	bool bIsCheckmate;

	UPROPERTY(VisibleAnywhere, Category = ThreatMap)
	bool bIsStalemate;
};
