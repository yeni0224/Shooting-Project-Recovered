// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestEnemySpawner.generated.h"

UCLASS()
class SHOOTING_API ATestEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, Category = PoolSettings)
	float spawnCoool1 = 2.0f;

	UPROPERTY(EditInstanceOnly, Category = PoolSettings)
	TSubclassOf<class ATestEnemyActor> enemyFactory1;

	UPROPERTY(EditInstanceOnly, Category = PoolSettings)
	class UArrowComponent* spawnArrow1;

	UPROPERTY(EditInstanceOnly, Category = PoolSettings)
	class USceneComponent* rootScene1;

	private:

	float currentTime = 0;
};
