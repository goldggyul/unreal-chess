// Fill out your copyright notice in the Description page of Project Settings.


#include "Board.h"

// Sets default values
ABoard::ABoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* mesh 설정 */
	// TODO: mesh 경로 변경
	BoardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOARD MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/Meshes/ChessPieces1/Floor.Floor'"));
	if (SM.Succeeded())
	{
		BoardMesh->SetStaticMesh(SM.Object);
	}
	BoardMesh->SetupAttachment(RootComponent);
	// TODO: Scale Setting
	//BoardMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));

	InitBoardState();
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

void ABoard::InitBoardState()
{
	/* Init Color */
	PieceColor.Init(EPieceColor::None, 8 * 8);
	// 인덱스는 왼쪽 위부터 0번
	for (int i = 0; i < 2 * 8; i++)
	{
		PieceColor[i] = EPieceColor::Black;
		PieceColor[63 - i] = EPieceColor::White;
	}
	/* Init Type */
	PieceType.Init(EPieceType::None, 8 * 8);
	// Rook
	PieceType[0] = EPieceType::Rook;
	PieceType[7] = EPieceType::Rook;
	PieceType[56] = EPieceType::Rook;
	PieceType[63] = EPieceType::Rook;
	// Knight
	PieceType[1] = EPieceType::Knight;
	PieceType[6] = EPieceType::Knight;
	PieceType[57] = EPieceType::Knight;
	PieceType[62] = EPieceType::Knight;
	// Bishop
	PieceType[2] = EPieceType::Bishop;
	PieceType[5] = EPieceType::Bishop;
	PieceType[58] = EPieceType::Bishop;
	PieceType[61] = EPieceType::Bishop;
	// Queen
	PieceType[3] = EPieceType::Queen;
	PieceType[59] = EPieceType::Queen;
	// King
	PieceType[4] = EPieceType::King;
	PieceType[58] = EPieceType::King;
	// Pawn
	for (int i = 0; i < 8; i++)
	{
		PieceType[8 + i] = EPieceType::Pawn;
		PieceType[48 + i] = EPieceType::Pawn;
	}
}


