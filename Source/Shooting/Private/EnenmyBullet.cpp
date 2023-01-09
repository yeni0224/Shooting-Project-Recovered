// Fill out your copyright notice in the Description page of Project Settings.


#include "EnenmyBullet.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "MyShootingModeBase.h"
#include "PlayerFlight.h"

AEnenmyBullet::AEnenmyBullet() {
	boxComp->SetCollisionProfileName(TEXT("Enemy"));
}

void AEnenmyBullet::BeginPlay()
{
	Super::BeginPlay(); //Super�� �ٷ� ���� Ŭ������ �ǹ��Ѵ�

	//�θ���� �������� boxComp
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnenmyBullet::OnOverlap);
}

void AEnenmyBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
		
	//�ε��� ����� �÷��̾���
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);

	//�÷��̾ �����Ѵ�
	//player->Destroy();

	if (player != nullptr) 
	{
		player->Destroy();

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
