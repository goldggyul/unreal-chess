// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessInfo.h"
#include "DrawDebugHelpers.h"

/**
 * 
 */
class CHESS_API ChessUtil
{
public:
	static FString GetColorString(EPieceColor Color)
	{
		FString PieceColorName;
		switch (Color)
		{
		case EPieceColor::Black:
			PieceColorName = TEXT("Black");
			break;
		case EPieceColor::White:
			PieceColorName = TEXT("White");
			break;
		default:
			break;
		}
		return PieceColorName;
	}

	static FString GetPieceTypeString(EPieceType PieceType)
	{
		FString PieceTypeName;
		switch (PieceType)
		{
		case EPieceType::Bishop:
			PieceTypeName = TEXT("Bishop");
			break;
		case EPieceType::King:
			PieceTypeName = TEXT("King");
			break;
		case EPieceType::Knight:
			PieceTypeName = TEXT("Knight");
			break;
		case EPieceType::Pawn:
			PieceTypeName = TEXT("Pawn");
			break;
		case EPieceType::Queen:
			PieceTypeName = TEXT("Queen");
			break;
		case EPieceType::Rook:
			PieceTypeName = TEXT("Rook");
			break;
		default:
			break;
		}
		return PieceTypeName;
	}

	static bool IsInBoard(const FVector Point)
	{
		FVector LeftTop = BoardCenter - FVector(SquareSize * 4, SquareSize * 4, 0);
		FVector RightBottom = BoardCenter + FVector(SquareSize * 4, SquareSize * 4, 0);
		if (Point.X < LeftTop.X || Point.X > RightBottom.X)
			return false;
		if (Point.Y<LeftTop.Y || Point.X>RightBottom.Y)
			return false;
		return true;
	}

	static FVector GetSquareCenter(const FVector Point)
	{
		// 보드에서의 인덱스를 실제 레벨에서의 위치 벡터로 변환
		int X = Point.X / SquareSize + 1;
		int Y = Point.Y / SquareSize + 1;
		// Board의 왼쪽 상단 부분이 (0.f, 0.f, 0.f)
		float LocationX = (BoardCenter.X - SquareSize * 4) + ((SquareSize / 2) * (2 * X - 1));
		float LocationY = (BoardCenter.Y - SquareSize * 4) + ((SquareSize / 2) * (2 * Y - 1));
		UE_LOG(LogTemp, Warning, TEXT("SquareCenter: %d,%d -> %f %f "), X, Y, LocationX, LocationY);
		return FVector(LocationX, LocationY, BoardCenter.Z);
	}

	static AActor* GetCollidedPiece(const UWorld* World, FVector Point)
	{
		FVector Bottom = Point;
		FVector Top = Point;
		Top.Z += 700.f;

		DrawDebugLine(
			World, Top, Bottom,
			FColor::Magenta, false, 1.f, 0.f, 10.f
		);

		FHitResult HitResult;
		FCollisionObjectQueryParams Query;
		Query.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
		if (World->LineTraceSingleByObjectType(OUT HitResult, Top, Bottom, Query))
		{
			return HitResult.GetActor();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[LINE TRACE] NO COLLISION"));
		}
		return nullptr;
	}
};
