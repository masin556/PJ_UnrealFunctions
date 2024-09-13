// LoadStreamLevel을 사용한 비동기로드를 오픈월드에서 사용하기 위해 제작합니다. copy right : ChungSik

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"  // Collision
#include "Kismet/Gameplaystatics.h" // LoadStreamLevel
#include "Blueprint/UserWidget.h" // 위젯 설정
#include "Engine/World.h"  // UWorld를 사용하기 위해 필요
#include "CAsyncLoadStream.generated.h"

UCLASS()
class DISPLAYOPTIONACTOR_API ACAsyncLoadStream : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACAsyncLoadStream();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
/////////////////////////////////////////////////////////////////////////////////////////////
	// BoxCollision함수
	UFUNCTION ()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Level 로딩
	UFUNCTION (BlueprintCallable, Category = "Level")
	void LoadNextLevel();

	// Level 언로드
	UFUNCTION (BlueprintCallable, Category = "Level")
	void UnloadCurrentLevel();

private:
	// 첫번째 BoxCollition
	UPROPERTY (BlueprintCallable, Category = "Collision")
	UBoxComponent* BoxCollision1;

	// 두번째 BoxCollision
	UPROPERTY (BlueprintCallable, Category = "Collision")
	UBoxComponent* BoxCollision2;

	// 로딩 중 표시할 위젯
	UPROPERTY (BlueprintCallable, Category = "UI")
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	// 현재 로드된 위젯 인스턴스
	UUserWidget* LoadingWidget;

	//페이드 인 / 아웃 처리 여부
	UPROPERTY (EditAnywhere, Category = "Fade")
	bool bEnableFade;

	// 이동할 다음 레벨 이름
	UPROPERTY (EditAnywhere, Category = "Level", meta = (AllowedClasses = "World"))
	//FString NextLevelName;
	TSoftObjectPtr<UWorld> NextLevelObject;

	// 현재 레벨 이름
	//FString CurrentLevelName;
	UPROPERTY (EditAnywhere, Category = "Level", meta = (AllowedClasses = "World"))
	TSoftObjectPtr<UWorld> CurrentLevel;


	// Fadein/Out 함수
	//void FFadeIn(float FadeTime, const FColor& FadeColor);
	//void FFadeOut(float FadeTime, const FColor& FadeColor);
	void FadeIn();
	void FadeOut();


	// 레벨 로드 및 언로드 시 이름을 가져오기 위한 함수
	FName GetLevelNameFromAsset(TSoftObjectPtr<UWorld> LevelAsset);
};
