// Fill out your copyright notice in the Description page of Project Settings.


#include "Piece.h"
#include "PaperSpriteActor.h"

// Sets default values
APiece::APiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsFirstMove = true;

	static ConstructorHelpers::FClassFinder<APaperSpriteActor> PS(TEXT("Blueprint'/Game/Blueprints/FocusBox/BS_BaseBox.BS_BaseBox_C'"));
	if (PS.Succeeded())
	{
		MoveBoxClass = PS.Class;
	}
}

void APiece::ShowLegalMoves()
{
	if (!IsValid(MoveBoxClass))
		return;
	DestroyMoveBoxes();

	for (auto Location : LegalMoves)
	{
		FVector MoveBoxLocation = Location;
		MoveBoxLocation.Z = 5.f;
		MoveBoxes.Add(GetWorld()->SpawnActor<APaperSpriteActor>(
			MoveBoxClass, MoveBoxLocation, FRotator::ZeroRotator));
	}
}

// Called when the game starts or when spawned
void APiece::BeginPlay()
{
	Super::BeginPlay();
	
}

void APiece::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(PieceMesh))
	{
		PieceMesh->SetCollisionProfileName(TEXT("Piece"));
	}
}

// Called every frame
void APiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UStaticMeshComponent* APiece::GetStaticMeshComponent()
{
	if (IsValid(PieceMesh))
		return PieceMesh;
	return nullptr;
}

void APiece::UpdateLegalMoves()
{
	LegalMoves.Empty();
}

bool APiece::IsAbleToPick()
{
	// if Legal Moves Exists

	// Temp: UpdateLegalMoves
	UpdateLegalMoves();
	ShowLegalMoves();

	return true;
}

bool APiece::IsAbleToPut(FVector Dest) const
{
	// if Dest in Legal Moves
	return LegalMoves.Contains(Dest);
}

void APiece::Put(FVector Dest)
{
	UE_LOG(LogTemp, Warning, TEXT("Super: Put piece"));

	if(bIsFirstMove)
		bIsFirstMove = false;

	SetActorLocation(Dest);
	DestroyMoveBoxes();
}

FVector APiece::GetPieceFowardVector() const
{
	if (GetPieceColor() == EPieceColor::Black)
	{
		return FVector(0.f, 1.f, 0.f);
	}
	return FVector(0.f, -1.f, 0.f);
}

FVector APiece::GetPieceRightVector() const
{
	if (GetPieceColor() == EPieceColor::Black)
	{
		return FVector(-1.f, 0.f, 0.f);
	}
	return FVector(1.f, 0.f, 0.f);
}

void APiece::DestroyMoveBoxes()
{
	if (MoveBoxes.Num() != 0)
	{
		for (auto MoveBox : MoveBoxes)
		{
			MoveBox->Destroy();
		}
	}
}
