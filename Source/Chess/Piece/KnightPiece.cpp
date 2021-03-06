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
	* 나이트의 행마법 : L자 모양. 수평 두칸 후 수직 한칸 혹은 수직 두칸 후 수평 한칸
	*/

	FVector FowardVector = UChessUtil::GetPlayerForwardVector(GetPieceColor());
	FVector RightVector = UChessUtil::GetPlayerRightVector(GetPieceColor());

	TSet<FVector> Differs;
	// 수평 두칸 후 수직 한칸
	Differs.Add(FowardVector * 2 - RightVector); // 앞 2 + 왼쪽
	Differs.Add(FowardVector * 2 + RightVector); // 앞 2 + 오른쪽
	Differs.Add(-FowardVector * 2 - RightVector); // 아래 2 + 왼쪽
	Differs.Add(-FowardVector * 2 + RightVector); // 아래 2 + 오른쪽
	// 수직 두칸 후 수평 한칸
	Differs.Add(FowardVector - RightVector * 2); // 앞 + 왼쪽 2 
	Differs.Add(FowardVector + RightVector * 2); // 앞 + 오른쪽 2
	Differs.Add(-FowardVector - RightVector * 2); // 아래 + 왼쪽 2 
	Differs.Add(-FowardVector + RightVector * 2); // 아래 + 오른쪽 2

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
