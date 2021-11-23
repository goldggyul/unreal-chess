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
	PieceMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
}

void AKnightPiece::UpdateMoves()
{
	Super::UpdateMoves();

	FVector FowardVector = UChessUtil::GetPlayerForwardVector(GetPieceColor());
	FVector RightVector = UChessUtil::GetPlayerRightVector(GetPieceColor());

	/*
	* ����Ʈ�� �ึ�� : L�� ���. ���� ��ĭ �� ���� ��ĭ Ȥ�� ���� ��ĭ �� ���� ��ĭ
	*/

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
		FVector Location = GetActorLocation();
		Location += Differ;
		AActor* HitActor = UChessUtil::GetCollidedPiece(GetWorld(), Location);
		APiece* HitPiece = Cast<APiece>(HitActor);
		if (!IsValid(HitPiece))
		{
			AddToMoves(Location);
		}
		else if (HitPiece->GetPieceColor() != GetPieceColor())
		{
			AddToMoves(Location);
		}
	}
}
