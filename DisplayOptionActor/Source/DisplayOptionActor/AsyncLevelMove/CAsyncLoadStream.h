// LoadStreamLevel을 사용한 비동기로드를 오픈월드에서 사용하기 위해 제작합니다. copy right : ChungSik

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAsyncLoadStream.generated.h"

UCLASS()
class DISPLAYOPTIONACTOR_API ACAsyncLoadStream : public AActor
{
	GENERATED_BODY()

public:
	ACAsyncLoadStream();

protected:
	virtual void BeginPlay() override;

public:
	/*BoxComponent*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* BoxCollision1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* BoxCollision2;

	/*Blueprint Set*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelSettings")
	TSoftObjectPtr<UWorld> LoadLevelObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelSettings")
	TSoftObjectPtr<UWorld> UnLoadLevelObject;

	// 페이드인 효과 사용여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FadeSettings")
	bool bUseFadeEffect;

	// 위젯 블루프린트 셋팅
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UISettings")
	TSubclassOf<class UUserWidget> LoadingWidgetClass;

	// Event Function
	UFUNCTION()
	void OnBoxCollision1OverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	
	UFUNCTION()
	void OnBoxCollision2OverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	class UUserWidget* LoadingWidget;

public:
	UPROPERTY()
	APlayerController* PlayerController;


private:
	void LoadStreamLevel(TSoftObjectPtr<UWorld> OnLevel);
	void UnLoadStreamLevel(TSoftObjectPtr<UWorld> UnLevel);
	void FadeIn(float FadeTime, const FColor& FadeColor);
	void FadeOut(float FadeTime, const FColor& FadeColor);

	void SetInputEnabled(bool bEnable);
private:
	bool bIsLoading; // 현재 맵 로딩 중인지 확인
	bool bIsInputDisabled; // 입력 비활성화 상태를 추적하는 변수

};