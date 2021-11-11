// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnPiece.h"

APawnPiece::APawnPiece()
{
	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PIECE MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/Meshes/ChessPieces1/Pawn.Pawn'"));
	if (SM.Succeeded())
	{
		PieceMesh->SetStaticMesh(SM.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't load pawn mesh"));
	}
	PieceMesh->SetupAttachment(RootComponent);

}
