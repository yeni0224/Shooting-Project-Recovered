// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeTest2.h"

// Sets default values
ACodeTest2::ACodeTest2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	pt_number = 30;
}

// Called when the game starts or when spawned
void ACodeTest2::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ACodeTest2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

