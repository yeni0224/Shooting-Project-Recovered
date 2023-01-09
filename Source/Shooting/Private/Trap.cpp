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

	//3. USphere Component로 루트 콜리전을 생성한다
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Trap Collision"));
	SetRootComponent(sphereComp);
	sphereComp->SetSphereRadius(200.f);
	
	//4. UStaticMesh를 루트의 자식 컴포넌트로 생성한다
	sMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sMesh"));
	sMeshComp->SetupAttachment(RootComponent);
	sMeshComp->SetRelativeScale3D(FVector(4.0f)); //크기를 4배 키워준다

	//8. 충돌 프리셋을 "TrapPreset"으로 선언
	sphereComp->SetCollisionProfileName(TEXT("TrapPreset"));
}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();

	//7. Overlap 델리게이트에 InTrap 함수를 연결한다
	//여기서 overlap할 함수의 인자를 f12 눌러서 정의를 찾아 긴 변수 가져오기
	//h에서 intrap함수 인자에 길게 적어준다
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATrap::InTrap); 
	
	//A. 박스에서 나왔을때의 상태에 대한 함수를 만들어준다.
	sphereComp->OnComponentEndOverlap.AddDynamic(this, &ATrap::OutTrap); //this : 자기 자신 클래스 변수를 이야기한다.

}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	//현재 누적 시간(currentTime)이 지정된 시간(teleportTime)에 도달하였다면//
	if (currentTime >= teleportTime) 
	{
		//랜덤한 위치(vector)를 설정한다
		float rand_hor = FMath::RandRange(-500.0f, 500.0f); //좌우로  5M씩
		float rand_ver = FMath::RandRange(300.0f, 1150.0f); //상하로 3M씩, 좌측 최상단 좌표(x, y, z)는 (0, -500, 300)
		FVector random_vec = FVector(-970.0f, rand_hor, rand_ver); //한번에 쓰는 방법

		//값이 미리 정해져있지 않다면 이렇게
		//FVector random_vec;
		//random_vec.Y = rand_hor;
		//random_vec.Z = rand_ver;

		//FVector random_vec = FMath::VRand()*500.f; //유닛벡터(normalize같은)의 형태로 좌표가 나온다, 구체의 형태로 좌표가 설정된다
		//random_vec.X = 0; //여기서는 X좌표가 움직이면 안되므로 0으로 고정시킨다. (sphere에서 circle의 공간 안에서 랜덤으로 위치 뽑기)

		//그 위치로 이동한다
		SetActorLocation(random_vec);

		//(currentTime)시간을 초기화한다 *중요*
		currentTime = 0;
	}
	else //그렇지 않다면
	{
		//시간을 누적한다
		currentTime += DeltaTime;
	}

}

//6. "InTrap" 함수 구현
void ATrap::InTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	//만일, 플레이어인 경우라면..
	APlayerFlight* player = Cast< APlayerFlight>(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("Player in mine"))

	if (player) 
	{
		//플레이어에 있는 CanFire변수를 False로 만든다
		player->canFire = false;

	}

}

//C. "OutTrap" 함수 구현
void ATrap::OutTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//D. 만일, 나가기 직전에 부딪힌 액터가 플레이어라면..
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);

	if (player != nullptr) 
	{
		//E. 플레이어의 canFire변수의 값을 true로 변경한다.
		player->canFire = true;
	}
	
}

