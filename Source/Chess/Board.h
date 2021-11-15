// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PieceState.h"
#include "Board.generated.h"

UCLASS()
class CHESS_API ABoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoard();

protected:
	virtual void PostInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector TransformIndexToLocation(uint8 Index) const;

	uint8 GetSquareSize() const { return SquareSize; }
	EPieceColor GetPieceColor(uint8 Index) const { return PieceColor[Index]; }
	EPieceType GetPieceType(uint8 Index) const { return PieceType[Index]; }

private:
	void InitBoardState();

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BoardMesh;
	 
	UPROPERTY(VisibleAnywhere)
	TArray<EPieceColor> PieceColor;

	UPROPERTY(VisibleAnywhere)
	TArray<EPieceType> PieceType;

	UPROPERTY(VisibleAnywhere)
	float SquareSize; // 한 칸의 가로(세로) 길이

};
