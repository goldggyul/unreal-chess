// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ChessInfo.h"
#include "ThreatMap.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API UThreatMap : public UObject
{
	GENERATED_BODY()

public:
	void SetPlayerColor(EPieceColor Color);

	void Update();
	void Show();

private:
	EPieceColor PlayerColor;

};
