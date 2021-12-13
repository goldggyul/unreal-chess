// Fill out your copyright notice in the Description page of Project Settings.

#include "ThreatBox.h"

// Sets default values
AThreatBox::AThreatBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (SM.Succeeded())
	{
		BoxMesh->SetStaticMesh(SM.Object);
	}
	SetRootComponent(BoxMesh);

	BoxMesh->SetRelativeScale3D(FVector(2.8f, 2.8f, 2.8f));
	
	static ConstructorHelpers::FObjectFinder<UMaterial> Mat(TEXT("Material'/Game/Materials/ThreatBox/M_Threat.M_Threat'"));
	if (Mat.Succeeded())
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Mat.Object, NULL);
		BoxMesh->SetMaterial(0, DynamicMaterial);
	}

	static ConstructorHelpers::FObjectFinder<UTexture> TSM(TEXT("Texture2D'/Game/Textures/ThreatBox/T_Square.T_Square'"));
	if (TSM.Succeeded())
	{
		Square = TSM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture> HSM(TEXT("Texture2D'/Game/Textures/ThreatBox/T_HalfSquare.T_HalfSquare'"));
	if (HSM.Succeeded())
	{
		HalfSquare = HSM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture> QSM(TEXT("Texture2D'/Game/Textures/ThreatBox/T_QuarterSquare.T_QuarterSquare'"));
	if (QSM.Succeeded())
	{
		QuarterSquare = QSM.Object;
	}
}


// Called when the game starts or when spawned
void AThreatBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThreatBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AThreatBox::SetShapeAndColor(uint8 PlayerCount, uint8 EnemyCount)
{
	BoxPlayerCount = PlayerCount;
	BoxEnemyCount = EnemyCount;

	if (PlayerCount == 0 && EnemyCount == 0) // 박스 만들 필요 없음 
	{
		return;
	}
	
	// Color 
	FLinearColor EnemyColor, PlayerColor;
	if (EnemyCount == 1)
	{
		EnemyColor = OneEnemy;
	}
	else
	{
		EnemyColor = MoreEnemy;
	}
	if (PlayerCount == 1)
	{
		PlayerColor = OnePlayer;
	}
	else
	{
		PlayerColor = MorePlayer;
	}

	DynamicMaterial->SetVectorParameterValue(TEXT("TopColor"), EnemyColor);
	DynamicMaterial->SetVectorParameterValue(TEXT("BottomColor"), PlayerColor);

	// Color and Texture
	if (PlayerCount == EnemyCount)
	{
		DynamicMaterial->SetTextureParameterValue(TEXT("SquareTexture"), HalfSquare);
	}
	else if (PlayerCount == 0 || EnemyCount == 0)
	{
		if (EnemyCount == 0)
		{
			DynamicMaterial->SetVectorParameterValue(TEXT("TopColor"), PlayerColor);
		}
		DynamicMaterial->SetTextureParameterValue(TEXT("SquareTexture"), Square);
	}
	else
	{
		if (EnemyCount > PlayerCount)
		{
			DynamicMaterial->SetVectorParameterValue(TEXT("TopColor"), PlayerColor);
			DynamicMaterial->SetVectorParameterValue(TEXT("BottomColor"), EnemyColor);
			SetActorRotation(GetActorRotation() + FRotator(0.0f, 180.0f, 0.0f));
		}
		DynamicMaterial->SetTextureParameterValue(TEXT("SquareTexture"), QuarterSquare);
	}
}

