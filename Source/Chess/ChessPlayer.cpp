// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPlayer.h"
#include "Camera/CameraComponent.h"
#include "PaperSpriteActor.h"
#include "Piece.h"
#include "DrawDebugHelpers.h"

// Sets default values
AChessPlayer::AChessPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	//Camera->SetupAttachment(RootComponent);
	SetRootComponent(Camera);

	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	static ConstructorHelpers::FClassFinder<APaperSpriteActor> PS(TEXT("Blueprint'/Game/Blueprints/FocusBox/BS_CurBox.BS_CurBox_C'"));
	if (PS.Succeeded())
	{
		CurBoxClass = PS.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot load CurBox class"));
	}

	MyState = EPlayerState::Idle;
}

// Called when the game starts or when spawned
void AChessPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChessPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChessPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChessPlayer::SetPieceColor(EPieceColor Color)
{
	PieceColor = Color;
}

EPieceColor AChessPlayer::GetPieceColor() const
{
	return PieceColor;
}

void AChessPlayer::SetBoxStart(FVector BoxStart)
{
	PrevMove = BoxStart;
}

FVector AChessPlayer::GetCurBoxLocation() const
{
	if (IsValid(CurBox))
	{
		return CurBox->GetActorLocation();
	}
	return FVector::ZeroVector;
}

void AChessPlayer::SetState(EPlayerState CurState)
{
	MyState = CurState;
}

EPlayerState AChessPlayer::GetState() const
{
	return MyState;
}

void AChessPlayer::SpawnCurBox()
{
	if (!IsValid(CurBoxClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("CurBoxClass is not exist"));
		return;
	}

	if (IsValid(CurBox))
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroy Previous CurBox"));
		CurBox->Destroy();
	}
	CurBox = GetWorld()->SpawnActor<APaperSpriteActor>(CurBoxClass, PrevMove, FRotator::ZeroRotator);
	CurBox->SetActorLabel(TEXT("CurBox"));
	CurBox->SetFolderPath("/Player");
}

void AChessPlayer::DestroyCurBox()
{
	CurBox->Destroy();
}

void AChessPlayer::MoveBoxToDest(FVector Dest)
{
	//UE_LOG(LogTemp, Warning, TEXT("Move To (%f, %f, %f)"), Dest.X, Dest.Y, Dest.Z);

	if (Dest.X < 150.f || Dest.X >2250.f || Dest.Y < 150.f || Dest.Y>2250.f)
		return;
	if (IsValid(CurBox))
	{
		CurBox->SetActorLocation(Dest);
		//UE_LOG(LogTemp, Warning, TEXT("Box Moved"));
		PrevMove = Dest;
	}
}

void AChessPlayer::PickCurPiece()
{
	// 선택할 수 있으면 들기

	// Test: 일단 자기꺼면 든다
	FHitResult HitResult;

	FVector Start = CurBox->GetActorLocation();
	FVector End = Start;
	Start.Z += 700.f;
	
	FCollisionObjectQueryParams Query;
	Query.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
	if (GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, Query))
	{
		APiece* HitPiece = Cast<APiece>(HitResult.GetActor());
		if (IsValid(HitPiece))
		{
			// For debugging
			FString PieceTypeName = ChessUtil::GetPieceTypeString(HitPiece->GetType());
			FString PieceColorName = ChessUtil::GetColorString(HitPiece->GetColor());
			UE_LOG(LogTemp, Warning, TEXT("HITTED: %s %s"), *PieceColorName, *PieceTypeName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO COLLISION"));
	}

	// For debugging
	FVector LineStart = CurBox->GetActorLocation();
	FVector LineEnd = LineStart;
	LineEnd.Z += 700.f;

	DrawDebugLine(
		GetWorld(), LineStart, LineEnd,
		FColor::Magenta, false, 2.f, 0.f, 10.f
	);
}

