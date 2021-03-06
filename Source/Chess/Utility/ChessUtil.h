// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Utility/ChessInfo.h"
#include "DrawDebugHelpers.h"
#include "ChessUtil.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API UChessUtil : public UObject
{
	GENERATED_BODY()
	
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
		FVector LeftTop = BoardCenter - FVector(SquareSize * 4.0f, SquareSize * 4.0f, 0.0f);
		FVector RightBottom = BoardCenter + FVector(SquareSize * 4.0f, SquareSize * 4.0f, 0.0f);
		if (Point.X < LeftTop.X || Point.X > RightBottom.X)
			return false;
		if (Point.Y < LeftTop.Y || Point.Y > RightBottom.Y)
			return false;
		return true;
	}

	static FVector GetSquareCenter(const FVector Point)
	{
		// 보드에서의 인덱스를 실제 레벨에서의 위치 벡터로 변환
		int32 X = Point.X / SquareSize + 1;
		int32 Y = Point.Y / SquareSize + 1;
		// Board의 왼쪽 상단 부분이 (0.0f, 0.0f, 0.0f)
		float LocationX = (BoardCenter.X - SquareSize * 4) + ((SquareSize / 2) * (2 * X - 1));
		float LocationY = (BoardCenter.Y - SquareSize * 4) + ((SquareSize / 2) * (2 * Y - 1));
		// UE_LOG(LogTemp, Warning, TEXT("SquareCenter: %d,%d -> %f %f "), X, Y, LocationX, LocationY);
		return FVector(LocationX, LocationY, BoardCenter.Z);
	}

	static AActor* GetCollidedPiece(const UWorld* World, FVector Point)
	{
		if (!UChessUtil::IsInBoard(Point))
		{
			return nullptr;
		}

		FVector Bottom = Point;
		FVector Top = Point;
		Top.Z += CollisionRange;

		DrawDebugLine(
			World, Top, Bottom,
			FColor::Magenta, false, 1.0f, 0.0f, 10.0f
		);

		FHitResult HitResult;
		FCollisionObjectQueryParams Query;
		Query.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
		if (World->LineTraceSingleByObjectType(OUT HitResult, Top, Bottom, Query))
		{
			return HitResult.GetActor();
		}
		return nullptr;
	}

	static FRotator GetPlayerDirection(EPieceColor Color)
	{
		if (Color == EPieceColor::Black)
		{
			return FRotator(0.0f, 180.0f, 0.0f);
		}
		return FRotator(0.0f, 0.0f, 0.0f);
	}

	static FVector GetPlayerForwardVector(EPieceColor Color)
	{
		if (Color == EPieceColor::Black)
		{
			return FVector(0.0f, 1.0f, 0.0f) * SquareSize;
		}
		return FVector(0.0f, -1.0f, 0.0f) * SquareSize;
	}

	static FVector GetPlayerRightVector(EPieceColor Color)
	{
		if (Color == EPieceColor::Black)
		{
			return FVector(-1.0f, 0.0f, 0.0f) * SquareSize;
		}
		return FVector(1.0f, 0.0f, 0.0f) * SquareSize;
	}
};
