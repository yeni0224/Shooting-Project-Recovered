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

		//박스 콜리전의 총돌 처리 프리셋을 "PlayerPreset"으로 설정한다.
		boxComp->SetCollisionProfileName(TEXT("PlayerPreset"));

		//메시 컴포넌트의 Static mesh 항목의 큐브 파일을 할당한다.
		ConstructorHelpers::FObjectFinder<UStaticMesh> triMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_TriPyramid.Shape_TriPyramid'"));

		//만일 큐브 파일을 로드하는데 성고앻ㅆ다면
		if (triMesh.Succeeded())
		{
			//로드한 파일을 메시 컴포넌트의 static mesh 항목에 넣는다
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

	//메터리얼 배열 0을 저장한다
	UMaterialInterface* imat = meshComp->GetMaterial(0);
	FHashedMaterialParameterInfo param = FHashedMaterialParameterInfo(TEXT("myColor"));

	//Material interface에서 백터 파라미터 값을 initcolor 변수에 저장
	imat->GetVectorParameterValue(param, initColor);
	UE_LOG(LogTemp, Warning, TEXT("R : %f G:%f B:%f"), initColor.R, initColor.G, initColor.B);

	//Material interface를 이용해서 material instance Dynamic 개채를 생성한다. 
	dynamicMat = UMaterialInstanceDynamic::Create(imat, this);
	//my_mat->GetVectorParameterValue(param, (FLinearColor)initColor);
	//생성한 다이나믹 메시를 생성한다?

	if (dynamicMat != nullptr) {
		meshComp->SetMaterial(0, dynamicMat);
	}

}

// Called every frame
void APlayerFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//사용자가 입력한 방향대로 이동
	// p = p0 +vt
	direction.Normalize();
	FVector dir = GetActorLocation() + direction * moveSpeedOrigin * DeltaTime;

	//Block 물리에서 : true 충돌하는지 확인 하고간다 false 충동확인안함
	SetActorLocation(dir, true);

}

// Called to bind functionality to input
void APlayerFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//기존의 UInputComponent* 변수를 
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	//함수연결
	enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Triggered, this, &APlayerFlight::HorizontalI);
	enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Completed, this, &APlayerFlight::HorizontalI);

	enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Triggered, this, &APlayerFlight::VerticalI);
	enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Completed, this, &APlayerFlight::VerticalI);

	enhancedInputComponent->BindAction(ia_fire, ETriggerEvent::Triggered, this, &APlayerFlight::FireBullet);

	enhancedInputComponent->BindAction(ia_boost, ETriggerEvent::Started, this, &APlayerFlight::StartBoost);
	enhancedInputComponent->BindAction(ia_boost, ETriggerEvent::Completed, this, &APlayerFlight::EndBoost);

	enhancedInputComponent->BindAction(ia_ult, ETriggerEvent::Triggered, this, &APlayerFlight::CheckEnemies);

	//Horizontal Axis 입력에 함수를 연결한다.
	//PlayerInputComponent->BindAxis("Horizontal", this, &APlayerFlight::HorizontalI);
	//Vertical Axis 입력에 함수를 연결한다.
	//PlayerInputComponent->BindAxis("Vertical", this, &APlayerFlight::VerticalI);

	//PlayerInputComponent->BindAction("Firing", IE_Pressed, this, &APlayerFlight::FireBullet);

}

void APlayerFlight::ReservationHitColor(float time)
{
	dynamicMat->SetVectorParameterValue(TEXT("myColor"), (FVector4)FLinearColor::Red);
	
	GetWorld()->GetTimerManager().SetTimer(colorTimer, this, &APlayerFlight::ChangeOriginColor, time, false);
}

//부딪혔을 때 색을 0.6초동안 바꾸는 함수
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
	
	
	//총알 스폰
	//총알 블루프린트 변수

	//총알을 bulletCount 수만큼 동시에 발사한다 >> 반복문

	for (int32 i = 0; i < bulletCount; i++) {

		//총알의 전체 간격
		float totalSize = (bulletCount -1) * bulletSpacing;

		//기준 위치
		float base_y = totalSize * -0.5f;

		//기준 위치로 벡터를 추가한다(기준 위치로 오프셋 벡터를 만든다)
		//총 간격의 거리 : (총알 갯수 - 1) * 150
		FVector offset = FVector(0, base_y + 150 * i, 0); 

		FVector spawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;

		spawnPosition += offset; //첫발의 위치

		FRotator spawnRotation = FRotator(90.0f, 0, 0);
		FActorSpawnParameters param;

		//오브젝트가 생성되었는데 이미 이자리에 다른 오브젝트가 위치해있을 경우. 생성되자마자 충돌이 발생한다.
		//그럴 때 어떻게 할건지 설정해주는 것이다	
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Abullet* bullet = GetWorld()->SpawnActor<Abullet>(bulletFactory, spawnPosition, spawnRotation, param);

		//생성된 총알(bullet)을 bulletAngle 만큼 일정하게 회전시킨다
		//회전은 FRotator을 사용해볼것
		float base_yaw = (bulletCount - 1) * bulletAngle * -0.5f; //각도의 시작 기준점을 구하는 것
		FRotator rot_base = FRotator(0, base_yaw + bulletAngle * i, 0); //기준점 각도에서 얼마나 더해질지
						
		
		//nullptr이 아닌 경우에만 실행시킨다
		if (bullet != nullptr && IsValid((bullet))) 
		{
			bullet->AddActorLocalRotation(rot_base); //실직적으로 회전을 시키는 함수
		}
		
				
	}	
			
	//총알 발사 효과음
	UGameplayStatics::PlaySound2D(this, fireSound);
}

