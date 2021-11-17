// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessInfo.h"
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
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const EPieceColor GetColor() const { return Color; }
	const EPieceType GetType() const { return Type; }

	virtual void UpdateLegalMoves();

	bool IsEnableToPick() const;

protected:
	void SetType(EPieceType PieceType) { Type = PieceType; }

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PieceMesh;

	UPROPERTY(VisibleAnywhere)
	TSet<uint8> LegalMoves;

private:
	UPROPERTY(EditAnywhere, Category = PieceState)
	EPieceColor Color;

	UPROPERTY(VisibleAnywhere, Category = PieceState)
	EPieceType Type;
};
