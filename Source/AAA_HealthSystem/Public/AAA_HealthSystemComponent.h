// Copyright (c) 2025 iamsince1998. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "AAA_HealthSystemComponent.generated.h"

#pragma region Delegates

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCurrentHealthChanged,bool,bIsDead, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMaxHealthChanged,float,NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateHealthBar,bool,bIsDead, float, NewProgressBarValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateMaxHealthBar,bool,bIsDead, float, NewMaxProgressBarValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentArmorChanged, float, NewArmor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMaxArmorChanged,float,NewMaxArmor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateArmorBar, float, NewProgressBarValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateMaxArmorBar,bool,bIsDead, float, NewMaxProgressBarValue);

#pragma endregion

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable,BlueprintType)
class AAA_HEALTHSYSTEM_API UAAA_HealthSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAAA_HealthSystemComponent();

#pragma region Variables
	//Current Health for this Target blueprint class
	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadWrite, EditAnywhere, Category = "AAA|Health")
	float Health = 100.f;
	//Max Health for this Target blueprint class(when you add more health , current health can go up to this value)
	UPROPERTY(ReplicatedUsing=OnRep_MaxHealth, BlueprintReadWrite, EditAnywhere, Category = "AAA|Health")
	float MaxHealth = 100.f;
	//You will get Health ProgressBar Value based on this value [1/100] (you want it normal as 100% or value of your choice) 
	UPROPERTY(ReplicatedUsing=OnRep_HealthBarPercentage, BlueprintReadWrite, EditAnywhere, Category = "AAA|Health")
	float HealthBarPercentage = 100.f;
	//to use armor make sure you enable it
	UPROPERTY(ReplicatedUsing=OnRep_bHasArmor, BlueprintReadWrite, EditAnywhere, Category = "AAA|Health|Armor")
	bool bHasArmor = false;
	//use this if you want to reduce armor faster than health(Default value is 1 , so it will reduce normally)
	UPROPERTY(ReplicatedUsing=OnRep_ArmorDamageMultiplier, BlueprintReadWrite, EditAnywhere, Category = "AAA|Health|Armor")
	float ArmorDamageMultiplier = 1.0f;
	//if you want to Regenerate armor with health enable it(if you HasArmor)
	UPROPERTY(ReplicatedUsing=OnRep_bRegenerateArmor, BlueprintReadWrite, EditAnywhere, Category = "AAA|Health|Armor")
	bool bRegenerateArmor = false;
	//Current Armor for this Target blueprint class
	UPROPERTY(ReplicatedUsing=OnRep_Armor, BlueprintReadWrite, EditAnywhere, Category = "AAA|Health|Armor")
	float Armor = 100.f;
	//Max Armor for this Target blueprint class(when you add more Armor , current Armor can go up to this value)
	UPROPERTY(ReplicatedUsing=OnRep_MaxArmor, BlueprintReadWrite, EditAnywhere, Category = "AAA|Health|Armor")
	float MaxArmor = 100.f;
	//You will get Armor ProgressBar Value based on this value [1/100] (you want it normal as 100% or value of your choice) 
	UPROPERTY(ReplicatedUsing=OnRep_ArmorBarPercentage, BlueprintReadWrite, EditAnywhere, Category = "AAA|Health|Armor")
	float ArmorBarPercentage = 100.f;

#pragma endregion

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Replication setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region Functions

	//Get Current Health For this Target Blueprint Class
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "AAA|Health")
	float GetCurrentHealth();
	//Get Current Armor For this Target Blueprint Class
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "AAA|Health|Armor")
	float GetCurrentArmor();
	//Set Current Health For this Target Blueprint Class(By Doing this you are Overriding the HealthValue)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AAA|Health")
	void SetCurrentHealth(float SetHealth);
	//Set Current Armor For this Target Blueprint Class(By Doing this you are Overriding the ArmorValue)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AAA|Health|Armor")
	void SetCurrentArmor(float SetArmor);
	//Get Current Health For this Target Blueprint Class
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "AAA|Health")
	float GetMaxHealth();
	//Set Max Health For this Target Blueprint Class(By Doing this you are Overriding the MaxHealthValue)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AAA|Health")
	void SetMaxHealth(float SetHealth);
	//Add Health in Current Health For this Target Blueprint Class(Check if you want to exceed the max health value)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AAA|Health")
	void AddHealth(bool bOverrideMaxHealth,float ValueToAdd,float& NewHealth);
	//Add in MaxHealth in Current MaxHealth For this Target Blueprint Class
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AAA|Health")
	void AddMaxHealth(float ValueToAdd,float& NewMaxHealth);
	//Reduce Health from Current Health For this Target Blueprint Class(if IsDead means health is Zero or less than zero)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AAA|Health")
	void ReduceHealth(bool& bIsDead,float ValueToReduce,float& NewHealth);
	//Reduce Health from Current Health For this Target Blueprint Class
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AAA|Health")
	void ReduceMaxHealth( float ValueToReduce, float& NewMaxHealth);
	//Regenerate health and Armor (This Will Regin Health to MaxHealth and Armor to Max Armor (if HasArmor))
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AAA|Health")
	void RegenerateHealth( float InTime, float HealthToRegin) ;
	//Get Health value in % for Progress bar ( this will return 1 if your health is 100 and HealthBarValueBasedOnPercentage is 100%)
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "AAA|Health")
	float GetHealthBarValue( );
	//Get Armor value in % for Progress bar ( this will return 1 if your Armor is 100 and ArmorBarValueBasedOnPercentage is 100%)
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "AAA|Health|Armor")
	float GetArmorBarValue( );
	//Get MaxHealth value in % for Progress bar ( this will return 1 if your Max health is 100 and HealthBarValueBasedOnPercentage is 100%)
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "AAA|Health")
	float GetMaxHealthBarValue( );
	//Get MaxArmor value in % for Progress bar ( this will return 1 if your MaxArmor is 100 and ArmorBarValueBasedOnPercentage is 100%)
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "AAA|Health|Armor")
	float GetMaxArmorBarValue( );
	//check if player is dead or not , based on the health (Return true if health is 0 or less than zero)
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "AAA|Health")
	bool GetIsDead( );

