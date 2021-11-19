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
		MoveBoxLocation.Z = MoveBoxZ;
		APaperSpriteActor* MoveBox = GetWorld()->SpawnActor<APaperSpriteActor>(
			MoveBoxClass, MoveBoxLocation, FRotator::ZeroRotator);
		MoveBox->SetFolderPath("/MoveBoxes");
		MoveBoxes.Add(MoveBox);
	}
}

void APiece::AddToLegalMoves(const FVector Location)
{
	if (UChessUtil::IsInBoard(Location))
	{
		LegalMoves.Add(Location);
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

	if (LegalMoves.Num() > 0)
		return true;
	UE_LOG(LogTemp, Warning, TEXT("Can't move this piece!"));
	return false;
}

bool APiece::IsAbleToPutAt(FVector Dest) const
{
	// if Dest in Legal Moves
	UE_LOG(LogTemp, Warning, TEXT("[Checking...] %f %f %f"), Dest.X, Dest.Y, Dest.Z);
	for (auto MoveLocation : LegalMoves)
	{
		if (Dest.Equals(MoveLocation, true))
		{
			return true;
		}
	}
	return false;
}

void APiece::PutAt(FVector Dest)
{
	UE_LOG(LogTemp, Warning, TEXT("Super: PutAt piece"));

	if(bIsFirstMove)
		bIsFirstMove = false;

	SetActorLocation(Dest);
	DestroyMoveBoxes();
}

FVector APiece::GetPieceFowardVector() const
{
	if (GetPieceColor() == EPieceColor::Black)
	{
		return FVector(0.0f, 1.0f, 0.0f) * SquareSize;
	}
	return FVector(0.0f, -1.0f, 0.0f) * SquareSize;
}

FVector APiece::GetPieceRightVector() const
{
	if (GetPieceColor() == EPieceColor::Black)
	{
		return FVector(-1.0f, 0.0f, 0.0f) * SquareSize;
	}
	return FVector(1.0f, 0.0f, 0.0f) * SquareSize;
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
