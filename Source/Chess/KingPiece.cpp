// Fill out your copyright notice in the Description page of Project Settings.


#include "KingPiece.h"
#include "Kismet/GameplayStatics.h"

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

TSet<FVector> AKingPiece::GetBasicMovesInCurBoard()
{
	TSet<FVector> CurMoves;
	/*
	* 킹의 행마법 : 상하좌우+대각방향 한 칸만
	*/

	FVector FowardVector = UChessUtil::GetPlayerForwardVector(GetPieceColor());
	FVector RightVector = UChessUtil::GetPlayerRightVector(GetPieceColor());

	TSet<FVector> Differs;
	Differs.Add(FowardVector); // 상
	Differs.Add(-FowardVector); // 하
	Differs.Add(-RightVector); // 좌
	Differs.Add(RightVector); // 우
	Differs.Add(FowardVector - RightVector); // 왼쪽 상단
	Differs.Add(FowardVector + RightVector); // 오른쪽 상단
	Differs.Add(-FowardVector - RightVector); // 왼쪽 하단
	Differs.Add(-FowardVector + RightVector); // 오른쪽 하단

	for (auto& Differ : Differs)
	{
		FVector Location = GetActorLocation();
		Location += Differ;
		if (UChessUtil::IsInBoard(Location))
		{
			AActor* HitActor = UChessUtil::GetCollidedPiece(GetWorld(), Location);
			APiece* HitPiece = Cast<APiece>(HitActor);
			if (!IsValid(HitPiece))
			{
				CurMoves.Add(Location);
			}
			else if (HitPiece->GetPieceColor() != GetPieceColor())
			{
				CurMoves.Add(Location);
			}
		}
	}
	return CurMoves;
}

void AKingPiece::UpdateSpecialMoves(TSet<APiece*>& EnemyPieces)
{
	// 캐슬링
	if (!IsFirstMove()) return;

	FVector RightVector = UChessUtil::GetPlayerRightVector(GetPieceColor());

	TSet<FVector> Differs;
	Differs.Add(RightVector);
	Differs.Add(-RightVector);
	for (auto& Differ : Differs)
	{
		for (FVector Cur = GetActorLocation() + Differ; UChessUtil::IsInBoard(Cur); Cur += Differ)
		{
			if (IsDestInThreatByEnemy(Cur, EnemyPieces)) break;

			AActor* HitActor = UChessUtil::GetCollidedPiece(GetWorld(), Cur);
			APiece* HitPiece = Cast<APiece>(HitActor);
			if (IsValid(HitPiece))
			{
				if (HitPiece->GetPieceColor() == GetPieceColor()
					&& HitPiece->GetPieceType() == EPieceType::Rook
					&& HitPiece->IsFirstMove()) // Only here, Add to moves
				{
					UE_LOG(LogTemp, Warning, TEXT("%s Able to castling!"), *UChessUtil::GetColorString(GetPieceColor()));
					AddToMoves(Cur - Differ);
				}
				break;
			}
		}
	}
}

bool AKingPiece::IsDestInThreatByEnemy(FVector Dest, TSet<APiece*>& EnemyPieces) const
{
	for (auto& EnemyPiece : EnemyPieces)
	{
		if (IsValid(EnemyPiece) && EnemyPiece->CanMoveTo(Dest))
		{
			return true;
		}
	}
	return false;
}
