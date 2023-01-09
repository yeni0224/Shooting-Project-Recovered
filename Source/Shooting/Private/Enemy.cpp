// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerFlight.h"
#include "EngineUtils.h"
#include "MyShootingModeBase.h"
#include "MyShootingLibrary.h"

// Sets default values 생성하기전 before spawn 
AEnemy::AEnemy()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f));

	/*충돌 프리셋 설정하기*/
	//Collision Enable 값을 Query and Physics로 설정한다.
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//응답 채널의 Object Type 을 "Enemy"로 설정한다.
	//프로젝트 config - DefaultEngine - ctrl F 콜리전이름
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel2);

	//응답채널을 일괄적으로 ignore 상태로 처리한다
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	//응탑채널을 Player와 Bullet 채널에 대해서만 overlap으로 처리한다.
	//>> 전체를 ignore로 밀고 overlap으로 해야 코드를 줄일 수 있음. 순서 중요!
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
	//콜리전설정을 C++로하면 블루프린트 클래스를 다시만들어야 적용된다.

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(RootComponent);

	//위치 조정
	meshComp->SetRelativeLocation(FVector(0, 0, -50.0f));

}

// Called when the game starts or when spawned 생성한후
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//충돌처리
	//boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlap);

	//추첨을 통해서 하나는 정면 방향, 또 다른 하나는 플레이어 방향으로 이동하고싶다
	//1. 추첨을 한다. 확률은 변수를 이용해서 7:3 비율로 한다.
	int32 drawNumber = FMath::RandRange(1, 100);
	//fmath 클래스에 있는 randrange함수, 보통 확률을 잡을때 0~1까지의 실수를 가지고 계산한다

	//2. 만일, 뽑은 값이 30보다 작으면...
	if (drawNumber <= traceRate)
	{

		//2-1. 플레이어 액터를 찾는다 : 레벨에 생성된BP의 플레이어를 찾아야하므로, 단순히 방향만 정할것이므로 h에 지역변수 선언하기.

		//BP에서 배치된 다른 플레이어 찾을 때 (월드에서 특정한 개체를 찾는 방법 1):
		//반환값은 Actor, 월드 함수를 인자로 받는다, 찾으려는 대상(APlayerFlight.h도 추가해줘야함)	
		//AActor* target = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerFlight::StaticClass());


		//2-2. 플레이어의 위치(내가 가고자하는 방향) - 나의 위치 = 갈 방향을 설정한다

		//월드에서 특정한 개체를 찾는 방법 1
		//액터만 가져오면 위치 자체를 쓸 수 있기 때문에
		//FVector targetdir = target->GetActorLocation() - GetActorLocation();
		//direction = targetdir;
		//이렇게 하면 각각의 거리가 천차만별 차이가 나게 된다. 방향이 어디가 되었든 몸통의 길이가 단위벡터 1짜리로 초기화시켜줘야한다		


		//월드에서 특정한 개체를 찾는 방법2
		//APlayerFlight* target; << 얘를 전역변수로 만들어주기 위해 h로 빼낸다

		// AplayerFlight에서 사용한다
		//해석 : World에 배치된 모든 배열들을 Iterator라는 형식에 넣었다, 하나씩 꺼냈을 때 null값이 아니라면, 반환 할때마다 하나씩 더해지는
		for (TActorIterator<APlayerFlight> it(GetWorld()); it; ++it)
		{
			target = *it;
		}
		//안정화를 위해서
		if (target != nullptr)
		{
			float temp = target->moveSpeed;

			FVector targetdir = target->GetActorLocation() - GetActorLocation();
			targetdir.Normalize();
			direction = targetdir;
			//player이 없는 상태에서 enemy가 발생할 때 크래시 방지를 위해서

			target->playerBomb.AddDynamic(this, &AEnemy::DestroyMySelf);
		}
	}

	//3. 그렇지않으면
	else
	{
		//3-1. 정면으로 방향을 정한다
		direction = GetActorForwardVector();
	}

	//충돌이 발생하면 실행할 함수 연결
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlap);

	boxComp->SetGenerateOverlapEvents(true);

	//게임모드에 enemies 배열에 자기 자신을 넣는다
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

	// 나의 정면 방향으로 계속 이동하고 싶다. 위에서 선언하였으므로 지워도 된다
    //direction = GetActorForwardVector();

    //위치 이동, ForwardVector은 normalize가 필요하지 않다
	//SetActorLocation(GetActorLocation() + direction * EnemySpeed * DeltaTime);

	//우리가 만든 라이브러리 함수를 이용해서 이동시킨다
	UMyShootingLibrary::MoveMyActor(this, direction, EnemySpeed, DeltaTime);

}

void AEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);

	if (player != nullptr) {
		//player->Destroy();

		//메뉴 위젯을 생성하는 함수를 실행한다
		AMyShootingModeBase* gm = Cast<AMyShootingModeBase>(GetWorld()->GetAuthGameMode());

		if (gm != nullptr) 
		{
			gm->ShowMenu();
		}

		//플레이어가 죽으면 메뉴창이 뜨도록 할 것이므로 필요없어짐
		//player->ReservationHitColor(0.2f);

		Destroy();
	}

}

void AEnemy::DestroyMySelf()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosion_fx, GetActorLocation(), GetActorRotation(), true);
	Destroy();
}


void AEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason) //액터의 함수를 가져다 쓰는 것
{
//죽는 순간 자기 자신을 배열에서 제거한다

// 	AMyShootingModeBase* gm = Cast<AMyShootingModeBase>(GetWorld()->GetAuthGameMode());
// 
// 	if (gm != nullptr) {
// 		gm->enemies.Remove(this);
// 	}
// 
//델리게이트에 걸어놓은 자기 함수를 제거한다
	//target->playerBomb.RemoveDynamic(this, AEnemy::DestroyMySelf());
}


void AEnemy::SetNewDirection(FVector newDir)
{
	//이동 방향을 newDir로 바꾼다
	direction = newDir;

}

