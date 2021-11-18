// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPlayer.h"
#include "Camera/CameraComponent.h"
#include "PaperSpriteActor.h"
#include "Piece.h"
#include "DrawDebugHelpers.h"
#include "ChessUtil.h"
#include "Engine/StaticMeshActor.h"

// Sets default values
AChessPlayer::AChessPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	//Camera->SetupAttachment(RootComponent);
	SetRootComponent(Camera);

	static ConstructorHelpers::FObjectFinder<UMaterial> DM(TEXT("Material'/Game/Materials/M_Dark.M_Dark'"));
	if (DM.Succeeded())
	{
		DarkMaterial = DM.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> LM(TEXT("Material'/Game/Materials/M_Light.M_Light'"));
	if (LM.Succeeded())
	{
		LightMaterial = LM.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> DMI(TEXT("MaterialInstanceConstant'/Game/Materials/MI_Dark_T.MI_Dark_T'"));
	if (DMI.Succeeded())
	{
		TranslucentDarkMaterial = DMI.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> LMI(TEXT("MaterialInstanceConstant'/Game/Materials/MI_Light_T.MI_Light_T'"));
	if (LMI.Succeeded())
	{
		TranslucentLightMaterial = LMI.Object;
	}

	static ConstructorHelpers::FClassFinder<APaperSpriteActor> PS(TEXT("Blueprint'/Game/Blueprints/FocusBox/BS_PickBox.BS_PickBox_C'"));
	if (PS.Succeeded())
	{
		PickBoxClass = PS.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot load PickBox class"));
	}
	SetState(EPlayerState::Idle);
}

// Called when the game starts or when spawned
void AChessPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChessPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChessPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChessPlayer::SetPieceColor(EPieceColor Color)
{
	PlayerColor = Color;
}

EPieceColor AChessPlayer::GetPieceColor() const
{
	return PlayerColor;
}

void AChessPlayer::SetPickBoxStart(FVector BoxStart)
{
	PrevMove = BoxStart;
}

FVector AChessPlayer::GetPickBoxLocation() const
{
	if (IsValid(PickBox))
	{
		return PickBox->GetActorLocation();
	}
	return FVector::ZeroVector;
}

void AChessPlayer::SetState(EPlayerState CurState)
{
	MyState = CurState;
}

EPlayerState AChessPlayer::GetState() const
{
	return MyState;
}

void AChessPlayer::SpawnPickBox()
{
	if (!IsValid(PickBoxClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("PickBoxClass is not exist"));
		return;
	}

	if (IsValid(PickBox))
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroy Previous PickBox"));
		PickBox->Destroy();
	}
	PickBox = GetWorld()->SpawnActor<APaperSpriteActor>(PickBoxClass, PrevMove, FRotator::ZeroRotator);
	PickBox->SetActorLabel(TEXT("PickBox"));
	PickBox->SetFolderPath("/Player");
}

void AChessPlayer::DestroyPickBox()
{
	PickBox->Destroy();
}

void AChessPlayer::MovePickBox(FVector Dest)
{
	// 현재 State가 Pick이라면 Box를 움직일 때 Piece(혹은 그 복제품)도 움직여야 함

	//UE_LOG(LogTemp, Warning, TEXT("Move To (%f, %f, %f)"), Dest.X, Dest.Y, Dest.Z);

	if (Dest.X < 150.f || Dest.X >2250.f || Dest.Y < 150.f || Dest.Y>2250.f)
		return;
	if (IsValid(PickBox))
	{
		Dest.Z = PickBoxZ;
		PickBox->SetActorLocation(Dest);
		//UE_LOG(LogTemp, Warning, TEXT("Box Moved"));
		PrevMove = Dest;
		if (IsValid(PickedPiece))
		{
			Dest.Z = PickedPiece->GetActorLocation().Z;
			PickedPiece->SetActorLocation(Dest);
		}
	}
}

void AChessPlayer::PickPiece()
{
	// 선택할 수 있으면 들기

	// 현재 선택된 피스 저장
	CurPiece = GetCurPiece();
	if (IsValid(CurPiece))// Piece(Black or white) is here
	{
		// Do Something
		if (PlayerColor == CurPiece->GetColor())
		{
			if (CurPiece->IsAbleToPick())
			{
				SetState(EPlayerState::Pick);
				UE_LOG(LogTemp, Warning, TEXT("Pick this piece!"));
				SpawnPickedPiece();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot pick: It's enemy's piece!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot pick: It's empty!"));
	}
}

void AChessPlayer::SpawnPickedPiece()
{
	if (IsValid(PickedPiece))
		PickedPiece->Destroy();
	if (IsValid(CurPiece))
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Picked Piece"));

		FVector SpawnLocation = CurPiece->GetActorLocation();
		SpawnLocation.Z += 450.f;
		FRotator SpawnRotation = CurPiece->GetActorRotation();
		PickedPiece = GetWorld()->SpawnActor<AStaticMeshActor>(SpawnLocation, SpawnRotation);

		PickedPiece->SetMobility(EComponentMobility::Movable);
		PickedPiece->SetActorRelativeScale3D(FVector(2.f, 2.f, 2.f));
		PickedPiece->SetActorLabel(FString(TEXT("Picked Piece")));

		UStaticMeshComponent* CurPieceMeshComp = CurPiece->GetStaticMeshComponent();
		UStaticMeshComponent* PickedPieceMeshComp = PickedPiece->GetStaticMeshComponent();
		PickedPieceMeshComp->SetStaticMesh(CurPieceMeshComp->GetStaticMesh());
		SetMeshOpaque(false, CurPieceMeshComp);
		SetMeshOpaque(true, PickedPieceMeshComp);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot pick: It's empty!"));
	}
}

void AChessPlayer::PutPiece()
{
	// 현재 위치에 놓을 수 있으면 놓는다.
	if (!IsValid(CurPiece) || !IsValid(PickedPiece) || !IsValid(PickBox))
		return;
	FVector CurPickLocation = PickedPiece->GetActorLocation();
	CurPickLocation.Z = 0;
	if (CurPiece->IsAbleToPut(CurPickLocation))
	{
		PickedPiece->Destroy();
		CurPiece->SetActorLocation(CurPickLocation);
		SetMeshOpaque(true, CurPiece->GetStaticMeshComponent());
		CurPiece = nullptr;
		SetState(EPlayerState::Idle);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot put here!"));
	}
}

APiece* AChessPlayer::GetCurPiece()
{
	// Test: 일단 자기꺼면 든다
	FHitResult HitResult;

	FVector Start = PickBox->GetActorLocation();
	FVector End = Start;
	Start.Z += 700.f;

	// For debugging
	FVector LineStart = PickBox->GetActorLocation();
	FVector LineEnd = LineStart;
	LineEnd.Z += 700.f;
	DrawDebugLine(
		GetWorld(), LineStart, LineEnd,
		FColor::Magenta, false, 1.f, 0.f, 10.f
	);

	// Line Trace
	FCollisionObjectQueryParams Query;
	Query.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
	if (GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, Query))
	{
		APiece* HitPiece = Cast<APiece>(HitResult.GetActor());
		if (IsValid(HitPiece))
		{
			// For debugging
			FString PieceTypeName = ChessUtil::GetPieceTypeString(HitPiece->GetType());
			FString PieceColorName = ChessUtil::GetColorString(HitPiece->GetColor());
			UE_LOG(LogTemp, Warning, TEXT("[LINE TRACE] HITTED: %s %s"), *PieceColorName, *PieceTypeName);
			return HitPiece;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[LINE TRACE] NO COLLISION"));
	}
	return nullptr;
}

void AChessPlayer::SetMeshOpaque(bool bIsOpaque, class UStaticMeshComponent* Mesh) const
{
	if (!IsValid(Mesh))
		return;

	if (bIsOpaque) // 불투명
	{
		if (PlayerColor == EPieceColor::Black && IsValid(DarkMaterial))
		{
			UE_LOG(LogTemp, Warning, TEXT("Set Material: opaque dark"));
			Mesh->SetMaterial(0, DarkMaterial);
		}
		else if (PlayerColor == EPieceColor::White && IsValid(LightMaterial))
		{
			UE_LOG(LogTemp, Warning, TEXT("Set Material: opaque light"));
			Mesh->SetMaterial(0, LightMaterial);
		}
	}
	else // 투명
	{
		if (PlayerColor == EPieceColor::Black && IsValid(TranslucentDarkMaterial))
		{
			UE_LOG(LogTemp, Warning, TEXT("Set Material: translucent dark"));
			Mesh->SetMaterial(0, TranslucentDarkMaterial);
		}
		else if (PlayerColor == EPieceColor::White && IsValid(TranslucentLightMaterial))
		{
			UE_LOG(LogTemp, Warning, TEXT("Set Material: translucent light"));

			Mesh->SetMaterial(0, TranslucentLightMaterial);
		}
	}
}

