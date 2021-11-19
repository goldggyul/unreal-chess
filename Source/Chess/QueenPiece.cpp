// Fill out your copyright notice in the Description page of Project Settings.


#include "QueenPiece.h"

AQueenPiece::AQueenPiece()
{
	SetType(EPieceType::Queen);

	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("QUEEN MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/Meshes/Pieces/SM_Queen.SM_Queen'"));
	if (SM.Succeeded())
	{
		PieceMesh->SetStaticMesh(SM.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't load Queen mesh"));
	}
	SetRootComponent(PieceMesh);
	PieceMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
}

void AQueenPiece::UpdateLegalMoves()
{
	Super::UpdateLegalMoves();

	/*
	* 퀸의 행마법 : 상하좌우+대각방향 직진
	*/

	TSet<FVector> Differs;
	Differs.Add(GetPieceFowardVector()); // 상
	Differs.Add(-GetPieceFowardVector()); // 하
	Differs.Add(-GetPieceRightVector()); // 좌
	Differs.Add(GetPieceRightVector()); // 우
	Differs.Add(GetPieceFowardVector() - GetPieceRightVector()); // 왼쪽 상단
	Differs.Add(GetPieceFowardVector() + GetPieceRightVector()); // 오른쪽 상단
	Differs.Add(-GetPieceFowardVector() - GetPieceRightVector()); // 왼쪽 하단
	Differs.Add(-GetPieceFowardVector() + GetPieceRightVector()); // 오른쪽 하단
	for (auto Differ : Differs)
	{
		FVector Location = GetActorLocation();
		for (int i = 0; i < 7; i++) // 한번에 최대 7칸
		{
			Location += Differ;
			AActor* HitActor = UChessUtil::GetCollidedPiece(GetWorld(), Location);
			APiece* HitPiece = Cast<APiece>(HitActor);
			if (!IsValid(HitPiece))
			{
				AddToLegalMoves(Location);
			}
			else
			{
				if (HitPiece->GetPieceColor() != GetPieceColor())
				{
					AddToLegalMoves(Location);
				}
				break;
			}
		}
	}
}
