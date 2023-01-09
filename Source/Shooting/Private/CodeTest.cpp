// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeTest.h"
//#include "CodeTest2.h"
#pragma warning (disable: 4458) // c4458 경고 해제

// Sets default values
ACodeTest::ACodeTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACodeTest::BeginPlay()
{
	Super::BeginPlay();
	number = 3; // number 변수에 3을 넣는다
	number2 = number;
	number2 = -7;
	// number의 값을 output 콘솔 창에 출력한다.
	UE_LOG(LogTemp, Error, TEXT("number is %d"), number);
	UE_LOG(LogTemp, Error, TEXT("number2 is %d"), number2);
	UE_LOG(LogTemp, Error, TEXT("fnumber1 is %f"), fnumber1);
	UE_LOG(LogTemp, Error, TEXT("fnumber2 is %f"), fnumber2);
	UE_LOG(LogTemp, Error, TEXT("fnumber2 is %.2f"), fnumber2);
	UE_LOG(LogTemp, Error, TEXT("fnumber2 is %s"), *name);
	UE_LOG(LogTemp, Error, TEXT("fnumber2 is %s"), *name2);
	UE_LOG(LogTemp, Error, TEXT("fnumber3 is %s"), *name3);
	UE_LOG(LogTemp, Error, TEXT("================="));
	
	if (isit == false) {
		UE_LOG(LogTemp, Error, TEXT("isit은 거짓"));
	}
	if (isthat == true) {
		UE_LOG(LogTemp, Error, TEXT("isthat은 참"));
	}
	int32 num = 8;
	int32 num2 = 4;
	int32 result = Add(num, num2);
	UE_LOG(LogTemp, Warning, TEXT("result is %d"), result);
	result = ReverseAdd(num, num2);
	UE_LOG(LogTemp, Warning, TEXT("result is %d"), result);
	Swap(num, num2); // call by reference 값을 바꾼다
		result = Product(num, num2);
	UE_LOG(LogTemp, Warning, TEXT("result is %d"), result);
	float result2 = Divide(num, num2);
	UE_LOG(LogTemp, Warning, TEXT("result is %f"), result2);
		int intnum = fnumber1;
	UE_LOG(LogTemp, Warning, TEXT("intnum is %d"), intnum);
	UE_LOG(LogTemp, Warning, TEXT("(int)fnumber1 is %d"), (int)fnumber1);

	if (age >= 20) {
		UE_LOG(LogTemp, Warning, TEXT("꼰대"));
	}
	else if (age >= 10) {
		UE_LOG(LogTemp, Warning, TEXT("10~20"));
	}
	else UE_LOG(LogTemp, Warning, TEXT("꼰대 아님"));

		if (age >= 20 && height >= 160) {
			UE_LOG(LogTemp, Warning, TEXT("환영"));
		}
		else UE_LOG(LogTemp, Warning, TEXT("외면"));
	
	if (nation == TEXT("한국") && (age >= 20 && age < 40)) {
		UE_LOG(LogTemp, Warning, TEXT("청년"));
	}
	else UE_LOG(LogTemp, Warning, TEXT("청년아님"));
	//StringAppender("a", "b");
	UE_LOG(LogTemp, Warning, TEXT("%s"), *StringAppender("a", "b"));

	int32 checker = 123;
	if (checker % 2 == 0) {
		UE_LOG(LogTemp, Warning, TEXT("EVEN"));
	}
	else UE_LOG(LogTemp, Warning, TEXT("ODD"));

	for (int32 i = 0; i < 10; i++) {
		UE_LOG(LogTemp, Warning, TEXT("hi"));
	}

	int32 num3 = 3;
	num3 += 2;
	num3++;

	int32 a = num3 + num3++; // 12
	UE_LOG(LogTemp, Warning, TEXT("%d"), a);
	a = num3 + ++num3;
	UE_LOG(LogTemp, Warning, TEXT("%d"), a);

	//구구단 7단
	for (int32 i = 1; i < 10; i++) {
		for (int32 j = 1; j < 10; j++) {
			UE_LOG(LogTemp, Warning, TEXT("%d * %d = %d"),i, j, i*j);
		}
	}

	TArray<int32> ages;
	ages = { 3, 4, 5 }; 
	UE_LOG(LogTemp, Warning, TEXT("배열의 길이 %d"), ages.Num()); //배열 길이함수 Num()
	for (int32 i = 0; i < ages.Num(); i++) {
		UE_LOG(LogTemp, Warning, TEXT("%d"), ages[i]);
	}
	for (int32 i = 0; i < NumArr.Num(); i++) {
		UE_LOG(LogTemp, Warning, TEXT("%d"), NumArr[i]);
	}


	TMap<FString, float> distances;
	distances.Add("sss", 8.45f);
	UE_LOG(LogTemp,Warning,TEXT("%f"), distances["sss"])
		for (auto dist : distances) {// dist안에 키와 밸류 둘다 담겨있다
			UE_LOG(LogTemp, Warning, TEXT("%s---%f"), *dist.Key, dist.Value);
		}	

	intfloat.Add(1, 1.1);
	intfloat.Add(2, 2.2);
	for (auto inf : intfloat) {
		UE_LOG(LogTemp, Warning, TEXT("inf = %d---%f"), inf.Key, inf.Value);
	}

	int32 num5 = 1235;
	int32* ptr = &num5; //포인터변수 선언
	num5 = 22;
	UE_LOG(LogTemp, Warning, TEXT("num5 == %d ptr == %d"),num5, *ptr);
	*ptr = 66;
	UE_LOG(LogTemp, Warning, TEXT("num5 == %d ptr == %d"), num5, *ptr);
	UE_LOG(LogTemp, Warning, TEXT("*ptr == %d"),*ptr); //포인터변수가 참조하는주소의 값
	UE_LOG(LogTemp, Warning, TEXT("ptr == %p"), ptr); //포인터변수가 참조하는주소
	UE_LOG(LogTemp, Warning, TEXT("&ptr == %p"), &ptr); //포인터변수의 주소
	UE_LOG(LogTemp, Warning, TEXT("&num5 == %p"), &num5);

	int32 aaa = 32, bbb = 23;
	UE_LOG(LogTemp, Warning, TEXT("befor :: aaa == %d, bbb == %d"), aaa, bbb);
	Swap2(&aaa, &bbb);
	UE_LOG(LogTemp, Warning, TEXT("after :: aaa == %d, bbb == %d"), aaa, bbb);

	//CodeTest2->pt_number = 1111;
	//UE_LOG(LogTemp, Warning, TEXT("%d"), CodeTest2->pt_number);

	
}

// Called every frame
void ACodeTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


int32 ACodeTest::Add(int32 num, int32 num2) {
	return num + num2;
}

int32 ACodeTest::ReverseAdd(int32 num, int32 num2) {
	return num - num2;
}

void ACodeTest::Swap(int32& num, int32& num2) {
	int temp = num;
	num = num2;
	num2 = temp;
}

void ACodeTest::Swap2(int32* aa, int32* bb) {
	int temp = *aa;
	*aa = *bb;
	*bb = temp;
}

int32 ACodeTest::Product(int32 num, int32 num2) {
	return num * num2;
}

float ACodeTest::Divide(float num, float num2) {
	return num / num2;
}

FString ACodeTest::StringAppender(FString nam, FString nam2) {
	return nam + nam2;
}

