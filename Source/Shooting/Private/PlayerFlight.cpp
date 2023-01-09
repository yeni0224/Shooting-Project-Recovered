// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFlight.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Uobject/ConstructorHelpers.h"
#include "bullet.h"
#include "EngineUtils.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyShootingModeBase.h"
#include "DrawDebugHelpers.h"



// Sets default values
APlayerFlight::APlayerFlight()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Compent"));

		SetRootComponent(boxComp);

		meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Static Mesh"));

		meshComp->SetupAttachment(RootComponent);
		meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		FVector boxSize = FVector(50.0f, 50.0f, 50.0f);
		boxComp->SetBoxExtent(boxSize);

		//�ڽ� �ݸ����� �ѵ� ó�� �������� "PlayerPreset"���� �����Ѵ�.
		boxComp->SetCollisionProfileName(TEXT("PlayerPreset"));

		//�޽� ������Ʈ�� Static mesh �׸��� ť�� ������ �Ҵ��Ѵ�.
		ConstructorHelpers::FObjectFinder<UStaticMesh> triMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_TriPyramid.Shape_TriPyramid'"));

		//���� ť�� ������ �ε��ϴµ� �����ٸ�
		if (triMesh.Succeeded())
		{
			//�ε��� ������ �޽� ������Ʈ�� static mesh �׸� �ִ´�
			meshComp->SetStaticMesh(triMesh.Object);;
		}

}

// Called when the game starts or when spawned
void APlayerFlight::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerCon = Cast<APlayerController>(GetController());
	
	if (playerCon != nullptr) {
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());


		if (subsys != nullptr) {
			subsys->AddMappingContext(imc_myMapping, 0);
		}
	}

	//���͸��� �迭 0�� �����Ѵ�
	UMaterialInterface* imat = meshComp->GetMaterial(0);
	FHashedMaterialParameterInfo param = FHashedMaterialParameterInfo(TEXT("myColor"));

	//Material interface���� ���� �Ķ���� ���� initcolor ������ ����
	imat->GetVectorParameterValue(param, initColor);
	UE_LOG(LogTemp, Warning, TEXT("R : %f G:%f B:%f"), initColor.R, initColor.G, initColor.B);

	//Material interface�� �̿��ؼ� material instance Dynamic ��ä�� �����Ѵ�. 
	dynamicMat = UMaterialInstanceDynamic::Create(imat, this);
	//my_mat->GetVectorParameterValue(param, (FLinearColor)initColor);
	//������ ���̳��� �޽ø� �����Ѵ�?

	if (dynamicMat != nullptr) {
		meshComp->SetMaterial(0, dynamicMat);
	}

}

// Called every frame
void APlayerFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//����ڰ� �Է��� ������ �̵�
	// p = p0 +vt
	direction.Normalize();
	FVector dir = GetActorLocation() + direction * moveSpeedOrigin * DeltaTime;

	//Block �������� : true �浹�ϴ��� Ȯ�� �ϰ��� false �浿Ȯ�ξ���
	SetActorLocation(dir, true);

}

// Called to bind functionality to input
void APlayerFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//������ UInputComponent* ������ 
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	//�Լ�����
	enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Triggered, this, &APlayerFlight::HorizontalI);
	enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Completed, this, &APlayerFlight::HorizontalI);

	enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Triggered, this, &APlayerFlight::VerticalI);
	enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Completed, this, &APlayerFlight::VerticalI);

	enhancedInputComponent->BindAction(ia_fire, ETriggerEvent::Triggered, this, &APlayerFlight::FireBullet);

	enhancedInputComponent->BindAction(ia_boost, ETriggerEvent::Started, this, &APlayerFlight::StartBoost);
	enhancedInputComponent->BindAction(ia_boost, ETriggerEvent::Completed, this, &APlayerFlight::EndBoost);

	enhancedInputComponent->BindAction(ia_ult, ETriggerEvent::Triggered, this, &APlayerFlight::CheckEnemies);

	//Horizontal Axis �Է¿� �Լ��� �����Ѵ�.
	//PlayerInputComponent->BindAxis("Horizontal", this, &APlayerFlight::HorizontalI);
	//Vertical Axis �Է¿� �Լ��� �����Ѵ�.
	//PlayerInputComponent->BindAxis("Vertical", this, &APlayerFlight::VerticalI);

	//PlayerInputComponent->BindAction("Firing", IE_Pressed, this, &APlayerFlight::FireBullet);

}

void APlayerFlight::ReservationHitColor(float time)
{
	dynamicMat->SetVectorParameterValue(TEXT("myColor"), (FVector4)FLinearColor::Red);
	
	GetWorld()->GetTimerManager().SetTimer(colorTimer, this, &APlayerFlight::ChangeOriginColor, time, false);
}

