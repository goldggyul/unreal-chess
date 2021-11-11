// Fill out your copyright notice in the Description page of Project Settings.


#include "Piece.h"

// Sets default values
APiece::APiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Load White Piece Material
	static ConstructorHelpers::FObjectFinder<UMaterial> LM(TEXT("Material'/Game/Materials/M_LightMaterial.M_LightMaterial'"));
	if (LM.Succeeded())
	{
		LightMaterial = LM.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Light Material isn't exist"));
	}

	// Load Black Piece Material
	static ConstructorHelpers::FObjectFinder<UMaterial> DM(TEXT("Material'/Game/Materials/M_DarkMaterial.M_DarkMaterial'"));
	if (DM.Succeeded())
	{
		DarkMaterial = DM.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Dark Material isn't exist"));
	}
}

// Called when the game starts or when spawned
void APiece::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APiece::SetColor(EPieceColor PieceColor)
{
	Color = PieceColor;

	// Piece 생성자에서 Mesh 초기화하므로 항상 Valid여야함
	if (IsValid(PieceMesh))
	{
		switch (Color)
		{
		case EPieceColor::White:
			PieceMesh->SetMaterial(0, LightMaterial);
			break;
		case EPieceColor::Black:
			PieceMesh->SetMaterial(0, DarkMaterial);
			break;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no mesh. Can't set color"));
	}
}

void APiece::UpdateLegalMoves()
{
}

