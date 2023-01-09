// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossActor.generated.h"

UCLASS()
class SHOOTING_API ABossActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnyWhere, Category = BossSettings)
	class UBoxComponent* boxCollision;

	UPROPERTY(EditAnyWhere, Category = BossSettings)
	class UMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category = BossSettings)
	TSubclassOf<class AEnenmyBullet> bullet;

	//클래스 자체를 가리킬 변수 선언해서 화살표로 함수 연결하면 되겠구만
	class AEnenmyBullet* enemyBullet;

private:
	FVector startLocation;
	FVector endLcation;

	float currentTime = 0;
	float pattern_delay = 5.0f;

	//부채꼴 모양으로 총알을 발사할것이다
	UFUNCTION()
	void BossAttack1(float angle, int count);

	//총


};
