// Copyright (c) 2024 iamsince1998. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "AAA_HealthSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCurrentHealthChanged,bool,bIsDead, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMaxHealthChanged,float,NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateHealthBar,bool,bIsDead, float, NewProgressBarValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateMaxHealthBar,bool,bIsDead, float, NewMaxProgressBarValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AAA_HEALTHSYSTEM_API UAAA_HealthSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAAA_HealthSystemComponent();

	//Current Health for this Target blueprint class
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "AAA|Health")
	float Health = 100.f;
	//Max Health for this Target blueprint class(when you add more health , current health can go up to this value)
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "AAA|Health")
	float MaxHealth = 100.f;
	//You will get Health ProgressBar Value based on this value [1/100] (you want it normal as 100% or value of your choice) 
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "AAA|Health")
	float HealthBarPercentage = 100.f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Replication setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Get Current Health For this Target Blueprint Class
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "AAA|Health")
	float GetCurrentHealth();
	//Set Current Health For this Target Blueprint Class(By Doing this you are Overriding the HealthValue)
	UFUNCTION(BlueprintCallable, Category = "AAA|Health")
	void SetCurrentHealth(float SetHealth);
	//Get Current Health For this Target Blueprint Class
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "AAA|Health")
	float GetMaxHealth();
	//Set Max Health For this Target Blueprint Class(By Doing this you are Overriding the MaxHealthValue)
	UFUNCTION(BlueprintCallable, Category = "AAA|Health")
	void SetMaxHealth(float SetHealth);
	//Add Health in Current Health For this Target Blueprint Class(Check if you want to exceed the max health value)
	UFUNCTION(BlueprintCallable, Category = "AAA|Health")
	void AddHealth(bool bOverrideMaxHealth,float ValueToAdd,float& NewHealth);
	//Add in MaxHealth in Current MaxHealth For this Target Blueprint Class
	UFUNCTION(BlueprintCallable, Category = "AAA|Health")
	void AddMaxHealth(float ValueToAdd,float& NewMaxHealth);
	//Reduce Health from Current Health For this Target Blueprint Class(if IsDead means health is Zero or less than zero)
	UFUNCTION(BlueprintCallable, Category = "AAA|Health")
	void ReduceHealth(bool& bIsDead,float ValueToReduce,float& NewHealth);
	//Reduce Health from Current Health For this Target Blueprint Class
	UFUNCTION(BlueprintCallable, Category = "AAA|Health")
	void ReduceMaxHealth(float ValueToReduce,float& NewMaxHealth);
	//Get Health value in % for Progress bar ( this will return 1 if your health is 100 and HealthBarValueBasedOnPercentage is 100%)
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "AAA|Health")
	float GetHealthBarValue( );
	//Get MaxHealth value in % for Progress bar ( this will return 1 if your Maxhealth is 100 and HealthBarValueBasedOnPercentage is 100%)
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "AAA|Health")
	float GetMaxHealthBarValue( );
	//check if player is dead or not , based on the health (Return true if health is 0 or less then zero)
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "AAA|Health")
	bool GetIsDead( );


	
	//This Event will be Called When Healh Value will be Updated
	UPROPERTY(BlueprintAssignable, Category = "AAA|Health")
	FCurrentHealthChanged OnCurrentHealthChanged;
	//This Event will be Called When MaxHealh Value will be Updated
	UPROPERTY(BlueprintAssignable, Category = "AAA|Health")
	FMaxHealthChanged OnMaxHealthChanged;
	//This Event will be Called When Health value will be updated, this will return 1 if your health is 100 and HealthBarPercentage is 100% , so you can directly assign it to a progress bar
	UPROPERTY(BlueprintAssignable, Category = "AAA|Health")
	FUpdateHealthBar OnHealthBarUpdated;
	//This Event will be Called When MaxHealth value will be updated, this will return 1 if your Maxhealth is 100 and HealthBarPercentage is 100% , so you can directly assign it to a progress bar
	UPROPERTY(BlueprintAssignable, Category = "AAA|Health")
	FUpdateMaxHealthBar OnMaxHealthBarUpdated;
};
