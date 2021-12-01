// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PaperSpriteActor.h"
#include "Piece.h"
#include "DrawDebugHelpers.h"
#include "ChessUtil.h"
#include "ThreatMap.h"
#include "Engine/StaticMeshActor.h"
#include "PieceInfoWidget.h"
#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AChessPlayer::AChessPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SetRootComponent(SpringArm);
	Camera->SetupAttachment(RootComponent);

	SpringArm->TargetArmLength = 2600.0f;
	SpringArm->SetRelativeRotation(FRotator(-75.0f, 0.0f, 0.0f));
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));

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

	static ConstructorHelpers::FObjectFinder<USoundWave> Pick(TEXT("SoundWave'/Game/Sounds/ChessPickSound.ChessPickSound'"));
	if (Pick.Succeeded())
	{
		PickSound = Pick.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> Put(TEXT("SoundWave'/Game/Sounds/ChessPutSound.ChessPutSound'"));
	if (Put.Succeeded())
	{
		PutSound = Put.Object;
	}

	SetPicking(false);

	bIsThreatMapVisible = false;

	ThreatMap = CreateDefaultSubobject<UThreatMap>(TEXT("THREAT MAP"));
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

void AChessPlayer::SetPlayerColor(EPieceColor Color)
{
	PlayerColor = Color;
	ThreatMap->SetPlayerColor(Color);
}

EPieceColor AChessPlayer::GetPlayerColor() const
{
	return PlayerColor;
}

void AChessPlayer::SetPickBoxStart(FVector BoxStart)
{
	CurMove = BoxStart;
}

FVector AChessPlayer::GetPickBoxLocation() const
{
	if (IsValid(PickBox))
	{
		return PickBox->GetActorLocation();
	}
	return FVector::ZeroVector;
}

void AChessPlayer::SetPicking(bool Picking)
{
	bIsPicking = Picking;
}

bool AChessPlayer::IsPicking() const
{
	return bIsPicking;
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
	PickBox = GetWorld()->SpawnActor<APaperSpriteActor>(PickBoxClass, CurMove, FRotator::ZeroRotator);
	PickBox->SetActorLabel(TEXT("PickBox"));
	PickBox->SetFolderPath("/Player");
}

void AChessPlayer::DestroyPickBox()
{
	if (IsValid(PickBox))
	{
		PickBox->Destroy();
	}
}

void AChessPlayer::MovePickBox(FVector Dest)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC->InputEnabled()) return;

	APiece* CurPiece = GetPieceAtPickBox();
	if (IsValid(CurPiece))
	{
		CurPiece->DestroyMoveBoxes();
	}

	if (!UChessUtil::IsInBoard(Dest))
	{
		return;
	}
	if (IsValid(PickBox))
	{
		Dest.Z = PickBoxZ;
		PickBox->SetActorLocation(Dest);
		if (IsPicking())
		{
			Dest.Z = PickedMesh->GetActorLocation().Z;
			PickedMesh->SetActorLocation(Dest);
			CurPickedPiece->ShowMoves();
		}
		else
		{
			APiece* NextPiece = GetPieceAtPickBox();
			if (IsValid(NextPiece) && NextPiece->GetPieceColor() == GetPlayerColor())
			{
				NextPiece->ShowMoves();
			}
		}
	}
}

