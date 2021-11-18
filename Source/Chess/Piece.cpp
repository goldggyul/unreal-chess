// Fill out your copyright notice in the Description page of Project Settings.


#include "Piece.h"

// Sets default values
APiece::APiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	
}

bool APiece::IsAbleToPick() const
{
	// if Legal Moves Exists
	return true;
}

bool APiece::IsAbleToPut(FVector Dest) const
{
	// 
	return true;
}