//�ε����� �� ���� 0.6�ʵ��� �ٲٴ� �Լ�
//void APlayerFlight::ChangeHitColor()
//{
//	UMaterialInstanceDynamic* my_mat = Cast<UMaterialInstanceDynamic>(meshComp->GetMaterial(0));
//
//	my_mat->SetVectorParameterValue(TEXT("myColor"), FLinearColor::Red);
//	//my_mat->SetVectorParameterValue(TEXT("myColor"), FLinearColor(255, 0, 0, 255));
//}

void APlayerFlight::ChangeOriginColor()
{
	dynamicMat->SetVectorParameterValue(TEXT("myColor"), initColor);
}
//void APlayerFlight::HorizontalI(float val)
//{
//	h = val;
//	//UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), h);
//	direction.Y = h;
//}

void APlayerFlight::HorizontalI(const FInputActionValue& val)
{
	h = val.Get<float>();
	//UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), h);
	direction.Y = h;
}

//void APlayerFlight::VerticalI(float val)
//{
//	v = val;
//	//UE_LOG(LogTemp, Warning, TEXT("v : %.4f"), v);
//	direction.Z = v;
//}

void APlayerFlight::VerticalI(const FInputActionValue& val)
{
	v = val.Get<float>();
	direction.Z = v;
}

void APlayerFlight::FireBullet()
{
	if (!canFire) 
	{
		return;
	}
	
	
	//�Ѿ� ����
	//�Ѿ� �������Ʈ ����

	//�Ѿ��� bulletCount ����ŭ ���ÿ� �߻��Ѵ� >> �ݺ���

	for (int32 i = 0; i < bulletCount; i++) {

		//�Ѿ��� ��ü ����
		float totalSize = (bulletCount -1) * bulletSpacing;

		//���� ��ġ
		float base_y = totalSize * -0.5f;

		//���� ��ġ�� ���͸� �߰��Ѵ�(���� ��ġ�� ������ ���͸� �����)
		//�� ������ �Ÿ� : (�Ѿ� ���� - 1) * 150
		FVector offset = FVector(0, base_y + 150 * i, 0); 

		FVector spawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;

		spawnPosition += offset; //ù���� ��ġ

		FRotator spawnRotation = FRotator(90.0f, 0, 0);
		FActorSpawnParameters param;

		//������Ʈ�� �����Ǿ��µ� �̹� ���ڸ��� �ٸ� ������Ʈ�� ��ġ������ ���. �������ڸ��� �浹�� �߻��Ѵ�.
		//�׷� �� ��� �Ұ��� �������ִ� ���̴�	
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Abullet* bullet = GetWorld()->SpawnActor<Abullet>(bulletFactory, spawnPosition, spawnRotation, param);

		//������ �Ѿ�(bullet)�� bulletAngle ��ŭ �����ϰ� ȸ����Ų��
		//ȸ���� FRotator�� ����غ���
		float base_yaw = (bulletCount - 1) * bulletAngle * -0.5f; //������ ���� �������� ���ϴ� ��
		FRotator rot_base = FRotator(0, base_yaw + bulletAngle * i, 0); //������ �������� �󸶳� ��������
						
		
		//nullptr�� �ƴ� ��쿡�� �����Ų��
		if (bullet != nullptr && IsValid((bullet))) 
		{
			bullet->AddActorLocalRotation(rot_base); //���������� ȸ���� ��Ű�� �Լ�
		}
		
				
	}	
			
	//�Ѿ� �߻� ȿ����
	UGameplayStatics::PlaySound2D(this, fireSound);
}

void APlayerFlight::StartBoost()
{
	//�ӵ� ������
	moveSpeedOrigin = moveSpeed * 2;
	UE_LOG(LogTemp, Warning, TEXT("StartBoost"));

}

void APlayerFlight::EndBoost()
{
	//
	moveSpeedOrigin = moveSpeed;
	UE_LOG(LogTemp, Warning, TEXT("EndBoost"));
}

void APlayerFlight::Boost()
{
	isBoosting = !isBoosting;

	if (isBoosting) {
		moveSpeedOrigin = moveSpeed * 2;
	}
	else {
		moveSpeedOrigin = moveSpeed;
	}

	//���׿����ڽ����� �ۼ��ϸ�
	//moveSpeedOrigin = isBoosting ? moveSpeed ? * 2 : moveSpeed;
}

