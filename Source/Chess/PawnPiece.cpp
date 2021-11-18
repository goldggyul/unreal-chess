// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnPiece.h"
#include "DrawDebugHelpers.h"

APawnPiece::APawnPiece()
{
	SetType(EPieceType::Pawn);

	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PAWN MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/Meshes/Pieces/SM_Pawn.SM_Pawn'"));
	if (SM.Succeeded())
	{
		PieceMesh->SetStaticMesh(SM.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't load pawn mesh"));
	}
	SetRootComponent(PieceMesh);
	PieceMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
}

void APawnPiece::UpdateLegalMoves()
{
	Super::UpdateLegalMoves();

	// 일단 Legal Moves 말고 Possible Moves만

	/*
	* 폰의 행마법: 행마법과 기물을 잡는 법이 다르다
	* 1. 폰은 전진만 한다
	* 2. 한 번도 움직이지 않은 폰은 두 칸 전진이 가능하다. 그 외는 한 칸 전진
	* 3. 기물을 잡을 때는 대각선 전방에 있는 걸 잡을 수 있다.
	*	 한 칸(혹은 두칸) 전진한 곳에 적의 기물이 있다면 잡지 못하고 가지도 못한다.
	* 
	* 특별 행마법: 앙파상과 프로모션
	* 1. 앙파상
	* 바로 직전에 적의 폰이 두 칸 이동했으며 내 폰의 바로 왼쪽/오른쪽 중 한 곳에 있다면,
	* 그 방향 대각선으로 전진하면서 적의 기물을 잡을 수 있다.
	* 2. 프로모션
	* 폰이 상대방의 마지막 랭크에 가면 폰, 킹을 제외한 피스로 승격할 수 있다.
	* 
	* 구현
	* 1. 앙파상의 경우, 단순히 처음 두 칸 움직인 적의 폰이 아니라 '직전'에 움직였음을 알아야한다.
	*	1) Piece에 정보 주기? 그럼 모든 폰에게 정보를 줘야함
	*	2) Player에 정보 주기? 턴이 바뀔 때 직전에 폰이 처음으로 움직였다면 정보를 줘야한다. 즉 ChangePlayer일 때 앙파상을 Set
	*/

	// Test: 일단 자기꺼면 든다
	FHitResult HitResult;

	FVector Start = GetActorLocation();
	FVector End = Start;
	FVector Differ = GetPieceFowardVector() * SquareSize;

	// Line Trace
	FCollisionObjectQueryParams ObjectQuery;
	ObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
	FCollisionQueryParams Query = FCollisionQueryParams::DefaultQueryParam;
	Query.AddIgnoredActor(GetOwner());
	
	/*
	가장 가까운 부딪힌 액터를 얻어옴
	충돌 없음: 그냥 가능
	충돌 있음: 몇 Differ인지를 확인 -> 그 충돌칸 전까지 다 LegalMoves에 넣고,
	폰의 경우엔 적의 
	*/

	// 한 칸 앞
	End += Differ;
	DrawDebugLine(
		GetWorld(), Start, End,
		FColor::Orange, false, 1.f, 0.f, 10.f
	);

	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, ObjectQuery, Query);
	APiece* HitPiece = Cast<APiece>(HitResult.GetActor());
	if (!IsValid(HitPiece))
	{
		LegalMoves.Add(End);
	}

	// 첫 번째 움직임인 경우 두 칸 앞
	if (IsFirstMove())
	{
		End += Differ;
		DrawDebugLine(
			GetWorld(), Start, End,
			FColor::Orange, false, 1.f, 0.f, 10.f
		);
		GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, ObjectQuery, Query);
		HitPiece = Cast<APiece>(HitResult.GetActor());
		if (!IsValid(HitPiece))
		{
			LegalMoves.Add(End);
		}
	}

	// 적의 기물 잡기: 대각선 전방
	// 1. 오른쪽 대각선
	End  = Start + GetPieceFowardVector() * SquareSize + GetPieceRightVector() * SquareSize;
	DrawDebugLine(
		GetWorld(), Start, End,
		FColor::Orange, false, 1.f, 0.f, 10.f
	);
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, ObjectQuery, Query);
	if (HitResult.bBlockingHit)
	{
		HitPiece = Cast<APiece>(HitResult.GetActor());
		if (HitPiece->GetPieceColor() != GetPieceColor()) // 적의 기물 -> 잡을 수 있음
		{
			LegalMoves.Add(End);
		}
	}

	// 1. 왼쪽 대각선
	End = Start + GetPieceFowardVector() * SquareSize - GetPieceRightVector() * SquareSize;
	DrawDebugLine(
		GetWorld(), Start, End,
		FColor::Orange, false, 1.f, 0.f, 10.f
	);
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, ObjectQuery, Query);
	if (HitResult.bBlockingHit)
	{
		HitPiece = Cast<APiece>(HitResult.GetActor());
		if (HitPiece->GetPieceColor() != GetPieceColor()) // 적의 기물 -> 잡을 수 있음
		{
			LegalMoves.Add(End);
		}
	}
}
