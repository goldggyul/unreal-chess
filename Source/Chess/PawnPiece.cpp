// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnPiece.h"
#include "DrawDebugHelpers.h"

APawnPiece::APawnPiece()
{
	SetType(EPieceType::Pawn);

	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PAWN MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/Meshes/Pieces/SM_Pawn.SM_Pawn'"));
	if (SM.Succeeded())
	{
		PieceMesh->SetStaticMesh(SM.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't load pawn mesh"));
	}
	SetRootComponent(PieceMesh);
	PieceMesh->SetRelativeScale3D(PieceMeshSize);
}

void APawnPiece::UpdateMoves()
{
	Super::UpdateMoves();

	// �ϴ� Legal Moves ���� Possible Moves��

	/*
	* ���� �ึ��: �ึ���� �⹰�� ��� ���� �ٸ���
	* 1. ���� ������ �Ѵ�
	* 2. �� ���� �������� ���� ���� �� ĭ ������ �����ϴ�. �� �ܴ� �� ĭ ����
	* 3. �⹰�� ���� ���� �밢�� ���濡 �ִ� �� ���� �� �ִ�.
	*	 �� ĭ(Ȥ�� ��ĭ) ������ ���� ���� �⹰�� �ִٸ� ���� ���ϰ� ������ ���Ѵ�.
	* 
	* Ư�� �ึ��: ���Ļ�� ���θ��
	* 1. ���Ļ�
	* �ٷ� ������ ���� ���� �� ĭ �̵������� �� ���� �ٷ� ����/������ �� �� ���� �ִٸ�,
	* �� ���� �밢������ �����ϸ鼭 ���� �⹰�� ���� �� �ִ�.
	* 2. ���θ��
	* ���� ������ ������ ��ũ�� ���� ��, ŷ�� ������ �ǽ��� �°��� �� �ִ�.
	* 
	* ����
	* 1. ���Ļ��� ���, �ܼ��� ó�� �� ĭ ������ ���� ���� �ƴ϶� '����'�� ���������� �˾ƾ��Ѵ�.
	*	1) Piece�� ���� �ֱ�? �׷� ��� ������ ������ �����
	*	2) Player�� ���� �ֱ�? ���� �ٲ� �� ������ ���� ó������ �������ٸ� ������ ����Ѵ�. �� ChangePlayer�� �� ���Ļ��� Set
	*/


	FVector FowardVector = UChessUtil::GetPlayerForwardVector(GetPieceColor());
	FVector RightVector = UChessUtil::GetPlayerRightVector(GetPieceColor());

	TSet<FVector> Differs;
	Differs.Add(FowardVector);
	if (IsFirstMove())
	{
		// ù ��° �������� ��� �� ĭ �յ� üũ
		Differs.Add(FowardVector*2);
	}
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
		else
		{
			break;
		}
	}

	// ���� �⹰ ���: �밢�� ����
	Differs.Empty();
	Differs.Add(FowardVector + RightVector);
	Differs.Add(FowardVector - RightVector);

	for (auto& Differ : Differs)
	{
		FVector Location = GetActorLocation();
		Location += Differ;
		AActor* HitActor = UChessUtil::GetCollidedPiece(GetWorld(), Location);
		APiece* HitPiece = Cast<APiece>(HitActor);
		if (IsValid(HitPiece) && (HitPiece->GetPieceColor() != GetPieceColor()))
		{
			AddToMoves(Location);
		}
	}
}
