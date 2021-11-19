// Fill out your copyright notice in the Description page of Project Settings.


#include "BishopPiece.h"

ABishopPiece::ABishopPiece()
{
	SetType(EPieceType::Bishop);

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
	SetRootComponent(PieceMesh);
	PieceMesh->SetRelativeScale3D(PieceMeshSize);
}

void ABishopPiece::UpdateLegalMoves()
{
	Super::UpdateLegalMoves();
	/*
	* 비숍의 행마법
	* 대각선 방향 직진
	*/
	TSet<FVector> Differs;
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
