// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CodeTest.generated.h"



UCLASS()
class SHOOTING_API ACodeTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACodeTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	//정수형 변수 number를 선언한다.
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CodeVarible) //블루프린트에 나오게함
	int32 number; // 디폴트는 0
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CodeVarible)
	int32 number2; 
	//실수형 변수 선언
		UPROPERTY(EditInstanceOnly)
	float fnumber1=3.14;
		UPROPERTY(VisibleAnywhere)
	float fnumber2=10.214124;
	//문자열 변수 선언
	FString name; //디폴트는 빈칸
	FString name2 = "apple";
	FString name3 = FString(TEXT("안녕ㅎㄹㄶ"));
	
	//논리형 변수 선언
	bool isit;
	bool isthat = true; //값을 바로 넣는것 선언과 동시에 초기화

	int32 Add(int32 num, int32 num2);
	int32 ReverseAdd(int32 num, int32 num2);
	int32 Product(int32 num, int32 num2);
	float Divide(float num, float num2);

	UFUNCTION(BlueprintPure) // 실행핀 없는 노드
	void Swap(int32& num, int32& num2);

	void Swap2(int32* aa, int32* bb);
	UFUNCTION(BlueprintCallable) // 블루프린트에서 노드로 사용(실행핀있음)
	FString StringAppender(FString nam, FString nam2);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CodeVariable)
	int age = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CodeVariable)
	float height = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CodeVariable)
	FString nation ="";
	int32 result = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CodeVariable)
	TArray<int32> NumArr = { 1,2,3,5,6,7 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CodeVariable)
	TMap<int32, float> intfloat;


	
	

	

};
