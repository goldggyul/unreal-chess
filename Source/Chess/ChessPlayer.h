// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PieceState.h"
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

	void SetBoxStart(FVector BoxStart);
	FVector GetCurBoxLocation() const;

	void SetState(EPlayerState CurState);
	EPlayerState GetState() const;

	void SpawnCurBox();
	void DestroyCurBox();
	void MoveBoxToDest(FVector Dest);

private:
	UPROPERTY(VisibleAnywhere)
	EPieceColor PieceColor;
	
	UPROPERTY(VisibleAnywhere)
	EPlayerState MyState;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	FVector PrevMove; // ������ ���� ���� ��ġ. �ڽ� ���� ��ġ ������

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class APaperSpriteActor> CurBoxClass;

	UPROPERTY(VisibleAnywhere)
	class APaperSpriteActor* CurBox;


};
