// Fill out your copyright notice in the Description page of Project Settings.


#include "KingPiece.h"

AKingPiece::AKingPiece()
{
	SetType(EPieceType::King);

	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KING MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/Meshes/Pieces/SM_King.SM_King'"));
	if (SM.Succeeded())
	{
		PieceMesh->SetStaticMesh(SM.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't load King mesh"));
	}
	SetRootComponent(PieceMesh);
	PieceMesh->SetRelativeScale3D(PieceMeshSize);

}

void AKingPiece::UpdateLegalMoves()
{
	Super::UpdateLegalMoves();

	/*
	* ŷ�� �ึ�� : �����¿�+�밢���� �� ĭ��
	*/

	TSet<FVector> Differs;
	Differs.Add(GetPieceFowardVector()); // ��
	Differs.Add(-GetPieceFowardVector()); // ��
	Differs.Add(-GetPieceRightVector()); // ��
	Differs.Add(GetPieceRightVector()); // ��
	Differs.Add(GetPieceFowardVector() - GetPieceRightVector()); // ���� ���
	Differs.Add(GetPieceFowardVector() + GetPieceRightVector()); // ������ ���
	Differs.Add(-GetPieceFowardVector() - GetPieceRightVector()); // ���� �ϴ�
	Differs.Add(-GetPieceFowardVector() + GetPieceRightVector()); // ������ �ϴ�
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

	// ĳ����
	// ���ʰ� �������� �� ���� �ݸ����� �Ͼ Piece�� Rook�̸� ���� �ȿ���������
	// TODO: ŷ�� üũ�� �ƴ� ��
	if (IsFirstMove())
	{
		Differs.Empty();
		Differs.Add(GetPieceRightVector());
		Differs.Add(-GetPieceRightVector());
		for (auto Differ : Differs)
		{
			UE_LOG(LogTemp, Warning, TEXT("Checking castling..."));

			FVector Start = GetActorLocation();
			FVector End = Start + Differ * 4; // ���� 3ĭ Ȥ�� 4ĭ ����������

			DrawDebugLine(
				GetWorld(), Start, End,
				FColor::Orange, false, 1.f, 0.f, 20.f
			);

			// Line Trace
			FHitResult HitResult;
			FCollisionObjectQueryParams ObjectQuery;
			ObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
			FCollisionQueryParams Query = FCollisionQueryParams::DefaultQueryParam;
			Query.AddIgnoredActor(GetOwner());

			GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, ObjectQuery, Query);

			APiece* HitPiece = Cast<APiece>(HitResult.GetActor());
			if (IsValid(HitPiece)
				&& HitPiece->GetPieceType() == EPieceType::Rook
				&& HitPiece->IsFirstMove())
			{
				UE_LOG(LogTemp, Warning, TEXT("Able to castling!"));
				AddToLegalMoves(HitPiece->GetActorLocation() - Differ);
			}
		}
	}
}
