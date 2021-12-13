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

TSet<FVector> ABishopPiece::GetBasicMovesInCurBoard()
{
	TSet<FVector> CurMoves;
	/*
	* ����� �ึ��
	* �밢�� ���� ����
	*/

	FVector FowardVector = UChessUtil::GetPlayerForwardVector(GetPieceColor());
	FVector RightVector = UChessUtil::GetPlayerRightVector(GetPieceColor());

	TSet<FVector> Differs;
	Differs.Add(FowardVector - RightVector); // ���� ���
	Differs.Add(FowardVector + RightVector); // ������ ���
	Differs.Add(-FowardVector - RightVector); // ���� �ϴ�
	Differs.Add(-FowardVector + RightVector); // ������ �ϴ�

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
