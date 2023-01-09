// Fill out your copyright notice in the Description page of Project Settings.


#include "Playerbullet.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "MyShootingModeBase.h"

APlayerbullet::APlayerbullet() 
{
	boxComp->SetCollisionProfileName(TEXT("BulletPreset"));

}

void APlayerbullet::BeginPlay() 
{	
	Super::BeginPlay(); //Super�� �ٷ� ���� Ŭ������ �ǹ��Ѵ�

	SetDirection(GetActorForwardVector());

	//�θ���� �������� boxComp
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerbullet::OnOverlap);

}

//�浹ü ������Ʈ, ������, , , Hit�� ���õ� ����
void APlayerbullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//include "Enemy.h" �߰��������
	AEnemy* enemy = Cast<AEnemy>(OtherActor); //()�� <>�� ��ȯ�õ� ������ȯ ���� unll

	//���� �ε��� ����� �Ѿ��̶�� (other actor�� enemy���� �ƴ� �� �ִ� > enemy�� ĳ���� �Ͽ� Ȯ�κ��� �Ѵ�)
	if (enemy != nullptr) 
	{
		//���� �ִ� ��ġ�� ���� ����Ʈ�� ����
		//���� �ִ� ��ġ�� ���� ����Ʈ�� �����Ѵ� (�����ϱ� ���� �����ؾ� ���� ��ġ�� �� �� �ִ�)
		FVector enemyLoc = enemy->GetActorLocation();
		FRotator enemyRot = enemy->GetActorRotation();
		//� ����Ʈ�� ������ ������ �������� �޾ƿ��� ���ڴ� >>header

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion_effect, enemyLoc, enemyRot, true);
		//���� �����Ѵ� (������� ���� �����Ѵ�) , ��� ������Ʈ�� Destroy �Լ��� �����ִ�
		enemy->Destroy();

		//���Ӹ���� CurrentScore �߰��Ѵ� ���Ӹ��� � Ŭ���������� �ٷ� ������ �� �ֵ���.
		//�� �ڽ��� ������� ���� GameMode�� ���� 1���� �߰��Ѵ�.(GameMode > current ����)
		//���������� ������ ������ ���� ����
		AGameModeBase* gm = UGameplayStatics::GetGameMode(this);
		//AGameModeBase* gm = GetWorld()->GetAuthGameMode();

		AMyShootingModeBase* myGM = Cast<AMyShootingModeBase>(gm);
		myGM->AddScore(10);
		//UE_LOG(LogTemp, Warning, TEXT("Point: %d"), myGM->GetCurrentScore());


		//�� �ڽ��� �����Ѵ�
		Destroy();
	}
}