#pragma endregion
	
#pragma region AAA_Callbacks

	//This Event will be Called When Health Value will be Updated
	UPROPERTY(BlueprintAssignable, Category = "AAA|Health")
	FCurrentHealthChanged OnCurrentHealthChanged;
	//This Event will be Called When MaxHealth Value will be Updated
	UPROPERTY(BlueprintAssignable, Category = "AAA|Health")
	FMaxHealthChanged OnMaxHealthChanged;
	//This Event will be Called When Health value will be updated, this will return 1 if your health is 100 and HealthBarPercentage is 100% , so you can directly assign it to a progress bar
	UPROPERTY(BlueprintAssignable, Category = "AAA|Health")
	FUpdateHealthBar OnHealthBarUpdated;
	//This Event will be Called When MaxHealth value will be updated, this will return 1 if your Max health is 100 and HealthBarPercentage is 100% , so you can directly assign it to a progress bar
	UPROPERTY(BlueprintAssignable, Category = "AAA|Health")
	FUpdateMaxHealthBar OnMaxHealthBarUpdated;
	//This Event will be Called When Armor Value will be Updated
	UPROPERTY(BlueprintAssignable, Category = "AAA|Health|Armor")
	FCurrentArmorChanged OnCurrentArmorChanged;
	//This Event will be Called When MaxArmor Value will be Updated
	UPROPERTY(BlueprintAssignable, Category = "AAA|Health|Armor")
	FMaxArmorChanged OnMaxArmorChanged;
	//This Event will be Called When Armor value will be updated, this will return 1 if your Armor is 100 and ArmorBarPercentage is 100% , so you can directly assign it to a progress bar
	UPROPERTY(BlueprintAssignable, Category = "AAA|Health|Armor")
	FUpdateArmorBar OnArmorBarUpdated;
	//This Event will be Called When MaxArmor value will be updated, this will return 1 if your MaxArmor is 100 and ArmorBarPercentage is 100% , so you can directly assign it to a progress bar
	UPROPERTY(BlueprintAssignable, Category = "AAA|Health|Armor")
	FUpdateMaxArmorBar OnMaxArmorBarUpdated;

#pragma endregion

private:
	UFUNCTION()
	void OnRep_Health();

	UFUNCTION()
	void OnRep_MaxHealth();

	UFUNCTION()
	void OnRep_HealthBarPercentage();

	UFUNCTION()
	void OnRep_bHasArmor();

	UFUNCTION()
	void OnRep_ArmorDamageMultiplier();

	UFUNCTION()
	void OnRep_bRegenerateArmor();

	UFUNCTION()
	void OnRep_Armor();

	UFUNCTION()
	void OnRep_MaxArmor();

	UFUNCTION()
	void OnRep_ArmorBarPercentage();

	UFUNCTION(Server, Reliable)
	void ServerSetCurrentHealth(float SetHealth);

	UFUNCTION(Server, Reliable)
	void ServerSetCurrentArmor(float SetArmor);

	UFUNCTION(Server, Reliable)
	void ServerSetMaxHealth(float SetHealth);

	UFUNCTION(Server, Reliable)
	void ServerAddHealth(bool bOverrideMaxHealth, float ValueToAdd);

	UFUNCTION(Server, Reliable)
	void ServerAddMaxHealth(float ValueToAdd);

	UFUNCTION(Server, Reliable)
	void ServerReduceHealth(float ValueToReduce);

	UFUNCTION(Server, Reliable)
	void ServerReduceMaxHealth(float ValueToReduce);

	UFUNCTION(Server, Reliable)
	void ServerRegenerateHealth(float InTime, float HealthToRegin);

	void ApplySetCurrentHealth(float SetHealth);
	void ApplySetCurrentArmor(float SetArmor);
	void ApplySetMaxHealth(float SetHealth);
	void ApplyAddHealth(bool bOverrideMaxHealth, float ValueToAdd);
	void ApplyAddMaxHealth(float ValueToAdd);
	void ApplyReduceHealth(float ValueToReduce, bool& bIsDead);
	void ApplyReduceMaxHealth(float ValueToReduce);
	void BroadcastHealthUpdated();
	void BroadcastArmorUpdated();
	void BroadcastMaxHealthUpdated();
	void BroadcastMaxArmorUpdated();


	// Timer handle for managing the timer
	FTimerHandle AAATimerHandle;

};
