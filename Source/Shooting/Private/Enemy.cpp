// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerFlight.h"
#include "EngineUtils.h"
#include "MyShootingModeBase.h"
#include "MyShootingLibrary.h"

// Sets default values �����ϱ��� before spawn 
AEnemy::AEnemy()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f));

	/*�浹 ������ �����ϱ�*/
	//Collision Enable ���� Query and Physics�� �����Ѵ�.
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//���� ä���� Object Type �� "Enemy"�� �����Ѵ�.
	//������Ʈ config - DefaultEngine - ctrl F �ݸ����̸�
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel2);

	//����ä���� �ϰ������� ignore ���·� ó���Ѵ�
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	//��žä���� Player�� Bullet ä�ο� ���ؼ��� overlap���� ó���Ѵ�.
	//>> ��ü�� ignore�� �а� overlap���� �ؾ� �ڵ带 ���� �� ����. ���� �߿�!
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
	//�ݸ��������� C++���ϸ� �������Ʈ Ŭ������ �ٽø����� ����ȴ�.

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(RootComponent);

	//��ġ ����
	meshComp->SetRelativeLocation(FVector(0, 0, -50.0f));

}

// Called when the game starts or when spawned ��������
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//�浹ó��
	//boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlap);

	//��÷�� ���ؼ� �ϳ��� ���� ����, �� �ٸ� �ϳ��� �÷��̾� �������� �̵��ϰ�ʹ�
	//1. ��÷�� �Ѵ�. Ȯ���� ������ �̿��ؼ� 7:3 ������ �Ѵ�.
	int32 drawNumber = FMath::RandRange(1, 100);
	//fmath Ŭ������ �ִ� randrange�Լ�, ���� Ȯ���� ������ 0~1������ �Ǽ��� ������ ����Ѵ�

	//2. ����, ���� ���� 30���� ������...
	if (drawNumber <= traceRate)
	{

		//2-1. �÷��̾� ���͸� ã�´� : ������ ������BP�� �÷��̾ ã�ƾ��ϹǷ�, �ܼ��� ���⸸ ���Ұ��̹Ƿ� h�� �������� �����ϱ�.

		//BP���� ��ġ�� �ٸ� �÷��̾� ã�� �� (���忡�� Ư���� ��ü�� ã�� ��� 1):
		//��ȯ���� Actor, ���� �Լ��� ���ڷ� �޴´�, ã������ ���(APlayerFlight.h�� �߰��������)	
		//AActor* target = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerFlight::StaticClass());


		//2-2. �÷��̾��� ��ġ(���� �������ϴ� ����) - ���� ��ġ = �� ������ �����Ѵ�

		//���忡�� Ư���� ��ü�� ã�� ��� 1
		//���͸� �������� ��ġ ��ü�� �� �� �ֱ� ������
		//FVector targetdir = target->GetActorLocation() - GetActorLocation();
		//direction = targetdir;
		//�̷��� �ϸ� ������ �Ÿ��� õ������ ���̰� ���� �ȴ�. ������ ��� �Ǿ��� ������ ���̰� �������� 1¥���� �ʱ�ȭ��������Ѵ�		


		//���忡�� Ư���� ��ü�� ã�� ���2
		//APlayerFlight* target; << �긦 ���������� ������ֱ� ���� h�� ������

		// AplayerFlight���� ����Ѵ�
		//�ؼ� : World�� ��ġ�� ��� �迭���� Iterator��� ���Ŀ� �־���, �ϳ��� ������ �� null���� �ƴ϶��, ��ȯ �Ҷ����� �ϳ��� ��������
		for (TActorIterator<APlayerFlight> it(GetWorld()); it; ++it)
		{
			target = *it;
		}
		//����ȭ�� ���ؼ�
		if (target != nullptr)
		{
			float temp = target->moveSpeed;

			FVector targetdir = target->GetActorLocation() - GetActorLocation();
			targetdir.Normalize();
			direction = targetdir;
			//player�� ���� ���¿��� enemy�� �߻��� �� ũ���� ������ ���ؼ�

			target->playerBomb.AddDynamic(this, &AEnemy::DestroyMySelf);
		}
	}

	//3. �׷���������
	else
	{
		//3-1. �������� ������ ���Ѵ�
		direction = GetActorForwardVector();
	}

	//�浹�� �߻��ϸ� ������ �Լ� ����
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlap);

	boxComp->SetGenerateOverlapEvents(true);

	//���Ӹ�忡 enemies �迭�� �ڱ� �ڽ��� �ִ´�
// 	AMyShootingModeBase* gm = Cast<AMyShootingModeBase>(GetWorld()->GetAuthGameMode());
// 
// 	if (gm != nullptr) {
// 		gm->enemies.Add(this);
// 	}

//
	target->OnSetdirection.AddDynamic(this, &AEnemy::SetNewDirection);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���� ���� �������� ��� �̵��ϰ� �ʹ�. ������ �����Ͽ����Ƿ� ������ �ȴ�
    //direction = GetActorForwardVector();

    //��ġ �̵�, ForwardVector�� normalize�� �ʿ����� �ʴ�
	//SetActorLocation(GetActorLocation() + direction * EnemySpeed * DeltaTime);

	//�츮�� ���� ���̺귯�� �Լ��� �̿��ؼ� �̵���Ų��
	UMyShootingLibrary::MoveMyActor(this, direction, EnemySpeed, DeltaTime);

}

void AEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);

	if (player != nullptr) {
		//player->Destroy();

		//�޴� ������ �����ϴ� �Լ��� �����Ѵ�
		AMyShootingModeBase* gm = Cast<AMyShootingModeBase>(GetWorld()->GetAuthGameMode());

		if (gm != nullptr) 
		{
			gm->ShowMenu();
		}

		//�÷��̾ ������ �޴�â�� �ߵ��� �� ���̹Ƿ� �ʿ������
		//player->ReservationHitColor(0.2f);

		Destroy();
	}

}

void AEnemy::DestroyMySelf()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosion_fx, GetActorLocation(), GetActorRotation(), true);
	Destroy();
}


void AEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason) //������ �Լ��� ������ ���� ��
{
//�״� ���� �ڱ� �ڽ��� �迭���� �����Ѵ�

// 	AMyShootingModeBase* gm = Cast<AMyShootingModeBase>(GetWorld()->GetAuthGameMode());
// 
// 	if (gm != nullptr) {
// 		gm->enemies.Remove(this);
// 	}
// 
//��������Ʈ�� �ɾ���� �ڱ� �Լ��� �����Ѵ�
	//target->playerBomb.RemoveDynamic(this, AEnemy::DestroyMySelf());
}


void AEnemy::SetNewDirection(FVector newDir)
{
	//�̵� ������ newDir�� �ٲ۴�
	direction = newDir;

}

