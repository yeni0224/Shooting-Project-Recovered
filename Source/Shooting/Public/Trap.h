// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trap.generated.h"

UCLASS()
class SHOOTING_API ATrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//1. USphere Component를 선언한다
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TrapSettings)
	class USphereComponent* sphereComp;

	//2. UStaticMesh를 선언한다
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TrapSettings)
	class UStaticMeshComponent* sMeshComp;

	//5. "InTrap" 함수 선언
	UFUNCTION(BlueprintCallable)
	void InTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//B. OutTrap 함수 선언
	UFUNCTION(BlueprintCallable)
	void OutTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TrapSettings)
	float teleportTime = 2.0f;
private:
	
	float currentTime = 0;

};
