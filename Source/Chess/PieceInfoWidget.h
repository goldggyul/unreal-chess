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

public:
	EPieceColor PlayerColor;

private:
	TWeakObjectPtr<class AChessPlayer> CurrentPlayer;

	UPROPERTY()
	TMap<EPieceType, class UTexture2D*> MoveImages;

	UPROPERTY()
	TMap<EPieceType, FText> PieceNames;

	UPROPERTY()
	class UTexture2D* WhiteKingImage;

	UPROPERTY()
	class UTexture2D* BlackKingImage;

	// LeftTop
	// PieceInfo
	UPROPERTY(meta = (BindWidget))
	class UOverlay* Overlay_PieceInfo;

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

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Assist;

	// RightTop
	// EnemyInfo
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Enemy;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_EnemyColor;

	
};
