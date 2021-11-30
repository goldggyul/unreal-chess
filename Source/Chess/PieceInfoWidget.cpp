// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceInfoWidget.h"
#include "ChessPlayer.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "ChessUtil.h"

UPieceInfoWidget::UPieceInfoWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> RookTexture(TEXT("Texture2D'/Game/UI/Images/IMG_RookMove.IMG_RookMove'"));
	if (RookTexture.Succeeded())
	{
		const FString RookStr = FString::Printf(TEXT("Rook"));
		PieceNames.Add(EPieceType::Rook, FText::FromString(RookStr));
		MoveImages.Add(EPieceType::Rook, RookTexture.Object);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> KnightTexture(TEXT("Texture2D'/Game/UI/Images/IMG_KnightMove.IMG_KnightMove'"));
	if (KnightTexture.Succeeded())
	{
		const FString KnightStr = FString::Printf(TEXT("Knight"));
		PieceNames.Add(EPieceType::Knight, FText::FromString(KnightStr));
		MoveImages.Add(EPieceType::Knight, KnightTexture.Object);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> BishopTexture(TEXT("Texture2D'/Game/UI/Images/IMG_BishopMove.IMG_BishopMove'"));
	if (BishopTexture.Succeeded())
	{
		const FString BishopStr = FString::Printf(TEXT("Bishop"));
		PieceNames.Add(EPieceType::Bishop, FText::FromString(BishopStr));
		MoveImages.Add(EPieceType::Bishop, BishopTexture.Object);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> KingTexture(TEXT("Texture2D'/Game/UI/Images/IMG_KingMove.IMG_KingMove'"));
	if (KingTexture.Succeeded())
	{
		const FString KingStr = FString::Printf(TEXT("King"));
		PieceNames.Add(EPieceType::King, FText::FromString(KingStr));
		MoveImages.Add(EPieceType::King, KingTexture.Object);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> QueenTexture(TEXT("Texture2D'/Game/UI/Images/IMG_QueenMove.IMG_QueenMove'"));
	if (QueenTexture.Succeeded())
	{
		const FString QueenStr = FString::Printf(TEXT("Queen"));
		PieceNames.Add(EPieceType::Queen, FText::FromString(QueenStr));
		MoveImages.Add(EPieceType::Queen, QueenTexture.Object);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> PawnTexture(TEXT("Texture2D'/Game/UI/Images/IMG_PawnMove.IMG_PawnMove'"));
	if (PawnTexture.Succeeded())
	{
		const FString PawnStr = FString::Printf(TEXT("Pawn"));
		PieceNames.Add(EPieceType::Pawn, FText::FromString(PawnStr));
		MoveImages.Add(EPieceType::Pawn, PawnTexture.Object);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> WhiteKingTexture(TEXT("Texture2D'/Game/UI/Images/WhiteKing.WhiteKing'"));
	if (WhiteKingTexture.Succeeded())
	{
		WhiteKingImage = WhiteKingTexture.Object;
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> BlackKingTexture(TEXT("Texture2D'/Game/UI/Images/BlackKing.BlackKing'"));
	if (BlackKingTexture.Succeeded())
	{
		BlackKingImage = BlackKingTexture.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> WhitePlayerTexture(TEXT("Texture2D'/Game/UI/Images/WhiteCat.WhiteCat'"));
	if (WhitePlayerTexture.Succeeded())
	{
		WhitePlayerImage = WhitePlayerTexture.Object;
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> BlackPlayerTexture(TEXT("Texture2D'/Game/UI/Images/GrayCat.GrayCat'"));
	if (BlackPlayerTexture.Succeeded())
	{
		BlackPlayerImage = BlackPlayerTexture.Object;
	}
}

void UPieceInfoWidget::BindPlayer(class AChessPlayer* CurrentPlayer)
{
	CurrentPlayer->OnPickPiece.AddUObject(this, &UPieceInfoWidget::UpdateCurPiece);
	CurrentPlayer->OnPutPiece.AddUObject(this, &UPieceInfoWidget::EraseCurPiece);
	Btn_Assist->OnClicked.AddDynamic(CurrentPlayer, &AChessPlayer::AssistPressed);
}

void UPieceInfoWidget::SetPlayerColor(EPieceColor CurPlayerColor)
{
	UTexture2D* PlayerKingImg = nullptr;
	UTexture2D* EnemyKingImg = nullptr;
	UTexture2D* PlayerImg = nullptr;
	UTexture2D* EnemyImg = nullptr;

	if (CurPlayerColor == EPieceColor::White)
	{
		PlayerKingImg = WhiteKingImage;
		EnemyKingImg = BlackKingImage;

		PlayerImg = WhitePlayerImage;
		EnemyImg = BlackPlayerImage;
	}
	else if (CurPlayerColor == EPieceColor::Black)
	{
		PlayerKingImg = BlackKingImage;
		EnemyKingImg = WhiteKingImage;

		PlayerImg = BlackPlayerImage;
		EnemyImg = WhitePlayerImage;
	}

	if (IsValid(Img_PlayerKing) && IsValid(PlayerKingImg))
	{
		Img_PlayerKing->SetBrushFromTexture(PlayerKingImg);
	}
	if (IsValid(Img_EnemyKing) && IsValid(EnemyKingImg))
	{
		Img_EnemyKing->SetBrushFromTexture(EnemyKingImg);
	}
	if (IsValid(Img_Player) && IsValid(PlayerImg))
	{
		Img_Player->SetBrushFromTexture(PlayerImg);
	}
	if (IsValid(Img_Enemy) && IsValid(EnemyImg))
	{
		Img_Enemy->SetBrushFromTexture(EnemyImg);
	}
}

void UPieceInfoWidget::HidePieceNameAndResult()
{
	Overlay_PieceInfo->SetVisibility(ESlateVisibility::Hidden);
	Text_Result->SetVisibility(ESlateVisibility::Hidden);
}

void UPieceInfoWidget::UpdateCurPiece(EPieceType CurPieceType)
{
	FString PieceName = UChessUtil::GetPieceTypeString(CurPieceType);
	if (IsValid(Img_PieceMove))
	{
		Img_PieceMove->SetBrushFromTexture(MoveImages[CurPieceType]);
	}

	if (IsValid(Text_PieceName))
	{
		Text_PieceName->SetText(PieceNames[CurPieceType]);
	}
	Overlay_PieceInfo->SetVisibility(ESlateVisibility::Visible);
}

void UPieceInfoWidget::EraseCurPiece()
{
	UE_LOG(LogTemp, Log, TEXT("Widget: EraseCurPiece"));

	Overlay_PieceInfo->SetVisibility(ESlateVisibility::Hidden);
}

void UPieceInfoWidget::ShowResult(FString Result)
{
	Text_Result->SetText(FText::FromString(Result));
	Text_Result->SetVisibility(ESlateVisibility::Visible);
}
