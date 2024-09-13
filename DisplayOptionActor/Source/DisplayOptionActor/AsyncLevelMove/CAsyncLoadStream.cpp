// Fill out your copyright notice in the Description page of Project Settings.


#include "CAsyncLoadStream.h"
#include "GameFramework/Character.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ACAsyncLoadStream::ACAsyncLoadStream()
{
	// BoxCollision 초기화
	BoxCollision1 = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision1"));
	RootComponent = BoxCollision1;

	// BoxCollision2 초기화 및 해당 콜리전을 RootComponent에 
	BoxCollision2 = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision2"));
	BoxCollision2->SetupAttachment(RootComponent);

	// 콜리전 콜백 등록
	BoxCollision1->OnComponentBeginOverlap.AddDynamic(this, &ACAsyncLoadStream::OnOverlapBegin);
	BoxCollision2->OnComponentBeginOverlap.AddDynamic(this, &ACAsyncLoadStream::OnOverlapBegin);

	// 페이드 옵션 기본값
	bEnableFade = true;

	// 만약 FString으로 설정했다면 이곳에 NextLevelName기본값을 설정해주자. 
	// NextLevelName = "NextLevel"; // 다음 레벨 이름 설정
	// CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this); // 현재 레벨 이름 가져오기
}

// Called when the game starts or when spawned
void ACAsyncLoadStream::BeginPlay()
{
	Super::BeginPlay();

}



void ACAsyncLoadStream::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->DisableInput(UGameplayStatics::GetPlayerController(this, 0);
		
		if (bEnableFade)
		{
			FadeOut();
		}
		else
		{
			LoadNextLevel();
		}
	}
}

void ACAsyncLoadStream::LoadNextLevel()
{
	if (LoadingWidgetClass)
	{
		LoadingWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingWidgetClass);
		if (LoadingWidget)
		{
			LoadingWidget->AddToViewport();
		}
	}

	// 현재 레벨 언로드
	UnloadCurrentLevel();

	// 다음 레벨 비동기로드
	FName NextLevelName = GetLevelNameFromAsset(NextLevelObject);
	if (!NextLevelName.IsValid())
	{
		UGameplayStatics::LoadStreamLevel(this, NextLevelName, true, false, FLatentActionInfo());
	}
}

void ACAsyncLoadStream::UnloadCurrentLevel()
{
	FName CurrentLevelName = GetLevelNameFromAsset(CurrentLevel);
	if (!CurrentLevelName.IsNone())
	{
		UGameplayStatics::UnloadStreamLevel(this, CurrentLevelName, FLatentActionInfo(), true);
	}
}

void ACAsyncLoadStream::FadeIn()
{
}

void ACAsyncLoadStream::FadeOut()
{
}

FName ACAsyncLoadStream::GetLevelNameFromAsset(TSoftObjectPtr<UWorld> LevelAsset)
{
	return FName();
}


