// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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
};
