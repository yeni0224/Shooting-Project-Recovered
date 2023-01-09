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
	boxComp->SetWorldScale3D(FVector(0.8f, 0.5f, 0.5f)); //������ �ڵ�� �����ϱ�

	//�Ѿ� �ݸ����� �浹 ó�� �������� "BulletPreset"���� �����Ѵ�
	//boxComp->SetCollisionProfileName(TEXT("BulletPreset")); >>Playerbullet���� �ʿ��ϹǷ� �̰� �����

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp -> SetupAttachment(RootComponent);
	//������� ��ġ
	meshComp->SetRelativeLocation(FVector(0, 0, -50.0f));
	//relative : ������� <-> Set World Relative Location : ��������
}

// Called when the game starts or when spawned
void Abullet::BeginPlay()
{
	Super::BeginPlay();

	//�浹(overlap)�� �߻��ϸ� ������ �Լ��� �����Ѵ�
	//boxComp->OnComponentBeginOverlap.AddDynamic(this, &Abullet::OnOverlap);
	//Playerbullet�� BeginPlay���� �����Ұ��̴�

	//������ �̺�Ʈ�� �߻��ϵ��� �����Ѵ�
	boxComp->SetGenerateOverlapEvents(true);

	//�¾ �� 2�� �ڿ� �����Ѵ�.
	//SetLifeSpan(2.0f);                                 //������ �Լ�  �� ����, �ݺ� �� ����
	GetWorld()->GetTimerManager().SetTimer(lifeTimer, this, &Abullet::DestroySelf, 2.0f, false);
}

// Called every frame
void Abullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//direction = FVector(1, 0, 0); world direction
	// ���� ���� �������� ��� �̵��ϰ� �ʹ�
	
	direction = GetActorForwardVector();

	//��ġ �̵�, ForwardVector�� normalize�� �ʿ����� �ʴ�
	SetActorLocation(GetActorLocation() + direction * bulletSpeed * DeltaTime);

}


void Abullet::SetDirection(FVector value)
{
	direction = value;
}

void Abullet::DestroySelf() {
	Destroy();
}

