// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ChessGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API AChessGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AChessGameModeBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
};