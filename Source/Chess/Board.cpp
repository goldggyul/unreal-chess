// Fill out your copyright notice in the Description page of Project Settings.


#include "Board.h"

const uint8 BoardSize = 8; // ���� ũ��: 8 * 8

// Sets default values
ABoard::ABoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* mesh ���� */
	// TODO: mesh ��� ����
	BoardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOARD MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/Meshes/ChessPieces1/Floor.Floor'"));
	if (SM.Succeeded())
	{
		BoardMesh->SetStaticMesh(SM.Object);
	}
	BoardMesh->SetupAttachment(RootComponent);
	// TODO: Scale Setting
	BoardMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));

	/* Init Board State */
	BoardState.Init(EPieceColor::Empty, BoardSize * BoardSize);
	// Init Black Pieces
	// ���� ������ 0��
	for (int i = 0; i < 2 * BoardSize; i++)
	{
		BoardState[i] = EPieceColor::Black;
		BoardState[BoardSize * BoardSize - i - 1] = EPieceColor::White;
	}
}

void ABoard::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

