// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Utility/ChessInfo.h"
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
	void SetPlayerColor(EPieceColor CurPlayerColor);
	void HidePieceNameAndResult();

	void UpdateCurPiece(EPieceType CurPieceType);
	void EraseCurPiece();

	void ShowResult(FString Result);

public:
	EPieceColor PlayerColor;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UButton* Btn_ThreatMap;

private:
	UPROPERTY()
	TMap<EPieceType, class UTexture2D*> MoveImages;

	UPROPERTY()
	TMap<EPieceType, FText> PieceNames;

	UPROPERTY()
	class UTexture2D* WhiteKingImage;

	UPROPERTY()
	class UTexture2D* BlackKingImage;

	UPROPERTY()
	class UTexture2D* WhitePlayerImage;

	UPROPERTY()
	class UTexture2D* BlackPlayerImage;

	// LeftTop
	// PieceInfo
	UPROPERTY(meta = (BindWidget))
	class UOverlay* Overlay_PieceInfo;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_PieceMove;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_PieceName;

	// LeftBottom
	// PlayerInfo
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Player;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_PlayerKing;


	// RightTop
	// EnemyInfo
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Enemy;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_EnemyKing;

	// Result
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Result;

	
};
