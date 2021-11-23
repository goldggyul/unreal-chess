// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessInfo.h"
#include "ChessUtil.h"
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
	void DestroyMoveBoxes();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	void SetType(EPieceType PT) { PieceType = PT; }
	virtual void UpdateLegalMoves();
	void RemoveCheckMoves();
	void AddToLegalMoves(const FVector Location);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const EPieceColor GetPieceColor() const { return PieceColor; }
	const EPieceType GetPieceType() const { return PieceType; }
	class UStaticMeshComponent* GetStaticMeshComponent();

	bool IsAbleToPick();
	bool IsAbleToPutAt(FVector Dest) const;
	virtual void PutAt(FVector Dest);

	bool IsFirstMove() const { return bIsFirstMove; }
	
protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PieceMesh;

	UPROPERTY(VisibleAnywhere)
	TSet<FVector> LegalMoves;

private:
	UPROPERTY(EditAnywhere, Category = PieceState)
	EPieceColor PieceColor;

	UPROPERTY(VisibleAnywhere, Category = PieceState)
	EPieceType PieceType;

	UPROPERTY(VisibleAnywhere, Category = PieceState)
	bool bIsFirstMove;

	UPROPERTY(VisibleAnywhere, Category = Moves)
	TArray<class APaperSpriteActor*> MoveBoxes;

	UPROPERTY(VisibleAnywhere, Category = Moves)
	TSubclassOf<class APaperSpriteActor> MoveBoxClass;

};
