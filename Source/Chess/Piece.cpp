// Fill out your copyright notice in the Description page of Project Settings.


#include "Piece.h"
#include "PaperSpriteActor.h"
#include "Kismet/GameplayStatics.h"

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

void APiece::ShowMoves()
{
	if (!IsValid(MoveBoxClass))
		return;
	DestroyMoveBoxes();

	for (auto& Location : Moves)
	{
		FVector MoveBoxLocation = Location;
		MoveBoxLocation.Z = MoveBoxZ;
		APaperSpriteActor* MoveBox = GetWorld()->SpawnActor<APaperSpriteActor>(
			MoveBoxClass, MoveBoxLocation, FRotator::ZeroRotator);
		MoveBox->SetFolderPath("/MoveBoxes");
		MoveBoxes.Add(MoveBox);
	}
}

void APiece::AddToMoves(const FVector Location)
{
	if (UChessUtil::IsInBoard(Location))
	{
		Moves.Add(Location);
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

void APiece::UpdateBasicMoves()
{
	Moves.Empty();

	TSet<FVector> CurMoves = GetBasicMovesInCurBoard();
	for (const auto& Move : CurMoves)
	{
		AddToMoves(Move);
	}
}

TSet<FVector> APiece::GetBasicMovesInCurBoard()
{
	return TSet<FVector>();
}

void APiece::UpdateSpecialMoves(TSet<APiece*>& EnemyPieces)
{

}

void APiece::RemoveMoveKingCheckedByEnemies(APiece* MyKing, TSet<APiece*>& EnemyPieces)
{
	FVector OriginalLocation = GetActorLocation();

	// 임시로 둬보면서 킹이 체크인지 확인
	if (!IsValid(MyKing)) return;
		
	for (auto It = Moves.CreateConstIterator(); It; ++It)
	{
		SetActorLocation(*It);

		// 킹이 체크면 Remove
		FVector MyKingLocation = MyKing->GetActorLocation();

		for (auto& EnemyPiece : EnemyPieces)
		{
			if (!IsValid(EnemyPiece)) continue;
			if (EnemyPiece->GetActorLocation() == *It) continue; // 현재 피스가 적 피스 잡는 경우
			TSet<FVector> CurEnemyMoves = EnemyPiece->GetBasicMovesInCurBoard();
			if (CurEnemyMoves.Contains(MyKingLocation))
			{
				Moves.Remove(*It);
				break;
			}
		}
	}
	// Original moves
	SetActorLocation(OriginalLocation);
}

bool APiece::CanMoveTo(FVector Location)
{
	return Moves.Contains(Location);
}

bool APiece::IsAbleToPick()
{
	// if Legal Moves Exists
	//ShowMoves();

	if (Moves.Num() > 0)
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

	for (auto& MoveLocation : Moves)
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
	for (auto& MoveBox : MoveBoxes)
	{
		MoveBox->Destroy();
	}
	MoveBoxes.Empty();
}