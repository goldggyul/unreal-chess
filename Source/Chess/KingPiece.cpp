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
	* 킹의 행마법 : 상하좌우+대각방향 한 칸만
	*/

	TSet<FVector> Differs;
	Differs.Add(GetPieceFowardVector()); // 상
	Differs.Add(-GetPieceFowardVector()); // 하
	Differs.Add(-GetPieceRightVector()); // 좌
	Differs.Add(GetPieceRightVector()); // 우
	Differs.Add(GetPieceFowardVector() - GetPieceRightVector()); // 왼쪽 상단
	Differs.Add(GetPieceFowardVector() + GetPieceRightVector()); // 오른쪽 상단
	Differs.Add(-GetPieceFowardVector() - GetPieceRightVector()); // 왼쪽 하단
	Differs.Add(-GetPieceFowardVector() + GetPieceRightVector()); // 오른쪽 하단
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

	// 캐슬링
	// 왼쪽과 오른쪽을 쭉 보고 콜리전이 일어난 Piece가 Rook이며 아직 안움직였을때
	// TODO: 킹이 체크가 아닐 때
	if (IsFirstMove())
	{
		Differs.Empty();
		Differs.Add(GetPieceRightVector());
		Differs.Add(-GetPieceRightVector());
		for (auto Differ : Differs)
		{
			UE_LOG(LogTemp, Warning, TEXT("Checking castling..."));

			FVector Start = GetActorLocation();
			FVector End = Start + Differ * 4; // 룩이 3칸 혹은 4칸 떨어져있음

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
