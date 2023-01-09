// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldActor.h"
#include "Enemy.h"
#include "PlayerFlight.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AShieldActor::AShieldActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("box collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50, 150, 50));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeScale3D(FVector(1.0f, 3.0f, 1.0f));

	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap); //player 충돌응답처리
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap); //enemy 충돌응답처리
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
}

// Called when the game starts or when spawned
void AShieldActor::BeginPlay()
{
	Super::BeginPlay();

	//충돌시 실행할 함수 바인딩 하기
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AShieldActor::ShieldOverlapped);

}

// Called every frame
void AShieldActor::Tick(float DeltaTime)	
{
	Super::Tick(DeltaTime);

	AddActorWorldRotation(FRotator(0, 0, 3));

}

void AShieldActor::ShieldOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//부딪힌 대상이 Player라면..
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);
	AEnemy* enemy = Cast<AEnemy>(OtherActor);

	UE_LOG(LogTemp, Warning, TEXT("Player is Attached to ShieldActor"));
		
	if (player != nullptr) 
	{
		//player에게 부착된다
		this->AttachToActor(player, FAttachmentTransformRules::KeepWorldTransform);

		//player->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

		FTimerHandle snapTimer;
		GetWorld()->GetTimerManager().SetTimer(snapTimer, this, &AShieldActor::ResetAttach, 3, true);

	}
	
	//부딪힌 대상이 Enemy라면
	
	if (enemy != nullptr) 
	{
				
		//충돌한 Enemy를 파괴한다.
		enemy->Destroy();

		UE_LOG(LogTemp, Warning, TEXT("Enemy is Attached to ShieldActor"));
	}
	
	
}

void AShieldActor::ResetAttach()
{
	//붙여놨던 부모를 해제한다
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	//떼어낼 때는 이미 부모가 있는 것이므로 따로 부모를 지정해줄 필요가 없다
}

