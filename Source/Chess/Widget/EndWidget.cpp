// Fill out your copyright notice in the Description page of Project Settings.


#include "EndWidget.h"
#include "Components/TextBlock.h"

void UEndWidget::SetWinner(EPieceColor WinnerColor)
{
	if (WinnerColor == EPieceColor::Black)
	{
		Text_White->SetText(FText::FromString(TEXT("Too bad...")));
		Text_Black->SetText(FText::FromString(TEXT("Winner!")));
	}
}

void UEndWidget::SetResultToDraw()
{
	Text_Draw->SetVisibility(ESlateVisibility::Visible);
	Text_White->SetVisibility(ESlateVisibility::Hidden);
	Text_Black->SetVisibility(ESlateVisibility::Hidden);
}
