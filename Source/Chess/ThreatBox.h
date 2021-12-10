// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessInfo.h"
#include "ThreatBox.generated.h"

#define MoreEnemy FLinearColor(5.0f, 0.0f, 0.1f)
#define OneEnemy FLinearColor(5.0f, 0.1f, 0.0f)
#define MorePlayer FLinearColor(0.1f, 0.1f, 5.0f)
#define OnePlayer FLinearColor(0.15f, 0.45f, 5.0f)

UCLASS()
class CHESS_API AThreatBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThreatBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetShapeAndColor(uint8 WhiteCount, uint8 BlackCount);

private:
	UPROPERTY(VisibleAnywhere, Category = ThreatBox)
	class UStaticMeshComponent* BoxMesh;

	UPROPERTY(VisibleAnywhere, Category = ThreatBox)
	class UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(VisibleAnywhere, Category = ThreatBox)
	class UTexture* Square;

	UPROPERTY(VisibleAnywhere, Category = ThreatBox)
	class UTexture* HalfSquare;

	UPROPERTY(VisibleAnywhere, Category = ThreatBox)
	class UTexture* QuarterSquare;

	UPROPERTY(VisibleAnywhere, Category = ThreatBox)
	int32 BoxPlayerCount;

	UPROPERTY(VisibleAnywhere, Category = ThreatBox)
	int32 BoxEnemyCount;
	
	
};
