// Fill out your copyright notice in the Description page of Project Settings.


#include "BossActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnenmyBullet.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ABossActor::ABossActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(boxCollision);
	boxCollision->SetBoxExtent(FVector(50, 300, 150));
	boxCollision->SetGenerateOverlapEvents(true);
	boxCollision->SetCollisionProfileName(TEXT("Boss"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp -> SetupAttachment(RootComponent);
	meshComp->SetRelativeScale3D(FVector(1, 6, 3));

}

// Called when the game starts or when spawned
void ABossActor::BeginPlay()
{
	Super::BeginPlay();

	//최초의 위치에서 500으로 이동
	startLocation = GetActorLocation();
	

}

// Called every frame
void ABossActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//현재 위치에서 (0, 0, 200)위치까지 이동한다
	//어떻게 알 수 있냐 : 최초의 위치에서 이동한 위치를 뺀다

	float distance = (startLocation - GetActorLocation()).Length();

// 	float shaking = FMath::RandRange(-1.0f, 1.0f);
// 
// 	if (distance < 500) 
// 	{
// 		FVector dir = GetActorLocation() * -1;
// 		dir.Y = shaking;
// 		SetActorLocation(GetActorLocation() + GetActorUpVector() * -1 * 200, DeltaTime); //마지막에 충돌처리 안한다
// 	}
// 	
// 	else {
// 		SetActorLocation(FVector(0, 0, 200)); //위치값이 200을 넘어가면 200에서 고정시켜준다
// 	}


//Clamp : 어떤 값 안에서 벗어나지 않도록 조치하는 것
	float testValue = 0;

	testValue +=DeltaTime;
	testValue = FMath::Clamp(testValue, 0.0f, 3.0f);

	FVector result = FMath::Lerp(startLocation, endLcation, testValue);
	SetActorLocation(result);
 
	if (currentTime > pattern_delay) 
	{
		BossAttack1(15, 6);
		currentTime = 0;
    }

	else
	{
		currentTime += DeltaTime;
	}
}

void ABossActor::BossAttack1(float angle, int count)
{	
	UE_LOG(LogTemp, Warning, TEXT("Boss Actor Fires"));

	//angle간격으로 count 수만큼 총알을 생성한다
	//간격 : count-1

	//시작 각도 = 270- (angle *  (count - 1) * 0.5)
	float startAngle = 270 - (angle * (count -1) * 0.5);

	for (int32 i = 0; i < count; i++) 
	{
		//기본 좌표(0, rcos@, rsin@)
		FVector bullet_base = FVector(0, 
		100 * FMath::Cos(FMath::DegreesToRadians (startAngle + angle * i)), 
		100 * FMath::Sin(FMath::DegreesToRadians(startAngle + angle * i)));

		enemyBullet = GetWorld()->SpawnActor<AEnenmyBullet>(bullet, GetActorLocation() + bullet_base, FRotator(-90, 0, 0));
		//bullet, GetActorLocation()  : bullet 생성 위치

		FVector dir = bullet_base;
		//enemyBullet->SetDirection(bullet_base.GetSafeNormal());
		//GetSafeNormal : 원본을 바꾸고 싶지 않을 때, 벡터 값으로 반환해주고 싶다면

		//쿼터니암 계산을 사용하는 법
		//Up Vector 축을 bullet base 방향으로 회전시켰을 때의 로테이터 값을 계산한다.
		//#include "Kismet/KismetMathLibrary.h" 추가해준다
		FRotator rot = UKismetMathLibrary::MakeRotFromXZ(bullet_base.GetSafeNormal(), enemyBullet->GetActorUpVector());
		//UpVector는 고정시킬것이다

		enemyBullet->SetActorRotation(rot); //rot 계산이 완료되었기 때문에 바로 괄호 안에 집어넣는 것이다
		//enemyBullet->SetDirection(enemyBullet->GetActorForwardVector());
	}
}

