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

private:
	void ShowLegalMoves();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	void SetType(EPieceType PT) { PieceType = PT; }
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const EPieceColor GetPieceColor() const { return PieceColor; }
	const EPieceType GetPieceType() const { return PieceType; }
	class UStaticMeshComponent* GetStaticMeshComponent();

	virtual void UpdateLegalMoves();

	bool IsAbleToPick();
	bool IsAbleToPut(FVector Dest) const;
	virtual void Put(FVector Dest);

	FVector GetPieceFowardVector() const;
	FVector GetPieceRightVector() const;
	void DestroyMoveBoxes();

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PieceMesh;

	UPROPERTY(VisibleAnywhere)
	TSet<FVector> LegalMoves;

	bool IsFirstMove() const { return bIsFirstMove; }

private:
	UPROPERTY(EditAnywhere, Category = PieceState)
	EPieceColor PieceColor;

	UPROPERTY(VisibleAnywhere, Category = PieceState)
	EPieceType PieceType;

	bool bIsFirstMove;

	UPROPERTY(VisibleAnywhere, Category = Moves)
	TArray<class APaperSpriteActor*> MoveBoxes;

	UPROPERTY(VisibleAnywhere, Category = Moves)
	TSubclassOf<class APaperSpriteActor> MoveBoxClass;

};
