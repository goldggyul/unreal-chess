// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define BoardCenter FVector(1200.0f, 1200.0f, 0.0f)
#define PieceMeshSize FVector(2.0f, 2.0f, 2.0f)
#define SquareSize 300.0f
#define PickBoxZ 10.0f
#define MoveBoxZ 5.0f
#define PickedPieceZ 450.0f
#define CollisionRange 700.0f

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
enum class EPlayState : uint8
{
	Check,
	Checkmate,
	Stalemate
};
