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

void AQueenPiece::UpdateBasicMoves()
{
	Super::UpdateBasicMoves();

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
		for (int i = 0; i < 7; i++) // �ѹ��� �ִ� 7ĭ
		{
			Location += Differ;
			AActor* HitActor = UChessUtil::GetCollidedPiece(GetWorld(), Location);
			APiece* HitPiece = Cast<APiece>(HitActor);
			if (!IsValid(HitPiece))
			{
				AddToMoves(Location);
			}
			else
			{
				if (HitPiece->GetPieceColor() != GetPieceColor())
				{
					AddToMoves(Location);
				}
				break;
			}
		}
	}
}
