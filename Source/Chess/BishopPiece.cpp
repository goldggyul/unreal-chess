// Fill out your copyright notice in the Description page of Project Settings.


#include "BishopPiece.h"

ABishopPiece::ABishopPiece()
{
	Type = EPieceType::Bishop;

	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BISHOP MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/Meshes/Pieces/SM_Bishop.SM_Bishop'"));
	if (SM.Succeeded())
	{
		PieceMesh->SetStaticMesh(SM.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't load Bishop mesh"));
	}
	PieceMesh->SetupAttachment(RootComponent);
	PieceMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
}
