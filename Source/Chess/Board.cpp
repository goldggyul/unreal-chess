// Fill out your copyright notice in the Description page of Project Settings.


#include "Board.h"

// Sets default values
ABoard::ABoard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* mesh ¼³Á¤ */
	BoardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOARD MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/Meshes/Board/SM_WoodBoard.SM_WoodBoard'"));
	if (SM.Succeeded())
	{
		BoardMesh->SetStaticMesh(SM.Object);
	}
	SetRootComponent(BoardMesh);
	BoardMesh->SetRelativeScale3D(PieceMeshSize);
	BoardMesh->SetRelativeLocation(BoardCenter);
}