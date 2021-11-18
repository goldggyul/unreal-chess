// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ChessInfo.h"
#include "ChessPlayer.generated.h"

UCLASS()
class CHESS_API AChessPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AChessPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetPieceColor(EPieceColor Color);
	EPieceColor GetPieceColor() const;

	void SetPickBoxStart(FVector BoxStart);
	FVector GetPickBoxLocation() const;

	void SetState(EPlayerState CurState);
	EPlayerState GetState() const;

	void SpawnPickBox();
	void DestroyPickBox();
	void MovePickBox(FVector Dest);

	void PickPiece();
	void SpawnPickedPiece();
	bool PutPiece();

	class APiece* GetCurPiece();

private:
	void SetMeshOpaque(bool bIsOpaque, class UStaticMeshComponent* MeshComponent = nullptr) const;

private:
	UPROPERTY(VisibleAnywhere)
	EPieceColor PlayerColor;
	
	UPROPERTY(VisibleAnywhere)
	EPlayerState MyState;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	FVector PrevMove; // 이전에 수를 놨던 위치. 박스 시작 위치 설정용

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class APaperSpriteActor> PickBoxClass;

	UPROPERTY(VisibleAnywhere)
	class APaperSpriteActor* PickBox;

	UPROPERTY(VisibleAnywhere)
	class AStaticMeshActor* PickedPiece;

	UPROPERTY(VisibleAnywhere)
	class APiece* CurPiece;

	// 불투명 <-> 투명하게 바꾸기 위해 머티리얼 저장
	UPROPERTY(VisibleAnywhere, Category = PieceMaterials)
	class UMaterial* DarkMaterial;

	UPROPERTY(VisibleAnywhere, Category = PieceMaterials)
	class UMaterial* LightMaterial;

	UPROPERTY(VisibleAnywhere, Category = PieceMaterials)
	class UMaterialInstance* TranslucentDarkMaterial;

	UPROPERTY(VisibleAnywhere, Category = PieceMaterials)
	class UMaterialInstance* TranslucentLightMaterial;
};
