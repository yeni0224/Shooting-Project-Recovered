// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "bullet.h"
#include "EnenmyBullet.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AEnenmyBullet : public Abullet
{
	GENERATED_BODY()

public:
	AEnenmyBullet();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
};
