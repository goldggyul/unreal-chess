// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ChessGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API AChessGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

private:
	void SpawnChessSet();
	void SpawnBoard();
	void SpawnPieces();
	
private:
	class ABoard* Board;
};
