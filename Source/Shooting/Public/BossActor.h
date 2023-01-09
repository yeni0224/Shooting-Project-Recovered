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

	//Ŭ���� ��ü�� ����ų ���� �����ؼ� ȭ��ǥ�� �Լ� �����ϸ� �ǰڱ���
	class AEnenmyBullet* enemyBullet;

private:
	FVector startLocation;
	FVector endLcation;

	float currentTime = 0;
	float pattern_delay = 5.0f;

	//��ä�� ������� �Ѿ��� �߻��Ұ��̴�
	UFUNCTION()
	void BossAttack1(float angle, int count);

	//��


};
