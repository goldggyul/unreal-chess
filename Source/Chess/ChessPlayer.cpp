// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPlayer.h"
#include "Camera/CameraComponent.h"
#include "PaperSpriteActor.h"

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

void AChessPlayer::SetBoxStart(FVector BoxStartLocation)
{
	PrevMove = BoxStartLocation;
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
}

