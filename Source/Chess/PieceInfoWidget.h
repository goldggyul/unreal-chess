// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChessInfo.h"
#include "PieceInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API UPieceInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPieceInfoWidget(const FObjectInitializer& ObjectInitializer);
	
public:
	void BindPlayer(class AChessPlayer* Player);

	void UpdateCurPiece();
	void EraseCurPiece();

private:
	TWeakObjectPtr<class AChessPlayer> CurrentPlayer;

	// LeftTop
	// PieceInfo
	UPROPERTY()
	TMap<EPieceType, class UTexture2D*> MoveImages;

	UPROPERTY()
	TMap<EPieceType, FText> PieceNames;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_PieceMove;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_PieceName;

	// LeftBottom
	// PlayerBtn
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Assist;
	// PlayerInfo
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_PlayerColor;

	// RightTop
	// EnemyInfo
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Enemy;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_EnemyColor;

	
};
