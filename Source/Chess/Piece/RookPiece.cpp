// Fill out your copyright notice in the Description page of Project Settings.


#include "RookPiece.h"

ARookPiece::ARookPiece()
{
	SetType(EPieceType::Rook);

	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ROOK MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/Meshes/Pieces/SM_Rook.SM_Rook'"));
	if (SM.Succeeded())
	{
		PieceMesh->SetStaticMesh(SM.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't load rook mesh"));
	}
	SetRootComponent(PieceMesh);
	PieceMesh->SetRelativeScale3D(PieceMeshSize);
}

TSet<FVector> ARookPiece::GetBasicMovesInCurBoard()
{
	TSet<FVector> CurMoves;

	/*
	* 룩의 행마법 : 상하좌우 직진
	* ※ 캐슬링 시 같이 움직여야함: 킹에서 움직이게 할 예정
	*/

	FVector FowardVector = UChessUtil::GetPlayerForwardVector(GetPieceColor());
	FVector RightVector = UChessUtil::GetPlayerRightVector(GetPieceColor());

	TSet<FVector> Differs;
	Differs.Add(FowardVector); // 상
	Differs.Add(-FowardVector); // 하
	Differs.Add(-RightVector); // 좌
	Differs.Add(RightVector); // 우

	for (auto& Differ : Differs)
	{
		for (FVector Cur = GetActorLocation() + Differ; UChessUtil::IsInBoard(Cur); Cur += Differ)
		{
			AActor* HitActor = UChessUtil::GetCollidedPiece(GetWorld(), Cur);
			APiece* HitPiece = Cast<APiece>(HitActor);
			if (!IsValid(HitPiece))
			{
				CurMoves.Add(Cur);
			}
			else
			{
				if (HitPiece->GetPieceColor() != GetPieceColor())
				{
					CurMoves.Add(Cur);
				}
				break;
			}
		}
	}
	return CurMoves;
}