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
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap); //player �浹����ó��
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap); //enemy �浹����ó��
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
}

// Called when the game starts or when spawned
void AShieldActor::BeginPlay()
{
	Super::BeginPlay();

	//�浹�� ������ �Լ� ���ε� �ϱ�
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
	//�ε��� ����� Player���..
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);
	AEnemy* enemy = Cast<AEnemy>(OtherActor);

	UE_LOG(LogTemp, Warning, TEXT("Player is Attached to ShieldActor"));
		
	if (player != nullptr) 
	{
		//player���� �����ȴ�
		this->AttachToActor(player, FAttachmentTransformRules::KeepWorldTransform);

		//player->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

		FTimerHandle snapTimer;
		GetWorld()->GetTimerManager().SetTimer(snapTimer, this, &AShieldActor::ResetAttach, 3, true);

	}
	
	//�ε��� ����� Enemy���
	
	if (enemy != nullptr) 
	{
				
		//�浹�� Enemy�� �ı��Ѵ�.
		enemy->Destroy();

		UE_LOG(LogTemp, Warning, TEXT("Enemy is Attached to ShieldActor"));
	}
	
	
}

void AShieldActor::ResetAttach()
{
	//�ٿ����� �θ� �����Ѵ�
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	//��� ���� �̹� �θ� �ִ� ���̹Ƿ� ���� �θ� �������� �ʿ䰡 ����
}

