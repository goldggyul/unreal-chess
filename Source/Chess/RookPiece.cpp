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
	PieceMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
}

void ARookPiece::UpdateLegalMoves()
{
	Super::UpdateLegalMoves();

	/*
	* ���� �ึ�� : �����¿� ����
	* �� ĳ���� �� ���� ����������: ŷ���� �����̰� �� ����
	*/
	
	TSet<FVector> Differs;
	Differs.Add(GetPieceFowardVector()); // ��
	Differs.Add(-GetPieceFowardVector()); // ��
	Differs.Add(-GetPieceRightVector()); // ��
	Differs.Add(GetPieceRightVector()); // ��
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
