// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Utility/ChessInfo.h"
#include "../Utility/ChessUtil.h"
#include "Piece.generated.h"


UCLASS()
class CHESS_API APiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APiece();

public:
	const EPieceColor GetPieceColor() const { return PieceColor; }
	const EPieceType GetPieceType() const { return PieceType; }

	virtual void UpdateBasicMoves();
	void RemoveMoveKingCheckedByEnemies(APiece* MyKing, TSet<APiece*>& EnemyPieces);
	bool CanMoveTo(FVector Location);

	bool IsAbleToPick();
	bool IsAbleToPutAt(FVector Dest) const;
	void PutAt(FVector Dest);

	bool IsFirstMove() const { return bIsFirstMove; }
	
	TSet<FVector> GetMoves() { return Moves; };

	void ShowMoves();
	void DestroyMoveBoxes();

	class APiece* GetCopy();
	void SetMeshOpaque(bool bIsOpaque);

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual TSet<FVector> GetBasicMovesInCurBoard();


	void SetType(EPieceType PT) { PieceType = PT; }
	void AddToMoves(const FVector Location);

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PieceMesh;

	UPROPERTY(VisibleAnywhere)
	TSet<FVector> Moves;

private:
	UPROPERTY(EditAnywhere, Category = PieceState)
	EPieceColor PieceColor;

	UPROPERTY(VisibleAnywhere, Category = PieceState)
	EPieceType PieceType;

	UPROPERTY(VisibleAnywhere, Category = PieceState)
	bool bIsFirstMove;

	UPROPERTY(VisibleAnywhere, Category = Moves)
	TSet<class APaperSpriteActor*> MoveBoxes;

	UPROPERTY(VisibleAnywhere, Category = Moves)
	TSubclassOf<class APaperSpriteActor> MoveBoxClass;

	// 피스들 불투명 <-> 투명하게 바꾸기 위해 머티리얼 저장
	UPROPERTY(VisibleAnywhere, Category = PieceMaterials)
	class UMaterial* DarkMaterial;

	UPROPERTY(VisibleAnywhere, Category = PieceMaterials)
	class UMaterial* LightMaterial;

	UPROPERTY(VisibleAnywhere, Category = PieceMaterials)
	class UMaterialInstance* TranslucentDarkMaterial;

	UPROPERTY(VisibleAnywhere, Category = PieceMaterials)
	class UMaterialInstance* TranslucentLightMaterial;
};
