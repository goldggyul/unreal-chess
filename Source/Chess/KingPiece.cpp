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

	TSet<FVector> Differs;
	// 0 1 2
	// 3   4
	// 5 6 7
	FVector PieceForward = GetPieceFowardVector();
	FVector PieceRight = GetPieceRightVector();
	Differs.Add(PieceForward - PieceRight);
	Differs.Add(PieceForward);
	Differs.Add(PieceForward + PieceRight);
	Differs.Add(-PieceRight);
	Differs.Add(PieceRight);
	Differs.Add(-PieceForward - PieceRight);
	Differs.Add(-PieceForward);
	Differs.Add(-PieceForward + PieceRight);
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
	UE_LOG(LogTemp, Warning, TEXT("%s Legal Move Num: %d"), *GetActorLabel(), LegalMoves.Num());
}
