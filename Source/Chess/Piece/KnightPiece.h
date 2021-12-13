// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "KnightPiece.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API AKnightPiece : public APiece
{
	GENERATED_BODY()
	
public:
	AKnightPiece();

protected:
	virtual TSet<FVector> GetBasicMovesInCurBoard() override;

};
