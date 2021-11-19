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
	* ����� �ึ��
	* �밢�� ���� ����
	*/
	TSet<FVector> Differs;
	Differs.Add(GetPieceFowardVector() - GetPieceRightVector()); // ���� ���
	Differs.Add(GetPieceFowardVector() + GetPieceRightVector()); // ������ ���
	Differs.Add(-GetPieceFowardVector() - GetPieceRightVector()); // ���� �ϴ�
	Differs.Add(-GetPieceFowardVector() + GetPieceRightVector()); // ������ �ϴ�
	for (auto Differ : Differs)
	{
		FVector Location = GetActorLocation();
		for (int i = 0; i < 7; i++) // �ѹ��� �ִ� 7ĭ
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
