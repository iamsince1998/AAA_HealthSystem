// Copyright (c) 2025 iamsince1998. All Rights Reserved.


#include "AAA_HealthSystemComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UAAA_HealthSystemComponent::UAAA_HealthSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;
	bEditableWhenInherited = true;
	// ...
}


// Called when the game starts
void UAAA_HealthSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAAA_HealthSystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Specify the variable to replicate
	DOREPLIFETIME(UAAA_HealthSystemComponent, Health);
	DOREPLIFETIME(UAAA_HealthSystemComponent, MaxHealth);
	DOREPLIFETIME(UAAA_HealthSystemComponent, HealthBarPercentage);
}


// Called every frame
void UAAA_HealthSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UAAA_HealthSystemComponent::GetCurrentHealth()
{
	return Health;
}

float UAAA_HealthSystemComponent::GetCurrentArmor()
{
	return Armor;
}

void UAAA_HealthSystemComponent::SetCurrentHealth(float SetHealth)
{
	Health = SetHealth;
	OnCurrentHealthChanged.Broadcast( GetIsDead(),Health);
	OnHealthBarUpdated.Broadcast( GetIsDead(), GetHealthBarValue());
}

void UAAA_HealthSystemComponent::SetCurrentArmor(float SetArmor)
{
	Armor = SetArmor;
	OnCurrentArmorChanged.Broadcast(Armor);
	OnArmorBarUpdated.Broadcast(GetArmorBarValue());
}

float UAAA_HealthSystemComponent::GetMaxHealth()
{
	return MaxHealth;
}

void UAAA_HealthSystemComponent::SetMaxHealth(float SetHealth)
{
	MaxHealth = SetHealth;
	OnMaxHealthChanged.Broadcast(MaxHealth);
}

void UAAA_HealthSystemComponent::AddHealth(bool bOverrideMaxHealth,float ValueToAdd, float& NewHealth)
{
	if (bOverrideMaxHealth)
	{
		Health = Health + ValueToAdd;
	}
	else
	{
		if (Health<MaxHealth)
		{
			if (MaxHealth - Health <= ValueToAdd)
			{
				Health = Health + ValueToAdd;
			}
			else
			{
				Health = MaxHealth;
			}
		}
	}
	NewHealth = Health;
	OnCurrentHealthChanged.Broadcast(GetIsDead(), Health);
	OnHealthBarUpdated.Broadcast(GetIsDead(), GetHealthBarValue());
}

void UAAA_HealthSystemComponent::AddMaxHealth( float ValueToAdd, float& NewMaxHealth)
{
	MaxHealth = MaxHealth + ValueToAdd;
	NewMaxHealth = MaxHealth;
	OnMaxHealthChanged.Broadcast(MaxHealth);
	OnMaxHealthBarUpdated.Broadcast(GetIsDead(), GetMaxHealthBarValue());
}

void UAAA_HealthSystemComponent::ReduceHealth(bool& bIsDead, float ValueToReduce, float& NewHealth)
{
	if (bHasArmor)
	{
		//if only armor is enough to contain that damage we will return from here
		if (Armor > 0 && Armor >= ValueToReduce * ArmorDamageMultiplier)
		{
				Armor -= ValueToReduce * ArmorDamageMultiplier;
				OnCurrentArmorChanged.Broadcast(Armor);
				OnArmorBarUpdated.Broadcast(GetArmorBarValue());
				NewHealth = Health;
				return;
		}
		float l_ValueToReduceAfterArmor = ValueToReduce * ArmorDamageMultiplier - Armor;
		ValueToReduce = l_ValueToReduceAfterArmor / ArmorDamageMultiplier;
		//at this point armor is zero and we get new ValueToReduce from Health
		Armor = 0;
		OnCurrentArmorChanged.Broadcast(Armor);
		OnArmorBarUpdated.Broadcast(GetArmorBarValue());

	}
	Health = Health - ValueToReduce;
	if (Health <= 0)
	{
		bIsDead = true;
	}
	else
	{
		bIsDead = false;
	}
	NewHealth = Health;
	OnCurrentHealthChanged.Broadcast(GetIsDead(), Health);
	OnHealthBarUpdated.Broadcast(GetIsDead(), GetHealthBarValue());
}

void UAAA_HealthSystemComponent::ReduceMaxHealth( float ValueToReduce, float& NewMaxHealth)
{
	MaxHealth = MaxHealth - ValueToReduce;
	NewMaxHealth = MaxHealth;
	OnMaxHealthChanged.Broadcast(MaxHealth);
	OnMaxHealthBarUpdated.Broadcast(GetIsDead(), GetMaxHealthBarValue());
}

void UAAA_HealthSystemComponent::RegenerateHealth(float InTime, float HealthToRegin)
{
	GetWorld()->GetTimerManager().SetTimer(AAATimerHandle, [this, HealthToRegin]()
		{
			if (Health <= 0)
			{
				// Stop the timer if dead
				GetWorld()->GetTimerManager().ClearTimer(AAATimerHandle);
			}
			// Check if current Health is less than MaxHealth
			if (Health < MaxHealth)
			{
				// Increase health by HealthToRegin but not exceeding MaxHealth
				Health = FMath::Min(Health + HealthToRegin, MaxHealth);
				OnCurrentHealthChanged.Broadcast(GetIsDead(), Health);
				OnHealthBarUpdated.Broadcast(GetIsDead(), GetHealthBarValue());
				// Log current health for debugging
				UE_LOG(LogTemp, Warning, TEXT("Current Health: %f, MaxHealth: %f"), Health, MaxHealth);
			}
			else
			{
				if (bHasArmor && bRegenerateArmor)
				{
					if (Armor < MaxArmor)
					{
						// Increase Armor by HealthToRegin but not exceeding MaxArmor
						Armor = FMath::Min(Armor + HealthToRegin, MaxArmor);
						OnCurrentArmorChanged.Broadcast(Armor);
						OnArmorBarUpdated.Broadcast(GetArmorBarValue());
						// Log current Armor for debugging
						UE_LOG(LogTemp, Warning, TEXT("Current Armor: %f, MaxHealth: %f"), Armor, MaxArmor);
					}
				}
				if (Armor >= MaxArmor)
				{
					// Stop the timer when Health reaches or exceeds MaxHealth
					GetWorld()->GetTimerManager().ClearTimer(AAATimerHandle);

					// Log when the timer stops
					UE_LOG(LogTemp, Warning, TEXT("Health reached MaxHealth, stopping timer."));
				}
				
			}
		}, InTime, true);  // 'true' for looping timer
}



float UAAA_HealthSystemComponent::GetHealthBarValue()
{
		return Health / HealthBarPercentage;
}

float UAAA_HealthSystemComponent::GetArmorBarValue()
{
	return Armor/ArmorBarPercentage;
}

float UAAA_HealthSystemComponent::GetMaxHealthBarValue()
{
	return MaxHealth / HealthBarPercentage;
}

float UAAA_HealthSystemComponent::GetMaxArmorBarValue()
{
	return MaxArmor/ArmorBarPercentage;
}

bool UAAA_HealthSystemComponent::GetIsDead()
{
	if (Health<=0)
	{
		return true;
	}
	return false;
}







