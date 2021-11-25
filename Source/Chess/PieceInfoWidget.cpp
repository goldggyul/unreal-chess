// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceInfoWidget.h"
#include "ChessPlayer.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

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
}

void UPieceInfoWidget::BindPlayer(class AChessPlayer* Player)
{
	CurrentPlayer = Player;
	CurrentPlayer->OnPickPiece.AddUObject(this, &UPieceInfoWidget::UpdateCurPiece);
	CurrentPlayer->OnPutPiece.AddUObject(this, &UPieceInfoWidget::EraseCurPiece);


}

void UPieceInfoWidget::UpdateCurPiece()
{
	EPieceType CurPieceType = CurrentPlayer->GetCurPieceType();

	if (IsValid(Img_PieceMove))
	{
		Img_PieceMove->SetBrushFromTexture(MoveImages[CurPieceType]);
	}

	if (IsValid(Text_PieceName))
	{
		Text_PieceName->SetText(PieceNames[CurPieceType]);
	}
	SetVisibility(ESlateVisibility::Visible);
}

void UPieceInfoWidget::EraseCurPiece()
{
	SetVisibility(ESlateVisibility::Hidden);
}
