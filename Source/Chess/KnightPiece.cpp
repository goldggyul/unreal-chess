// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightPiece.h"

AKnightPiece::AKnightPiece()
{
	Type = EPieceType::Knight;

	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KNIGHT MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/Meshes/Pieces/SM_Knight.SM_Knight'"));
	if (SM.Succeeded())
	{
		PieceMesh->SetStaticMesh(SM.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't load Knight mesh"));
	}
	PieceMesh->SetupAttachment(RootComponent);
	PieceMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
}
