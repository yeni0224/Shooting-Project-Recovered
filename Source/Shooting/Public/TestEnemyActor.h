// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestEnemyActor.generated.h"

UCLASS()
class SHOOTING_API ATestEnemyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestEnemyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EnemySettings)
	class UBoxComponent* boxComp1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EnemySettings)
	class UStaticMeshComponent* meshComp1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = EnemySettings)
	float enemySpeed1 = 400;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = EnemySettings)
	float traceRate1 = 45.0f;

private:

	FVector direction1;
};
