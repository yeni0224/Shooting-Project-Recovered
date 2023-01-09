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

	//������ ��ġ���� 500���� �̵�
	startLocation = GetActorLocation();
	

}

// Called every frame
void ABossActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//���� ��ġ���� (0, 0, 200)��ġ���� �̵��Ѵ�
	//��� �� �� �ֳ� : ������ ��ġ���� �̵��� ��ġ�� ����

	float distance = (startLocation - GetActorLocation()).Length();

// 	float shaking = FMath::RandRange(-1.0f, 1.0f);
// 
// 	if (distance < 500) 
// 	{
// 		FVector dir = GetActorLocation() * -1;
// 		dir.Y = shaking;
// 		SetActorLocation(GetActorLocation() + GetActorUpVector() * -1 * 200, DeltaTime); //�������� �浹ó�� ���Ѵ�
// 	}
// 	
// 	else {
// 		SetActorLocation(FVector(0, 0, 200)); //��ġ���� 200�� �Ѿ�� 200���� ���������ش�
// 	}


//Clamp : � �� �ȿ��� ����� �ʵ��� ��ġ�ϴ� ��
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

	//angle�������� count ����ŭ �Ѿ��� �����Ѵ�
	//���� : count-1

	//���� ���� = 270- (angle *  (count - 1) * 0.5)
	float startAngle = 270 - (angle * (count -1) * 0.5);

	for (int32 i = 0; i < count; i++) 
	{
		//�⺻ ��ǥ(0, rcos@, rsin@)
		FVector bullet_base = FVector(0, 
		100 * FMath::Cos(FMath::DegreesToRadians (startAngle + angle * i)), 
		100 * FMath::Sin(FMath::DegreesToRadians(startAngle + angle * i)));

		enemyBullet = GetWorld()->SpawnActor<AEnenmyBullet>(bullet, GetActorLocation() + bullet_base, FRotator(-90, 0, 0));
		//bullet, GetActorLocation()  : bullet ���� ��ġ

		FVector dir = bullet_base;
		//enemyBullet->SetDirection(bullet_base.GetSafeNormal());
		//GetSafeNormal : ������ �ٲٰ� ���� ���� ��, ���� ������ ��ȯ���ְ� �ʹٸ�

		//���ʹϾ� ����� ����ϴ� ��
		//Up Vector ���� bullet base �������� ȸ�������� ���� �������� ���� ����Ѵ�.
		//#include "Kismet/KismetMathLibrary.h" �߰����ش�
		FRotator rot = UKismetMathLibrary::MakeRotFromXZ(bullet_base.GetSafeNormal(), enemyBullet->GetActorUpVector());
		//UpVector�� ������ų���̴�

		enemyBullet->SetActorRotation(rot); //rot ����� �Ϸ�Ǿ��� ������ �ٷ� ��ȣ �ȿ� ����ִ� ���̴�
		//enemyBullet->SetDirection(enemyBullet->GetActorForwardVector());
	}
}

