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

void AKnightPiece::UpdateLegalMoves()
{
	Super::UpdateLegalMoves();

	TSet<FVector> Differs;

	/*
	* ����Ʈ�� �ึ�� : L�� ���. ���� ��ĭ �� ���� ��ĭ Ȥ�� ���� ��ĭ �� ���� ��ĭ
	*/
	// ���� ��ĭ �� ���� ��ĭ
	Differs.Add(GetPieceFowardVector() * 2 - GetPieceRightVector()); // �� 2 + ����
	Differs.Add(GetPieceFowardVector() * 2 + GetPieceRightVector()); // �� 2 + ������
	Differs.Add(-GetPieceFowardVector() * 2 - GetPieceRightVector()); // �Ʒ� 2 + ����
	Differs.Add(-GetPieceFowardVector() * 2 + GetPieceRightVector()); // �Ʒ� 2 + ������
	// ���� ��ĭ �� ���� ��ĭ
	Differs.Add(GetPieceFowardVector() - GetPieceRightVector() * 2); // �� + ���� 2 
	Differs.Add(GetPieceFowardVector() + GetPieceRightVector() * 2); // �� + ������ 2
	Differs.Add(-GetPieceFowardVector() - GetPieceRightVector() * 2); // �Ʒ� + ���� 2 
	Differs.Add(-GetPieceFowardVector() + GetPieceRightVector() * 2); // �Ʒ� + ������ 2

	for (auto Differ : Differs)
	{
		FVector Location = GetActorLocation();
		Location += Differ;
		AActor* HitActor = UChessUtil::GetCollidedPiece(GetWorld(), Location);
		APiece* HitPiece = Cast<APiece>(HitActor);
		if (!IsValid(HitPiece))
		{
			AddToLegalMoves(Location);
		}
		else if (HitPiece->GetPieceColor() != GetPieceColor())
		{
			AddToLegalMoves(Location);
		}
	}
}
