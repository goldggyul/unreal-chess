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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetPlayerColor(EPieceColor Color);
	EPieceColor GetPlayerColor() const;

	void SetPickBoxStart(FVector BoxStart);
	FVector GetPickBoxLocation() const;

	void SetPicking(bool Picking);
	bool IsPicking() const;

	void SpawnPickBox();
	void DestroyPickBox();
	void MovePickBox(FVector Dest);

	void PickPiece();
	bool PutCurPiece();

	void UpdateThreatMap();
	void ShowThreatMap();
	void DestroyThreatMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void SetMeshOpaque(bool bIsOpaque, class UStaticMeshComponent* MeshComponent = nullptr) const;

	void SpawnPickedPiece();
	class APiece* GetCurPiece();


private:
	UPROPERTY(VisibleAnywhere)
	EPieceColor PlayerColor;
	
	UPROPERTY(VisibleAnywhere)
	bool bIsPicking;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	FVector PrevMove; // 이전에 수를 놨던 위치.

	UPROPERTY(VisibleAnywhere)
	FVector CurMove;

	UPROPERTY(VisibleAnywhere)
	class UThreatMap* ThreatMap;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class APaperSpriteActor> PickBoxClass;

	UPROPERTY(VisibleAnywhere)
	class APaperSpriteActor* PickBox;

	UPROPERTY(VisibleAnywhere)
	class AStaticMeshActor* PickedPiece;

	UPROPERTY(VisibleAnywhere)
	class APiece* CurPiece;

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
