// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyShootingLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API UMyShootingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
	static void MoveMyActor(class AActor* actor, FVector dir, float speed, float deltatime);
	//static : 메모리 관리가 어렵다, 남발하지 않고 한쪽에 모아놓는 것을 추천한다
	
};
