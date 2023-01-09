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

	//����� �ð��� ������Ų��.

	currentTime += DeltaTime;

	if (!isSpawn) 
	{
		return;
	}

	if(gm != nullptr)
	{
		if(gm->canEnemySpawn)
		{
			//����, ������ �ð��� spawnCool�� ������ �ð����� ũ�ų� ���ٸ�
			if (currentTime >= spawnCool) 
			{
				//Enemy �������Ʈ�� �����Ѵ�.
				GetWorld()->SpawnActor<AEnemy>(enemyFactory, spawnArrow->GetComponentLocation(), spawnArrow->GetComponentRotation());
				//������ currentTime�� �ʱ�ȭ �Ѵ�.

				currentTime = 0;
			}
		}
	}


}


//Enemy ������ �ߴ��Ѵ�
void AEnemySpawningPool::StopAllSpawn()
{

	for (TActorIterator<AEnemySpawningPool> pool(GetWorld()); pool; ++pool)
	{
		pool->isSpawn = false;
	}
}


