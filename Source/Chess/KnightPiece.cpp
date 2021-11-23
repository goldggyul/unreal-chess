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
	* 나이트의 행마법 : L자 모양. 수평 두칸 후 수직 한칸 혹은 수직 두칸 후 수평 한칸
	*/

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
