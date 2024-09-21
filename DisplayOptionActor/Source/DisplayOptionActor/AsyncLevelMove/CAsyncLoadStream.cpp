#include "CAsyncLoadStream.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


ACAsyncLoadStream::ACAsyncLoadStream()
{
	BoxCollision1 = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision1"));
	BoxCollision1->SetupAttachment(RootComponent);
	BoxCollision1->OnComponentBeginOverlap.AddDynamic(this, &ACAsyncLoadStream::OnBoxCollision1OverlapBegin);

	BoxCollision2 = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision2"));
	BoxCollision2->SetupAttachment(RootComponent);
	BoxCollision2->OnComponentBeginOverlap.AddDynamic(this, &ACAsyncLoadStream::OnBoxCollision2OverlapBegin);


	bIsLoading = true;
}

void ACAsyncLoadStream::BeginPlay()
{
	Super::BeginPlay();
	// 시작시 불러올 구간
	if (LoadingWidgetClass)
	{
		LoadingWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingWidgetClass);
		if (LoadingWidget)
		{
			LoadingWidget->AddToViewport();
		}
	}
}

void ACAsyncLoadStream::OnBoxCollision1OverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 충돌시 불러올 구간
	SetInputEnabled(false);
	FadeOut(2.0f, FColor::Black);
	LoadStreamLevel(LoadLevelObject);
}

void ACAsyncLoadStream::OnBoxCollision2OverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 충돌시 불러올 구간
	FadeOut(2.0f, FColor::Black);
	LoadStreamLevel(LoadLevelObject);
}

void ACAsyncLoadStream::LoadStreamLevel(TSoftObjectPtr<UWorld> OnLevel)
{
	if (OnLevel.IsValid())
	{
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, OnLevel, true, false, FLatentActionInfo());
		SetInputEnabled(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Level Reference"));
	}
}

void ACAsyncLoadStream::UnLoadStreamLevel(TSoftObjectPtr<UWorld> UnLevel)
{
	//if (!UnloadLevelName.IsEmpty()) //FString으로 받았을 경우
	if (UnLevel.IsValid())
	{
		//UGameplayStatics::UnloadStreamLevel(this, FName(*UnloadLevelName.GetAssetName()), FLatentActionInfo(), true);
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, UnLevel, FLatentActionInfo(), true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Level Reference"));
	}
}

void ACAsyncLoadStream::FadeIn(float FadeTime, const FColor& FadeColor)
{
	PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->ClientSetCameraFade(true, FadeColor, FVector2D(1.0f, 0.0f), FadeTime, false, true);
		UE_LOG(LogTemp, Log, TEXT("FadeIn"));
	}
}

void ACAsyncLoadStream::FadeOut(float FadeTime, const FColor& FadeColor)
{
	PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->ClientSetCameraFade(true, FadeColor, FVector2D(0.0f, 1.0f), FadeTime, false, true);
		UE_LOG(LogTemp, Log, TEXT("FadeOut"));
	}
}

void ACAsyncLoadStream::SetInputEnabled(bool bEnable)
{
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		if (bEnable)
		{
			PlayerController->SetIgnoreMoveInput(false);
			PlayerController->SetIgnoreLookInput(false);
			UE_LOG(LogTemp, Log, TEXT("Input Disabled"));
		}
		else
		{
			PlayerController->SetIgnoreMoveInput(true);
			PlayerController->SetIgnoreLookInput(true);
			UE_LOG(LogTemp, Log, TEXT("Input Enabled (Default)"));
		}
	}
}


