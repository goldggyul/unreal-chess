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
	FHitResult HitResult;

	FVector Start = GetActorLocation();
	FVector End = Start;
	FVector Differ = GetPieceFowardVector() * SquareSize;

	// Line Trace
	FCollisionObjectQueryParams ObjectQuery;
	ObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
	FCollisionQueryParams Query = FCollisionQueryParams::DefaultQueryParam;
	Query.AddIgnoredActor(GetOwner());
	
	/*
	���� ����� �ε��� ���͸� ����
	�浹 ����: �׳� ����
	�浹 ����: �� Differ������ Ȯ�� -> �� �浹ĭ ������ �� LegalMoves�� �ְ�,
	���� ��쿣 ���� 
	*/

	// �� ĭ ��
	End += Differ;
	DrawDebugLine(
		GetWorld(), Start, End,
		FColor::Orange, false, 1.f, 0.f, 10.f
	);

	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, ObjectQuery, Query);
	APiece* HitPiece = Cast<APiece>(HitResult.GetActor());
	if (!IsValid(HitPiece))
	{
		LegalMoves.Add(End);
	}

	// ù ��° �������� ��� �� ĭ ��
	if (IsFirstMove())
	{
		End += Differ;
		DrawDebugLine(
			GetWorld(), Start, End,
			FColor::Orange, false, 1.f, 0.f, 10.f
		);
		GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, ObjectQuery, Query);
		HitPiece = Cast<APiece>(HitResult.GetActor());
		if (!IsValid(HitPiece))
		{
			LegalMoves.Add(End);
		}
	}

	// ���� �⹰ ���: �밢�� ����
	// 1. ������ �밢��
	End  = Start + GetPieceFowardVector() * SquareSize + GetPieceRightVector() * SquareSize;
	DrawDebugLine(
		GetWorld(), Start, End,
		FColor::Orange, false, 1.f, 0.f, 10.f
	);
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, ObjectQuery, Query);
	if (HitResult.bBlockingHit)
	{
		HitPiece = Cast<APiece>(HitResult.GetActor());
		if (HitPiece->GetPieceColor() != GetPieceColor()) // ���� �⹰ -> ���� �� ����
		{
			LegalMoves.Add(End);
		}
	}

	// 1. ���� �밢��
	End = Start + GetPieceFowardVector() * SquareSize - GetPieceRightVector() * SquareSize;
	DrawDebugLine(
		GetWorld(), Start, End,
		FColor::Orange, false, 1.f, 0.f, 10.f
	);
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, ObjectQuery, Query);
	if (HitResult.bBlockingHit)
	{
		HitPiece = Cast<APiece>(HitResult.GetActor());
		if (HitPiece->GetPieceColor() != GetPieceColor()) // ���� �⹰ -> ���� �� ����
		{
			LegalMoves.Add(End);
		}
	}
}
