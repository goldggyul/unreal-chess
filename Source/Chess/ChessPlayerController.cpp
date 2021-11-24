// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPlayerController.h"
#include "ChessPlayer.h"
#include "ChessInfo.h"
#include "ChessUtil.h"
#include "DrawDebugHelpers.h"
#include "Engine/EngineTypes.h"

AChessPlayerController::AChessPlayerController()
{
}

void AChessPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CurPlayer = GetWorld()->SpawnActor<AChessPlayer>(FVector(1200.0f, 2100.0f, 2200.0f), FRotator(-70.0f, -90.0f, 0.0f));
	PrevPlayer = GetWorld()->SpawnActor<AChessPlayer>(FVector(1200.0f, 300.0f, 2200.0f), FRotator(-70.0f, 90.0f, 0.0f));

	CurPlayer->SetActorLabel(FString(TEXT("WhitePlayer")));
	PrevPlayer->SetActorLabel(FString(TEXT("BlackPlayer")));

	CurPlayer->SetPlayerColor(EPieceColor::White);
	PrevPlayer->SetPlayerColor(EPieceColor::Black);

	CurPlayer->SetFolderPath("/Player");
	PrevPlayer->SetFolderPath("/Player");
	SetFolderPath("/Player");

	CurPlayer->SetPickBoxStart(FVector(1350.0f,1950.0f,PickBoxZ));
	PrevPlayer->SetPickBoxStart(FVector(1050.0f,450.0f, PickBoxZ));

	bShowMouseCursor = true;

	Possess(CurPlayer);

	CurPlayer->SpawnPickBox();
	CurPlayer->UpdateThreatMap();
	CurPlayer->ShowThreatMap();
}

void AChessPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	FVector CurMousePos;
	if (GetMousePosition(CurMousePos.X, CurMousePos.Y))
	{
		if (!MousePos.Equals(CurMousePos))
		{
			MoveBoxToMouse();
		}
	}
	MousePos = CurMousePos;
}

void AChessPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (InputComponent)
	{
		InputComponent->BindAction(TEXT("Up"), EInputEvent::IE_Pressed, this, &AChessPlayerController::Up);
		InputComponent->BindAction(TEXT("Down"), EInputEvent::IE_Pressed, this, &AChessPlayerController::Down);
		InputComponent->BindAction(TEXT("Right"), EInputEvent::IE_Pressed, this, &AChessPlayerController::Right);
		InputComponent->BindAction(TEXT("Left"), EInputEvent::IE_Pressed, this, &AChessPlayerController::Left);
		InputComponent->BindAction(TEXT("Enter"), EInputEvent::IE_Pressed, this, &AChessPlayerController::Enter);
		InputComponent->BindAction(TEXT("Click"), EInputEvent::IE_Pressed, this, &AChessPlayerController::Click);
	}
}

void AChessPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	/*if (IsValid(CurPlayer))
	{
		UE_LOG(LogTemp, Warning, TEXT("POSSESS! [%s]"), *(CurPlayer->GetActorLabel()));
		CurPlayer->SpawnPickBox();
		CurPlayer->UpdateThreatMap();
		CurPlayer->ShowThreatMap();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cur Player is not valid!"));
	}*/
}

void AChessPlayerController::Up()
{
	FVector CurBoxLocation = CurPlayer->GetPickBoxLocation();
	FVector Differ = UChessUtil::GetPlayerForwardVector(CurPlayer->GetPlayerColor());
	CurPlayer->MovePickBox(CurBoxLocation + Differ);
}

void AChessPlayerController::Down()
{
	FVector CurBoxLocation = CurPlayer->GetPickBoxLocation();
	FVector Differ = UChessUtil::GetPlayerForwardVector(CurPlayer->GetPlayerColor());
	CurPlayer->MovePickBox(CurBoxLocation - Differ);
}

void AChessPlayerController::Right()
{
	FVector CurBoxLocation = CurPlayer->GetPickBoxLocation();
	FVector Differ = UChessUtil::GetPlayerRightVector(CurPlayer->GetPlayerColor());
	CurPlayer->MovePickBox(CurBoxLocation + Differ);
}

void AChessPlayerController::Left()
{
	FVector CurBoxLocation = CurPlayer->GetPickBoxLocation();
	FVector Differ = UChessUtil::GetPlayerRightVector(CurPlayer->GetPlayerColor());
	CurPlayer->MovePickBox(CurBoxLocation - Differ);
}

void AChessPlayerController::Enter()
{
	if (!IsValid(CurPlayer))
		return;

	if (CurPlayer->IsPicking())
	{
		bool bIsPutSucceeded = CurPlayer->PutCurPiece();
		if (bIsPutSucceeded)
			ChangePlayer();
	}
	else
	{
		CurPlayer->PickPiece();
	}
}

void AChessPlayerController::Click()
{
	bool bIsMovingSucceed = MoveBoxToMouse();
	if(bIsMovingSucceed)
		Enter();
}

bool AChessPlayerController::MoveBoxToMouse()
{
	if (!IsValid(CurPlayer))
		return false;

	// Trace to see what is under the mouse cursor
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, OUT HitResult);

	if (HitResult.bBlockingHit)
	{
		FVector HitPoint = HitResult.ImpactPoint;

		// For debugging
		FVector LineStart = HitPoint;
		FVector LineEnd = LineStart;
		LineEnd.Z += 100.0f;

		DrawDebugLine(
			GetWorld(), LineStart, LineEnd,
			FColor::Blue, false, 1.0f, 0.0f, 30.0f
		);

		if (UChessUtil::IsInBoard(HitPoint))
		{
			FString HitLabel = HitResult.Actor->GetActorLabel();
			FVector ClickedSquareCenter = UChessUtil::GetSquareCenter(HitPoint);
			CurPlayer->MovePickBox(ClickedSquareCenter);
			return true;
		}
	}
	
	return false;
}

void AChessPlayerController::ChangePlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("NEXT TURN"));

	UnPossess();
	Swap(CurPlayer, PrevPlayer);
	Possess(CurPlayer);

	PrevPlayer->DestroyPickBox();
	PrevPlayer->DestroyThreatMap();
	
	CurPlayer->SpawnPickBox();
	CurPlayer->UpdateThreatMap();
	CurPlayer->ShowThreatMap();


	// if Checkmate.. Check.. Stalemate... : ShowMap UI
}
