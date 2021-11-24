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
	virtual void UpdateBasicMoves() override;
};