void AChessPlayer::PickPiece()
{
	// 선택할 수 있으면 들기

	// 현재 선택된 피스 저장
	CurPickedPiece = GetPieceAtPickBox();
	if (IsValid(CurPickedPiece))// Piece(Black or white) is here
	{
		// Do Something
		if (PlayerColor == CurPickedPiece->GetPieceColor())
		{
			if (CurPickedPiece->IsAbleToPick())
			{
				UE_LOG(LogTemp, Warning, TEXT("Pick this piece!"));
				SpawnPickedPiece();
				SetPicking(true);
				OnPickPiece.Broadcast(CurPickedPiece->GetPieceType()); // For UI
				UGameplayStatics::PlaySound2D(this, PickSound, 5.0f);
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
	if (IsValid(PickedMesh))
		PickedMesh->Destroy();
	if (IsValid(CurPickedPiece))
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Picked Piece"));

		FVector SpawnLocation = CurPickedPiece->GetActorLocation();
		SpawnLocation.Z += PickedPieceZ;
		FRotator SpawnRotation = CurPickedPiece->GetActorRotation();
		PickedMesh = GetWorld()->SpawnActor<AStaticMeshActor>(SpawnLocation, SpawnRotation);

		PickedMesh->SetMobility(EComponentMobility::Movable);
		PickedMesh->SetActorRelativeScale3D(PieceMeshSize);
		PickedMesh->SetActorLabel(FString(TEXT("Picked Piece")));
		PickedMesh->GetStaticMeshComponent()->SetCollisionProfileName(
			CurPickedPiece->GetStaticMeshComponent()->GetCollisionProfileName()
		);
		PickedMesh->SetFolderPath("/Player");

		UStaticMeshComponent* CurPieceMeshComp = CurPickedPiece->GetStaticMeshComponent();
		UStaticMeshComponent* PickedPieceMeshComp = PickedMesh->GetStaticMeshComponent();
		PickedPieceMeshComp->SetStaticMesh(CurPieceMeshComp->GetStaticMesh());
		SetMeshOpaque(false, CurPieceMeshComp);
		SetMeshOpaque(true, PickedPieceMeshComp);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot pick: It's empty!"));
	}
}

bool AChessPlayer::PutCurPiece()
{
	// 현재 위치에 놓을 수 있으면 놓는다.
	if (!IsValid(CurPickedPiece) || !IsValid(PickedMesh) || !IsValid(PickBox))
		return false;
	FVector CurPickLocation = PickedMesh->GetActorLocation();
	CurPickLocation.Z = 0;
	if (CurPickedPiece->IsAbleToPutAt(CurPickLocation))
	{
		// 입력 막기
		bool bIsOtherLocation = (CurPickedPiece->GetActorLocation() != CurPickLocation); // 제자리가 아닌 유효한 곳에 놨는지
		PickedMesh->Destroy();
		CurPickedPiece->PutAt(CurPickLocation);
		SetMeshOpaque(true, CurPickedPiece->GetStaticMeshComponent());
		SetPicking(false);
		OnPutPiece.Broadcast(); // For UI
		UGameplayStatics::PlaySound2D(this, PutSound, 5.0f);

		// If Special Move
		if (CurPickedPiece->IsSpecialMove(CurPickLocation))
		{
			CurPickedPiece->DoSpecialMove();

			if (CurPickedPiece->GetPieceType() == EPieceType::King)
			{
				// Castling
				UGameplayStatics::PlaySound2D(this, PutSound, 5.0f);
			}
			else if (CurPickedPiece->GetPieceType() == EPieceType::Pawn)
			{
				// En passant

				// Promotion
			}
			
		}
		
		CurPickedPiece = nullptr;

		return bIsOtherLocation;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot put here!"));
		return false;
	}
}

void AChessPlayer::UpdateThreatMap()
{
	if (IsValid(ThreatMap))
	{
		ThreatMap->UpdateMap();
		// if Checkmate.. Check.. Stalemate... : ShowMap UI
		if (ThreatMap->IsCheckmate())
		{
			UE_LOG(LogTemp, Error, TEXT("CHECKMATE"));
			//PieceInfoWidget->ShowResult(FString(TEXT("Checkmate")));
		}
		else if (ThreatMap->IsStalemate())
		{
			UE_LOG(LogTemp, Error, TEXT("STALEMATE"));
			//PieceInfoWidget->ShowResult(FString(TEXT("Stalemate")));
		}
		else if (ThreatMap->IsCheck())
		{
			UE_LOG(LogTemp, Error, TEXT("CHECK"));
			//PieceInfoWidget->ShowResult(FString(TEXT("Check")));
		}
	}
}

APiece* AChessPlayer::GetPieceAtPickBox()
{
	// Test: 일단 자기꺼면 든다
	FVector Start = PickBox->GetActorLocation();
	AActor* HitActor = UChessUtil::GetCollidedPiece(GetWorld(), Start);
	APiece* HitPiece = Cast<APiece>(HitActor);
	if (IsValid(HitPiece))
	{
		// For debugging
		FString PieceTypeName = UChessUtil::GetPieceTypeString(HitPiece->GetPieceType());
		FString PieceColorName = UChessUtil::GetColorString(HitPiece->GetPieceColor());
		return HitPiece;
	}
	else
	{
		return nullptr;
	}
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

void AChessPlayer::ShowThreatMap()
{
	if (IsValid(ThreatMap) && bIsThreatMapVisible)
	{
		ThreatMap->ShowMap();
	}
}

void AChessPlayer::DestroyThreatMap()
{
	if (IsValid(ThreatMap))
	{
		ThreatMap->DestroyThreatBoxes();
	}
}

EPieceType AChessPlayer::GetCurPieceType() const
{
	return CurPickedPiece->GetPieceType();
}

bool AChessPlayer::IsCheckmate() const
{
	return ThreatMap->IsCheckmate();
}

bool AChessPlayer::IsStalemate() const
{
	return ThreatMap->IsStalemate();
}

bool AChessPlayer::IsCheck() const
{
	return ThreatMap->IsCheck();
}

void AChessPlayer::ThreatBtnPressed()
{
	if (IsPlayerControlled())
	{
		bIsThreatMapVisible = !bIsThreatMapVisible;

		if (bIsThreatMapVisible)
		{
			ShowThreatMap();
		}
		else
		{
			DestroyThreatMap();
		}
	}
}