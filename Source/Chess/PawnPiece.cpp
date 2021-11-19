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
	PieceMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
}

void APawnPiece::UpdateLegalMoves()
{
	Super::UpdateLegalMoves();

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

	// Test: �ϴ� �ڱⲨ�� ���
	FVector Start = GetActorLocation();
	FVector Differ = GetPieceFowardVector() * SquareSize;
	// �� ĭ ��
	Start += Differ;
	AActor* HitActor = ChessUtil::GetCollidedPiece(GetWorld(), Start);
	APiece* HitPiece = Cast<APiece>(HitActor);
	if (!IsValid(HitPiece))
	{
		LegalMoves.Add(Start);
	}
	// ù ��° �������� ��� �� ĭ ��
	if (IsFirstMove())
	{
		Start += Differ;
		HitActor = ChessUtil::GetCollidedPiece(GetWorld(), Start);
		HitPiece = Cast<APiece>(HitActor);

		if (!IsValid(HitPiece))
		{
			LegalMoves.Add(Start);
		}
	}
	// ���� �⹰ ���: �밢�� ����
	// 1. ������ �밢��
	Start = GetActorLocation() + GetPieceFowardVector() * SquareSize + GetPieceRightVector() * SquareSize;
	HitActor = ChessUtil::GetCollidedPiece(GetWorld(), Start);
	HitPiece = Cast<APiece>(HitActor);
	if (IsValid(HitPiece) && (HitPiece->GetPieceColor() != GetPieceColor()))
	{
		LegalMoves.Add(Start);

	}

	// 1. ���� �밢��
	Start = GetActorLocation() + GetPieceFowardVector() * SquareSize - GetPieceRightVector() * SquareSize;
	HitActor = ChessUtil::GetCollidedPiece(GetWorld(), Start);
	HitPiece = Cast<APiece>(HitActor);
	if (IsValid(HitPiece) && (HitPiece->GetPieceColor() != GetPieceColor()))
	{
		LegalMoves.Add(Start);
	}
}
