// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChessInfo.h"
#include "EndWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API UEndWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetWinner(EPieceColor WinnerColor);
	void SetResultToDraw();

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_White;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Black;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Draw;

};
