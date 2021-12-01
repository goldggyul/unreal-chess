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

TSet<FVector> AQueenPiece::GetBasicMovesInCurBoard()
{
	TSet<FVector> CurMoves;

	/*
	* ���� �ึ�� : �����¿�+�밢���� ����
	*/

	FVector FowardVector = UChessUtil::GetPlayerForwardVector(GetPieceColor());
	FVector RightVector = UChessUtil::GetPlayerRightVector(GetPieceColor());

	TSet<FVector> Differs;
	Differs.Add(FowardVector); // ��
	Differs.Add(-FowardVector); // ��
	Differs.Add(-RightVector); // ��
	Differs.Add(RightVector); // ��
	Differs.Add(FowardVector - RightVector); // ���� ���
	Differs.Add(FowardVector + RightVector); // ������ ���
	Differs.Add(-FowardVector - RightVector); // ���� �ϴ�
	Differs.Add(-FowardVector + RightVector); // ������ �ϴ�

	for (auto Differ : Differs)
	{
		FVector Location = GetActorLocation();
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