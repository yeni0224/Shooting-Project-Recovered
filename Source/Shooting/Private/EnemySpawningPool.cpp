// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawningPool.h"
#include "Enemy.h"
#include "BossActor.h"
#include "Components/ArrowComponent.h"
#include "EngineUtils.h"
#include "EnemySpawningPool.h"
#include "MyShootingModeBase.h"

// Sets default values
AEnemySpawningPool::AEnemySpawningPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(rootScene);

	spawnArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn Arrow"));
	spawnArrow->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemySpawningPool::BeginPlay()
{
	Super::BeginPlay();

	gm = Cast<AMyShootingModeBase>(GetWorld()->GetAuthGameMode());

	gm->canEnemySpawn = true;
}

// Called every frame
void AEnemySpawningPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//경과된 시간을 누적시킨다.

	currentTime += DeltaTime;

	if (!isSpawn) 
	{
		return;
	}

	if(gm != nullptr)
	{
		if(gm->canEnemySpawn)
		{
			//만일, 누적된 시간이 spawnCool에 지정한 시간보다 크거나 같다면
			if (currentTime >= spawnCool) 
			{
				//Enemy 블루프린트를 생성한다.
				GetWorld()->SpawnActor<AEnemy>(enemyFactory, spawnArrow->GetComponentLocation(), spawnArrow->GetComponentRotation());
				//누적된 currentTime을 초기화 한다.

				currentTime = 0;
			}
		}
	}


}


//Enemy 생산을 중단한다
void AEnemySpawningPool::StopAllSpawn()
{

	for (TActorIterator<AEnemySpawningPool> pool(GetWorld()); pool; ++pool)
	{
		pool->isSpawn = false;
	}
}


