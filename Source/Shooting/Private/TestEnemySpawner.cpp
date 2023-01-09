// Fill out your copyright notice in the Description page of Project Settings.


#include "TestEnemySpawner.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "TestEnemyActor.h"

// Sets default values
ATestEnemySpawner::ATestEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootScene1 = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	SetRootComponent(rootScene1);

	spawnArrow1 = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn Arrow"));
	spawnArrow1->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void ATestEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATestEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentTime += DeltaTime;

	if (currentTime >= spawnCoool1) {
		GetWorld()->SpawnActor<ATestEnemyActor>(enemyFactory1, spawnArrow1->GetComponentLocation(), spawnArrow1->GetComponentRotation());

		currentTime = 0;
	}


}

