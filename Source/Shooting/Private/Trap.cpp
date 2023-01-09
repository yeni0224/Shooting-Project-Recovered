// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerFlight.h"


// Sets default values
ATrap::ATrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//3. USphere Component�� ��Ʈ �ݸ����� �����Ѵ�
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Trap Collision"));
	SetRootComponent(sphereComp);
	sphereComp->SetSphereRadius(200.f);
	
	//4. UStaticMesh�� ��Ʈ�� �ڽ� ������Ʈ�� �����Ѵ�
	sMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sMesh"));
	sMeshComp->SetupAttachment(RootComponent);
	sMeshComp->SetRelativeScale3D(FVector(4.0f)); //ũ�⸦ 4�� Ű���ش�

	//8. �浹 �������� "TrapPreset"���� ����
	sphereComp->SetCollisionProfileName(TEXT("TrapPreset"));
}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();

	//7. Overlap ��������Ʈ�� InTrap �Լ��� �����Ѵ�
	//���⼭ overlap�� �Լ��� ���ڸ� f12 ������ ���Ǹ� ã�� �� ���� ��������
	//h���� intrap�Լ� ���ڿ� ��� �����ش�
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATrap::InTrap); 
	
	//A. �ڽ����� ���������� ���¿� ���� �Լ��� ������ش�.
	sphereComp->OnComponentEndOverlap.AddDynamic(this, &ATrap::OutTrap); //this : �ڱ� �ڽ� Ŭ���� ������ �̾߱��Ѵ�.

}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	//���� ���� �ð�(currentTime)�� ������ �ð�(teleportTime)�� �����Ͽ��ٸ�//
	if (currentTime >= teleportTime) 
	{
		//������ ��ġ(vector)�� �����Ѵ�
		float rand_hor = FMath::RandRange(-500.0f, 500.0f); //�¿��  5M��
		float rand_ver = FMath::RandRange(300.0f, 1150.0f); //���Ϸ� 3M��, ���� �ֻ�� ��ǥ(x, y, z)�� (0, -500, 300)
		FVector random_vec = FVector(-970.0f, rand_hor, rand_ver); //�ѹ��� ���� ���

		//���� �̸� ���������� �ʴٸ� �̷���
		//FVector random_vec;
		//random_vec.Y = rand_hor;
		//random_vec.Z = rand_ver;

		//FVector random_vec = FMath::VRand()*500.f; //���ֺ���(normalize����)�� ���·� ��ǥ�� ���´�, ��ü�� ���·� ��ǥ�� �����ȴ�
		//random_vec.X = 0; //���⼭�� X��ǥ�� �����̸� �ȵǹǷ� 0���� ������Ų��. (sphere���� circle�� ���� �ȿ��� �������� ��ġ �̱�)

		//�� ��ġ�� �̵��Ѵ�
		SetActorLocation(random_vec);

		//(currentTime)�ð��� �ʱ�ȭ�Ѵ� *�߿�*
		currentTime = 0;
	}
	else //�׷��� �ʴٸ�
	{
		//�ð��� �����Ѵ�
		currentTime += DeltaTime;
	}

}

//6. "InTrap" �Լ� ����
void ATrap::InTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	//����, �÷��̾��� �����..
	APlayerFlight* player = Cast< APlayerFlight>(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("Player in mine"))

	if (player) 
	{
		//�÷��̾ �ִ� CanFire������ False�� �����
		player->canFire = false;

	}

}

//C. "OutTrap" �Լ� ����
void ATrap::OutTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//D. ����, ������ ������ �ε��� ���Ͱ� �÷��̾���..
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);

	if (player != nullptr) 
	{
		//E. �÷��̾��� canFire������ ���� true�� �����Ѵ�.
		player->canFire = true;
	}
	
}