//�ñر� ��ź �Լ�
void APlayerFlight::Explosion()
{
// ��� ���ʹ̸� �ı��Ѵ�
//1. world�� �ִ� ��� ���ʹ̸� ã�� ���ֹ����� (T Itterator�� �̿��� ���)
//UE_LOG(LogTemp, Warning, TEXT("Ult is Working"));

// 	for (TActorIterator<AEnemy> it(GetWorld()); it; ++it) //#include "EngineUtils.h"
// 	{
// 		eTarget = *it;
// 		
// 		if (eTarget != nullptr)
// 		{
// 			eTarget->Destroy();
// 		}
// 	}
// 
// for (TActorIterator<AEnemy>enemy(GetWorld()); enemy; ++enemy) 
// {
// 	enemy->DestroyMySelf();
// }
	

//2. enemy �����Ҷ����� �迭�� �־ �ݺ������� �ѹ��� destroy, 
//   �迭�� ��𿡼��� �����ϱ� ���� ���� �ִ� ���� ���� GameModeBase ���� ��
// �¾���� GameModeBase�� ���� enemy�� GameModeBase ���� �ݺ������� destroy �����ָ� �ȴ�
// GC, VM GarbageCollector �� �޸𸮸� ����
// �÷��̾ �ױ� �� �޸𸮸� ������� ���α׷��� ���������� ����ȴ�.
//�迭 <TArray>�� �̿��� ���
// AMyShootingModeBase * gm = Cast<AMyShootingModeBase>(GetWorld()->GetAuthGameMode()); //��������� ���Ӹ�带 �����´�. ��ȯ�� ���ִ� ��
//    
//     if (gm != nullptr) //����� �����Դٸ� nullptr�� �ƴϱ� ������ ����� ��ȭ�ƴ��� üũ�ϴ� ��
// 	{
// 		TArray<AEnemy*> enemyArr = gm->enemies;
// 		for (int32 i = 0; i < enemyArr.Num(); i++) { // �迭�� ������ŭ �ݺ��Ѵ�
// 			
// 			//Pending Kill ���� üũ, �Ѿ˿����� ũ���ð� ���� ��� PendingKill�� üũ�غ���
// 			if (IsValid(enemyArr[i])) { //��ȿ�ϴٴ� �� pendig kill ���µ� �ƴϰ�(�޸𸮿� �ִµ� ������ ����), nullptr�� �ƴϰ�
// 				enemyArr[i]->DestroyMySelf();
// 			}
// 		}
// 		//����Ʈ�� �� �� �ʱ�ȭ�Ѵ�.
// 		gm->enemies.Empty();
//     }


//3. deligate�� �̿��� �� enemy���� ���� �Լ��� �־�α�
//playerBomb.Broadcast(); //���� �Ű������� ���� ������ ��ȣ���� ��ĭ���� ����д�

OnSetdirection.Broadcast(FVector(0, 1, 0));

}

void APlayerFlight::CheckEnemies() 
{
	
	//������ Enemy���� ������ ���� ������ �迭
	TArray<FOverlapResult> enemiesInfo;

	//Player�κ��� ����
	FVector centerLoc = GetActorLocation() + GetActorUpVector() * 700;

	//GetActorRotation�� ���ʹϾ����� �ٲ۴�
	FQuat centerRot = GetActorRotation().Quaternion();

	//� ������Ʈ�� �浹�Ұ���
	FCollisionObjectQueryParams param = ECC_GameTraceChannel2;

	//� ������� �����Ұ���
	FCollisionShape checkShape = FCollisionShape::MakeSphere(500);

	//�ݰ� 5���� �̳��� �ִ� ��� Enemy ���͵��� �����Ѵ�, (�迭�� ���� �޴´�)
	//Enemy �迭 ����, ���� ��ġ, ���ʹϾ�, � ������Ʈ�� �浹�Ұ���, � ������� ��������, ������ ����Ұ���(���� �ƹ��͵� �ȳ����� default�� �����̴�)
	GetWorld()->OverlapMultiByObjectType(enemiesInfo, centerLoc, centerRot, param, checkShape);
		
	
	//����� ��µǴ��� üũ : üũ�� ��� ���ʹ��� �̸��� ����Ѵ�
	for (FOverlapResult enemyInfo : enemiesInfo) 
	{		
		UE_LOG(LogTemp, Warning, TEXT("Hit : %s"), *enemyInfo.GetActor()->GetName());

		enemyInfo.GetActor()->Destroy();
	}
	
	//������ üũ�� �� �ִ� �׸��� �׸� ���̴� #include "DrawDebugHelpers"
	//��� ȣ���ϴ���, ���� ����, �� ������, ���׸�Ʈ ����, �ѹ��� �׸� ���̴�, ����ð���ŭ �����Ұ���
	DrawDebugSphere(GetWorld(), centerLoc, 500, 20, FColor::Yellow, false, 2);
}


