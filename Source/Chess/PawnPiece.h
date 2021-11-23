// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PawnPiece.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API APawnPiece : public APiece
{
	GENERATED_BODY()
	
public:
	APawnPiece();

protected:
	virtual void UpdateMoves() override;
};
