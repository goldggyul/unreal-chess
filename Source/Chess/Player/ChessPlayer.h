// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Utility/ChessInfo.h"
#include "ChessPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPickPiece, EPieceType);
DECLARE_MULTICAST_DELEGATE(FOnPutPiece);

UCLASS()
class CHESS_API AChessPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AChessPlayer();

public:	
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

	EPieceType GetCurPieceType() const;

	bool IsCheckmate() const;
	bool IsStalemate() const;
	bool IsCheck() const;

	UFUNCTION()
	void ThreatBtnPressed();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void SpawnPickedPiece();
	class APiece* GetPieceAtPickBox();

public:
	FOnPickPiece OnPickPiece;
	FOnPutPiece OnPutPiece;

private:
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	EPieceColor PlayerColor;
	
	UPROPERTY(VisibleAnywhere)
	bool bIsPicking;

	UPROPERTY(VisibleAnywhere)
	FVector CurMove;

	UPROPERTY(VisibleAnywhere)
	class UThreatMap* ThreatMap;

	// Assist On, ThreatMap On
	UPROPERTY()
	bool bIsThreatMapVisible;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class APaperSpriteActor> PickBoxClass;

	UPROPERTY(VisibleAnywhere)
	class APaperSpriteActor* PickBox;

	UPROPERTY(VisibleAnywhere)
	class APiece* PickedPiece;

	UPROPERTY(VisibleAnywhere)
	class APiece* CurPickedPiece;

	UPROPERTY(VisibleAnywhere)
	class USoundWave* PickSound;

	UPROPERTY(VisibleAnywhere)
	class USoundWave* PutSound;


};
