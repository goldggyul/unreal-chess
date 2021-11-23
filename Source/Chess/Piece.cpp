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

void APiece::RemoveCheckMoves()
{
	FVector OriginalLocation = GetActorLocation();
	for (auto Move : LegalMoves)
	{
		//LegalMoves.Remove(Move);
		SetActorLocation(Move);

		// 킹이 체크면 Remove
		

	}
	SetActorLocation(OriginalLocation);
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
	
	if (GetActorLocation() == Dest)
		return true;

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

	bool bIsOtherLocation = (GetActorLocation() != Dest); // 제자리가 아닌 유효한 곳에 놨는지
	if(bIsOtherLocation && bIsFirstMove)
		bIsFirstMove = false;

	AActor* HitActor = UChessUtil::GetCollidedPiece(GetWorld(), Dest);
	APiece* HitPiece = Cast<APiece>(HitActor);
	if (IsValid(HitPiece) && HitPiece->GetPieceColor() != GetPieceColor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Catch piece!"));
		HitPiece->Destroy();
	}
	SetActorLocation(Dest);
	DestroyMoveBoxes();
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
	MoveBoxes.Empty();
}
