// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "KingPiece.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API AKingPiece : public APiece
{
	GENERATED_BODY()
	
public:
	AKingPiece();

protected:
	virtual TSet<FVector> GetBasicMovesInCurBoard() override;

private:
	bool IsLocationThreatenByEnemy(FVector CheckLocation, TSet<APiece*>& EnemyPieces) const;
};
