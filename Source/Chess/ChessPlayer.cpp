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
	//PickBox->SetActorLabel(TEXT("PickBox"));
	//PickBox->SetFolderPath("/Player");
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
			Dest.Z = PickedPiece->GetActorLocation().Z;
			PickedPiece->SetActorLocation(Dest);
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
				CurPickedPiece->ShowMoves();
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
	if (IsValid(PickedPiece))
		PickedPiece->Destroy();

	if (IsValid(CurPickedPiece))
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Picked Piece"));

		PickedPiece = CurPickedPiece->GetCopy();
		
		CurPickedPiece->SetMeshOpaque(false);
		PickedPiece->SetMeshOpaque(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot pick: It's empty!"));
	}
}

bool AChessPlayer::PutCurPiece()
{
	// 현재 위치에 놓을 수 있으면 놓는다.
	if (!IsValid(CurPickedPiece) || !IsValid(PickedPiece) || !IsValid(PickBox))
		return false;
	FVector CurPickLocation = PickedPiece->GetActorLocation();
	CurPickLocation.Z = 0;
	if (CurPickedPiece->IsAbleToPutAt(CurPickLocation))
	{
		// 입력 막기
		bool bIsOtherLocation = (CurPickedPiece->GetActorLocation() != CurPickLocation); // 제자리가 아닌 유효한 곳에 놨는지
		PickedPiece->Destroy();
		CurPickedPiece->PutAt(CurPickLocation);
		CurPickedPiece->SetMeshOpaque(true);
		CurPickedPiece = nullptr;
		SetPicking(false);
		OnPutPiece.Broadcast(); // For UI
		UGameplayStatics::PlaySound2D(this, PutSound, 5.0f);
		return bIsOtherLocation;
	}
	else
	{
		APiece* CurLocatedPiece = GetPieceAtPickBox();
		if (IsValid(CurLocatedPiece))
		{
			UE_LOG(LogTemp, Error, TEXT("Somethin here..."));
			if ((CurLocatedPiece->IsAbleToPick()) && (CurLocatedPiece->GetPieceColor() == GetPlayerColor()))
			{
				// Change Cur Piece
				CurPickedPiece->PutAt(CurPickedPiece->GetActorLocation());
				CurPickedPiece->SetMeshOpaque(true);
				PickPiece();
			}
		}
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
	FVector Start = PickBox->GetActorLocation();
	AActor* HitActor = UChessUtil::GetCollidedPiece(GetWorld(), Start);
	APiece* HitPiece = Cast<APiece>(HitActor);
	if (IsValid(HitPiece))
	{
		return HitPiece;
	}
	else
	{
		return nullptr;
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