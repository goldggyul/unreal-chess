// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightPiece.h"

AKnightPiece::AKnightPiece()
{
	SetType(EPieceType::Knight);

	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KNIGHT MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/Meshes/Pieces/SM_Knight.SM_Knight'"));
	if (SM.Succeeded())
	{
		PieceMesh->SetStaticMesh(SM.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't load Knight mesh"));
	}
	SetRootComponent(PieceMesh);
	PieceMesh->SetRelativeScale3D(PieceMeshSize);
}

TSet<FVector> AKnightPiece::GetBasicMovesInCurBoard()
{
	TSet<FVector> CurMoves;

	/*
	* ����Ʈ�� �ึ�� : L�� ���. ���� ��ĭ �� ���� ��ĭ Ȥ�� ���� ��ĭ �� ���� ��ĭ
	*/

	FVector FowardVector = UChessUtil::GetPlayerForwardVector(GetPieceColor());
	FVector RightVector = UChessUtil::GetPlayerRightVector(GetPieceColor());

	TSet<FVector> Differs;
	// ���� ��ĭ �� ���� ��ĭ
	Differs.Add(FowardVector * 2 - RightVector); // �� 2 + ����
	Differs.Add(FowardVector * 2 + RightVector); // �� 2 + ������
	Differs.Add(-FowardVector * 2 - RightVector); // �Ʒ� 2 + ����
	Differs.Add(-FowardVector * 2 + RightVector); // �Ʒ� 2 + ������
	// ���� ��ĭ �� ���� ��ĭ
	Differs.Add(FowardVector - RightVector * 2); // �� + ���� 2 
	Differs.Add(FowardVector + RightVector * 2); // �� + ������ 2
	Differs.Add(-FowardVector - RightVector * 2); // �Ʒ� + ���� 2 
	Differs.Add(-FowardVector + RightVector * 2); // �Ʒ� + ������ 2

	for (auto& Differ : Differs)
	{
		FVector Cur = GetActorLocation();
		Cur += Differ;
		if (UChessUtil::IsInBoard(Cur))
		{
			AActor* HitActor = UChessUtil::GetCollidedPiece(GetWorld(), Cur);
			APiece* HitPiece = Cast<APiece>(HitActor);
			if (!IsValid(HitPiece))
			{
				CurMoves.Add(Cur);
			}
			else if (HitPiece->GetPieceColor() != GetPieceColor())
			{
				CurMoves.Add(Cur);
			}
		}
		
	}
	return CurMoves;
}