void APlayerFlight::StartBoost()
{
	//속도 빠르게
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

	//삼항연산자식으로 작성하면
	//moveSpeedOrigin = isBoosting ? moveSpeed ? * 2 : moveSpeed;
}

//궁극기 폭탄 함수
void APlayerFlight::Explosion()
{
// 모든 에너미를 파괴한다
//1. world에 있는 모든 에너미를 찾아 없애버린다 (T Itterator을 이용한 방식)
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
	

//2. enemy 생성할때마다 배열에 넣어서 반복문으로 한번에 destroy, 
//   배열은 어디에서나 접근하기 쉬운 곳에 넣는 것이 좋다 GameModeBase 같은 곳
// 태어나마자 GameModeBase에 들어가면 enemy가 GameModeBase 안의 반복문에서 destroy 시켜주면 된다
// GC, VM GarbageCollector 로 메모리를 정리
// 플레이어가 죽기 전 메모리를 지워줘야 프로그램이 안정적으로 진행된다.
//배열 <TArray>를 이용한 방식
// AMyShootingModeBase * gm = Cast<AMyShootingModeBase>(GetWorld()->GetAuthGameMode()); //현재상태의 게임모드를 가져온다. 변환을 해주는 것
//    
//     if (gm != nullptr) //제대로 가져왔다면 nullptr이 아니기 때문에 제대로 변화됐는지 체크하는 것
// 	{
// 		TArray<AEnemy*> enemyArr = gm->enemies;
// 		for (int32 i = 0; i < enemyArr.Num(); i++) { // 배열의 갯수만큼 반복한다
// 			
// 			//Pending Kill 상태 체크, 총알에서도 크래시가 나는 경우 PendingKill을 체크해볼것
// 			if (IsValid(enemyArr[i])) { //유효하다는 건 pendig kill 상태도 아니고(메모리에 있는데 지워진 상태), nullptr도 아니고
// 				enemyArr[i]->DestroyMySelf();
// 			}
// 		}
// 		//리스트를 한 번 초기화한다.
// 		gm->enemies.Empty();
//     }


//3. deligate를 이용해 각 enemy마다 자폭 함수를 넣어두기
//playerBomb.Broadcast(); //현재 매개변수가 없기 때문에 괄호안을 빈칸으로 비워둔다

OnSetdirection.Broadcast(FVector(0, 1, 0));

}

void APlayerFlight::CheckEnemies() 
{
	
	//감지된 Enemy들의 정보를 담을 변수의 배열
	TArray<FOverlapResult> enemiesInfo;

	//Player로부터 범위
	FVector centerLoc = GetActorLocation() + GetActorUpVector() * 700;

	//GetActorRotation을 쿼터니언으로 바꾼다
	FQuat centerRot = GetActorRotation().Quaternion();

	//어떤 오브젝트와 충돌할건지
	FCollisionObjectQueryParams param = ECC_GameTraceChannel2;

	//어떤 모양으로 감지할건지
	FCollisionShape checkShape = FCollisionShape::MakeSphere(500);

	//반경 5미터 이내에 있는 모든 Enemy 액터들을 감지한다, (배열로 값을 받는다)
	//Enemy 배열 정보, 감지 위치, 쿼터니언, 어떤 오브젝트와 충돌할건지, 어떤 모양으로 감지할지, 응답은 어떻게할건지(값을 아무것도 안넣으면 default로 들어갈것이다)
	GetWorld()->OverlapMultiByObjectType(enemiesInfo, centerLoc, centerRot, param, checkShape);
		
	
	//제대로 출력되는지 체크 : 체크된 모든 에너미의 이름을 출력한다
	for (FOverlapResult enemyInfo : enemiesInfo) 
	{		
		UE_LOG(LogTemp, Warning, TEXT("Hit : %s"), *enemyInfo.GetActor()->GetName());

		enemyInfo.GetActor()->Destroy();
	}
	
	//범위를 체크할 수 있는 그림을 그릴 것이다 #include "DrawDebugHelpers"
	//어디서 호출하는지, 센터 지정, 구 반지름, 세그먼트 갯수, 한번만 그릴 것이다, 어느시간만큼 유지할건지
	DrawDebugSphere(GetWorld(), centerLoc, 500, 20, FColor::Yellow, false, 2);
}


