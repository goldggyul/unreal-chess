// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PieceState.h"
#include "Piece.generated.h"

UCLASS()
class CHESS_API APiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APiece();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetColor(EPieceColor PieceColor);

	//void SetType(EPieceType PieceType);
	//virtual void SetMesh();

	virtual void UpdateLegalMoves();

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PieceMesh;

private:
	UPROPERTY(VisibleAnywhere)
	uint8 Index; // Board에서의 인덱스

	UPROPERTY(VisibleAnywhere)
	EPieceColor Color;

	UPROPERTY(VisibleAnywhere)
	UMaterial* LightMaterial;

	UPROPERTY(VisibleAnywhere)
	UMaterial* DarkMaterial;

	UPROPERTY(VisibleAnywhere)
	EPieceType Type;

	UPROPERTY(VisibleAnywhere)
	TSet<uint8> LegalMoves;
};
