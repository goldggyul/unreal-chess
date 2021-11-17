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
	void MovePickBoxToDest(FVector Dest);

	void PickCurPiece();
	void PutPiece();

private:
	UPROPERTY(VisibleAnywhere)
	EPieceColor MyColor;
	
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


};
