// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChessPlayerController.generated.h"


UCLASS()
class CHESS_API AChessPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AChessPlayerController();

public:

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void ResignPressed();

protected:
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	virtual void OnUnPossess() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	void Up();
	void Down();
	void Right();
	void Left();
	void Enter();
	void Click();

	bool MoveBoxToMouse();

	void ChangePlayer();

private:
	UPROPERTY(VisibleAnywhere)
	class AChessPlayer* CurPlayer;

	UPROPERTY(VisibleAnywhere)
	class AChessPlayer* PrevPlayer;

	UPROPERTY(VisibleAnywhere)
	FVector MousePos;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class UPieceInfoWidget> PieceInfoClass;

	UPROPERTY(VisibleAnywhere)
	UPieceInfoWidget* PieceInfoWidget;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class UEndWidget> EndWidgetClass;

	//UPROPERTY(VisibleAnywhere)
	//UEndWidget* EndWidget;
};
