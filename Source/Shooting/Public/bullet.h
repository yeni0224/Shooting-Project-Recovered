// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "bullet.generated.h"

UCLASS()
class SHOOTING_API Abullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Abullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BulletSettings)
		class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BulletSettings)
		class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BulletSettings)
		float bulletSpeed=1000.f;

	//적의 위치를 알고 폭발해야하기 때문에 변수 생성
	UPROPERTY(EditDefaultsOnly, Category = BulletSettings)
	class UParticleSystem* Explosion_effect;

	//자식 클래스에 다 넘겨버렸기 때문에 여기서는 지워줘야한다
	//델리게이트로만든 함수는 UFUNCTION으로 설정 
	//UFUNCTION()
	//void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetDirection(FVector value);


private:
	FVector direction;
	FTimerHandle lifeTimer;

	void DestroySelf();
};
