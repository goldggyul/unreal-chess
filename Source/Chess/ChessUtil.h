// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ChessInfo.h"
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
		// ���忡���� �ε����� ���� ���������� ��ġ ���ͷ� ��ȯ
		int X = Point.X / SquareSize + 1;
		int Y = Point.Y / SquareSize + 1;
		// Board�� ���� ��� �κ��� (0.0f, 0.0f, 0.0f)
		float LocationX = (BoardCenter.X - SquareSize * 4) + ((SquareSize / 2) * (2 * X - 1));
		float LocationY = (BoardCenter.Y - SquareSize * 4) + ((SquareSize / 2) * (2 * Y - 1));
		UE_LOG(LogTemp, Warning, TEXT("SquareCenter: %d,%d -> %f %f "), X, Y, LocationX, LocationY);
		return FVector(LocationX, LocationY, BoardCenter.Z);
	}

	static AActor* GetCollidedPiece(const UWorld* World, FVector Point)
	{
		if (!UChessUtil::IsInBoard(Point))
		{
			UE_LOG(LogTemp, Warning, TEXT("[LINE TRACE] OUT OF BOARD"));
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
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[LINE TRACE] NO COLLISION"));
		}
		return nullptr;
	}

};
