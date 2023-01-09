// Fill out your copyright notice in the Description page of Project Settings.


#include "bullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "MyShootingModeBase.h"

// Sets default values
Abullet::Abullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	boxComp->SetWorldScale3D(FVector(0.8f, 0.5f, 0.5f)); //스케일 코드로 조정하기

	//총알 콜리전의 충돌 처리 프리셋을 "BulletPreset"으로 설정한다
	//boxComp->SetCollisionProfileName(TEXT("BulletPreset")); >>Playerbullet에게 필요하므로 이건 지운다

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp -> SetupAttachment(RootComponent);
	//상대적인 위치
	meshComp->SetRelativeLocation(FVector(0, 0, -50.0f));
	//relative : 상대적인 <-> Set World Relative Location : 절대적인
}

// Called when the game starts or when spawned
void Abullet::BeginPlay()
{
	Super::BeginPlay();

	//충돌(overlap)이 발생하면 실행할 함수를 연결한다
	//boxComp->OnComponentBeginOverlap.AddDynamic(this, &Abullet::OnOverlap);
	//Playerbullet의 BeginPlay에서 구현할것이다

	//오버랩 이벤트가 발생하도록 설정한다
	boxComp->SetGenerateOverlapEvents(true);

	//태어난 후 2초 뒤에 제거한다.
	//SetLifeSpan(2.0f);                                 //실행할 함수  초 설정, 반복 참 거짓
	GetWorld()->GetTimerManager().SetTimer(lifeTimer, this, &Abullet::DestroySelf, 2.0f, false);
}

// Called every frame
void Abullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//direction = FVector(1, 0, 0); world direction
	// 나의 정면 방향으로 계속 이동하고 싶다
	
	direction = GetActorForwardVector();

	//위치 이동, ForwardVector은 normalize가 필요하지 않다
	SetActorLocation(GetActorLocation() + direction * bulletSpeed * DeltaTime);

}


void Abullet::SetDirection(FVector value)
{
	direction = value;
}

void Abullet::DestroySelf() {
	Destroy();
}

