// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define BoardCenter FVector(1200.f, 1200.f, 1200)
#define SquareSize 300.f

UENUM(BlueprintType)
enum class EPieceColor : uint8
{
	None,
	White,
	Black
};

UENUM(BlueprintType)
enum class EPieceType : uint8
{
	None,
	King,
	Queen,
	Rook,
	Bishop,
	Knight,
	Pawn
};

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle, // ���� �ƹ��͵� ���� ����
	Pick, // �����ؼ� Piece ��� �ִ� ��
	Put // Piece �α� �Ϸ�
};

// For debugging
class ChessUtil
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
		// ���忡���� �ε����� ���� ���������� ��ġ ���ͷ� ��ȯ
		int X = Point.X / SquareSize + 1;
		int Y = Point.Y / SquareSize + 1;
		// Board�� ���� ��� �κ��� (0.f, 0.f, 0.f)
		float LocationX = (BoardCenter.X - SquareSize * 4) + ((SquareSize / 2) * (2 * X - 1));
		float LocationY = (BoardCenter.Y - SquareSize * 4) + ((SquareSize / 2) * (2 * Y - 1));
		UE_LOG(LogTemp, Warning, TEXT("SquareCenter: %d,%d -> %f %f "), X, Y, LocationX, LocationY);
		return FVector(LocationX, LocationY, BoardCenter.Z);
	}
};
