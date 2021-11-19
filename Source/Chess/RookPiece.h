// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "RookPiece.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API ARookPiece : public APiece
{
	GENERATED_BODY()
	
public:
	ARookPiece();

protected:
	virtual void UpdateLegalMoves();
};
