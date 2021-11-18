// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define BoardCenter FVector(1200.f, 1200.f, 0.f)
#define SquareSize 300.f
#define PickBoxZ 10.f

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
	Idle, // 아직 아무것도 선택 안함
	Pick, // 선택해서 Piece 들고 있는 중
	Put // Piece 두기 완료
};