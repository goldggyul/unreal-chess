// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPlayerController.h"
#include "ChessPlayer.h"
#include "ChessInfo.h"
#include "ChessUtil.h"

AChessPlayerController::AChessPlayerController()
{

}

void AChessPlayerController::BeginPlay()
{
	Super::BeginPlay();

	

	CurPlayer = GetWorld()->SpawnActor<AChessPlayer>(FVector(1200.f, 2100.f, 2200.f), FRotator(-70.f, -90.f, 0.f));
	PrevPlayer = GetWorld()->SpawnActor<AChessPlayer>(FVector(1200.f, 300.f, 2200.f), FRotator(-70.f, 90.f, 0.f));

	CurPlayer->SetActorLabel(FString(TEXT("WhitePlayer")));
	PrevPlayer->SetActorLabel(FString(TEXT("BlackPlayer")));

	CurPlayer->SetPieceColor(EPieceColor::White);
	PrevPlayer->SetPieceColor(EPieceColor::Black);

	CurPlayer->SetFolderPath("/Player");
	PrevPlayer->SetFolderPath("/Player");
	SetFolderPath("/Player");

	CurPlayer->SetBoxStart(FVector(1350.f,1950.f,10.f));
	PrevPlayer->SetBoxStart(FVector(1050.f,450.f,10.f));

	bShowMouseCursor = true;

	OnPossess(CurPlayer);
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
	
	if (IsValid(CurPlayer))
	{
		UE_LOG(LogTemp, Warning, TEXT("POSSESS!"));
		CurPlayer->SpawnCurBox();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cur Player is not valid!"));
	}
}

void AChessPlayerController::MoveBoxToDirection(FVector Dir)
{
	FVector Dest = CurPlayer->GetCurBoxLocation();
	if (CurPlayer->GetPieceColor() == EPieceColor::White)
	{
		Dest += Dir * 300.f;
	}
	else
	{
		Dest += Dir * -300.f;
	}
	CurPlayer->MoveBoxToDest(Dest);
}

void AChessPlayerController::Up()
{
	MoveBoxToDirection(FVector(0.f, -1.f, 0.f));
}

void AChessPlayerController::Down()
{
	MoveBoxToDirection(FVector(0.f, 1.f, 0.f));
}

void AChessPlayerController::Right()
{
	MoveBoxToDirection(FVector(1.f, 0.f, 0.f));
}

void AChessPlayerController::Left()
{
	MoveBoxToDirection(FVector(-1.f, 0.f, 0.f));
}

void AChessPlayerController::Enter()
{
	switch (CurPlayer->GetState())
	{
	case EPlayerState::Idle:
		CurPlayer->PickCurPiece();
		break;
	case EPlayerState::Pick:
	{
		// 선택할 수 있으면 놓고 턴 넘기기,
		// 선택 못하면 Piece 다시 제자리에 놓고 Idle로
	}
	case EPlayerState::Put:
	default:
		break;
	}

	//ChangePlayer();
}

void AChessPlayerController::Click()
{
	if (!IsValid(CurPlayer))
		return;

	// Trace to see what is under the mouse cursor
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, OUT HitResult);

	if (HitResult.bBlockingHit)
	{
		FVector HitPoint = HitResult.ImpactPoint;
		if (ChessUtil::IsInBoard(HitPoint))
		{
			FString HitLabel = HitResult.Actor->GetActorLabel();
			// 왜 Board만?
			UE_LOG(LogTemp, Warning, TEXT("Mouse Hit: %s (%f, %f, %f)"), *HitLabel, HitPoint.X, HitPoint.Y, HitPoint.Z);
			FVector ClickedSquareCenter = ChessUtil::GetSquareCenter(HitPoint);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Mouse didn't hit anything"));
	}
}

void AChessPlayerController::ChangePlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("NEXT TURN"));

	CurPlayer->DestroyCurBox();

	UnPossess();
	Swap(CurPlayer, PrevPlayer);
	OnPossess(CurPlayer);
}
