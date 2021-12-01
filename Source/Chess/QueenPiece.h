// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "QueenPiece.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API AQueenPiece : public APiece
{
	GENERATED_BODY()
	
public:
	AQueenPiece();

protected:
	virtual TSet<FVector> GetBasicMovesInCurBoard() override;

};
