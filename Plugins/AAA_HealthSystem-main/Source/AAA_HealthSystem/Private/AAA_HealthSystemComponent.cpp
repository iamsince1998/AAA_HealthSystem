// Copyright (c) 2025 iamsince1998. All Rights Reserved.


#include "AAA_HealthSystemComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UAAA_HealthSystemComponent::UAAA_HealthSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;
	SetIsReplicatedByDefault(true);
	bEditableWhenInherited = true;
	// ...
}


// Called when the game starts
void UAAA_HealthSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner() && !GetOwner()->GetIsReplicated() && GetWorld() && GetWorld()->GetNetMode() != NM_Standalone)
	{
		UE_LOG(LogTemp, Warning, TEXT("AAA_HealthSystemComponent owner '%s' is not replicated. Health replication requires owner actor replication."), *GetOwner()->GetName());
	}

	BroadcastHealthUpdated();
	BroadcastMaxHealthUpdated();
	BroadcastArmorUpdated();
	BroadcastMaxArmorUpdated();
}

void UAAA_HealthSystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Specify the variable to replicate
	DOREPLIFETIME(UAAA_HealthSystemComponent, Health);
	DOREPLIFETIME(UAAA_HealthSystemComponent, MaxHealth);
	DOREPLIFETIME(UAAA_HealthSystemComponent, HealthBarPercentage);
	DOREPLIFETIME(UAAA_HealthSystemComponent, bHasArmor);
	DOREPLIFETIME(UAAA_HealthSystemComponent, ArmorDamageMultiplier);
	DOREPLIFETIME(UAAA_HealthSystemComponent, bRegenerateArmor);
	DOREPLIFETIME(UAAA_HealthSystemComponent, Armor);
	DOREPLIFETIME(UAAA_HealthSystemComponent, MaxArmor);
	DOREPLIFETIME(UAAA_HealthSystemComponent, ArmorBarPercentage);
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
	if (GetOwner() && !GetOwner()->HasAuthority())
	{
		ServerSetCurrentHealth(SetHealth);
		return;
	}

	ApplySetCurrentHealth(SetHealth);
}

void UAAA_HealthSystemComponent::SetCurrentArmor(float SetArmor)
{
	if (GetOwner() && !GetOwner()->HasAuthority())
	{
		ServerSetCurrentArmor(SetArmor);
		return;
	}

	ApplySetCurrentArmor(SetArmor);
}

float UAAA_HealthSystemComponent::GetMaxHealth()
{
	return MaxHealth;
}

void UAAA_HealthSystemComponent::SetMaxHealth(float SetHealth)
{
	if (GetOwner() && !GetOwner()->HasAuthority())
	{
		ServerSetMaxHealth(SetHealth);
		return;
	}

	ApplySetMaxHealth(SetHealth);
}

void UAAA_HealthSystemComponent::AddHealth(bool bOverrideMaxHealth,float ValueToAdd, float& NewHealth)
{
	if (GetOwner() && !GetOwner()->HasAuthority())
	{
		ServerAddHealth(bOverrideMaxHealth, ValueToAdd);
		NewHealth = Health;
		return;
	}

	ApplyAddHealth(bOverrideMaxHealth, ValueToAdd);
	NewHealth = Health;
}

void UAAA_HealthSystemComponent::AddMaxHealth( float ValueToAdd, float& NewMaxHealth)
{
	if (GetOwner() && !GetOwner()->HasAuthority())
	{
		ServerAddMaxHealth(ValueToAdd);
		NewMaxHealth = MaxHealth;
		return;
	}

	ApplyAddMaxHealth(ValueToAdd);
	NewMaxHealth = MaxHealth;
}

void UAAA_HealthSystemComponent::ReduceHealth(bool& bIsDead, float ValueToReduce, float& NewHealth)
{
	if (GetOwner() && !GetOwner()->HasAuthority())
	{
		ServerReduceHealth(ValueToReduce);
		bIsDead = GetIsDead();
		NewHealth = Health;
		return;
	}

	ApplyReduceHealth(ValueToReduce, bIsDead);
	NewHealth = Health;
}

void UAAA_HealthSystemComponent::ReduceMaxHealth( float ValueToReduce, float& NewMaxHealth)
{
	if (GetOwner() && !GetOwner()->HasAuthority())
	{
		ServerReduceMaxHealth(ValueToReduce);
		NewMaxHealth = MaxHealth;
		return;
	}

	ApplyReduceMaxHealth(ValueToReduce);
	NewMaxHealth = MaxHealth;
}

void UAAA_HealthSystemComponent::RegenerateHealth(float InTime, float HealthToRegin)
{
	if (GetOwner() && !GetOwner()->HasAuthority())
	{
		ServerRegenerateHealth(InTime, HealthToRegin);
		return;
	}

	if (!GetWorld())
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(AAATimerHandle, [this, HealthToRegin]()
		{
			if (!GetOwner() || !GetOwner()->HasAuthority() || !GetWorld())
			{
				return;
			}

			if (Health <= 0)
			{
				// Stop the timer if dead
				GetWorld()->GetTimerManager().ClearTimer(AAATimerHandle);
				return;
			}
			// Check if current Health is less than MaxHealth
			if (Health < MaxHealth)
			{
				// Increase health by HealthToRegin but not exceeding MaxHealth
				Health = FMath::Min(Health + HealthToRegin, MaxHealth);
				BroadcastHealthUpdated();
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
						BroadcastArmorUpdated();
						// Log current Armor for debugging
						UE_LOG(LogTemp, Warning, TEXT("Current Armor: %f, MaxHealth: %f"), Armor, MaxArmor);
					}
				}
				if (!bHasArmor || !bRegenerateArmor || Armor >= MaxArmor)
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
	return HealthBarPercentage != 0.f ? Health / HealthBarPercentage : 0.f;
}

float UAAA_HealthSystemComponent::GetArmorBarValue()
{
	return ArmorBarPercentage != 0.f ? Armor / ArmorBarPercentage : 0.f;
}

float UAAA_HealthSystemComponent::GetMaxHealthBarValue()
{
	return HealthBarPercentage != 0.f ? MaxHealth / HealthBarPercentage : 0.f;
}

float UAAA_HealthSystemComponent::GetMaxArmorBarValue()
{
	return ArmorBarPercentage != 0.f ? MaxArmor / ArmorBarPercentage : 0.f;
}

bool UAAA_HealthSystemComponent::GetIsDead()
{
	if (Health<=0)
	{
		return true;
	}
	return false;
}

void UAAA_HealthSystemComponent::OnRep_Health()
{
	BroadcastHealthUpdated();
}

void UAAA_HealthSystemComponent::OnRep_MaxHealth()
{
	BroadcastMaxHealthUpdated();
}

void UAAA_HealthSystemComponent::OnRep_HealthBarPercentage()
{
	BroadcastHealthUpdated();
	BroadcastMaxHealthUpdated();
}

void UAAA_HealthSystemComponent::OnRep_bHasArmor()
{
	BroadcastArmorUpdated();
	BroadcastMaxArmorUpdated();
}

void UAAA_HealthSystemComponent::OnRep_ArmorDamageMultiplier()
{
}

void UAAA_HealthSystemComponent::OnRep_bRegenerateArmor()
{
	BroadcastArmorUpdated();
}

void UAAA_HealthSystemComponent::OnRep_Armor()
{
	BroadcastArmorUpdated();
}

void UAAA_HealthSystemComponent::OnRep_MaxArmor()
{
	BroadcastMaxArmorUpdated();
}

void UAAA_HealthSystemComponent::OnRep_ArmorBarPercentage()
{
	BroadcastArmorUpdated();
	BroadcastMaxArmorUpdated();
}

void UAAA_HealthSystemComponent::ServerSetCurrentHealth_Implementation(float SetHealth)
{
	ApplySetCurrentHealth(SetHealth);
}

void UAAA_HealthSystemComponent::ServerSetCurrentArmor_Implementation(float SetArmor)
{
	ApplySetCurrentArmor(SetArmor);
}

void UAAA_HealthSystemComponent::ServerSetMaxHealth_Implementation(float SetHealth)
{
	ApplySetMaxHealth(SetHealth);
}

void UAAA_HealthSystemComponent::ServerAddHealth_Implementation(bool bOverrideMaxHealth, float ValueToAdd)
{
	ApplyAddHealth(bOverrideMaxHealth, ValueToAdd);
}

void UAAA_HealthSystemComponent::ServerAddMaxHealth_Implementation(float ValueToAdd)
{
	ApplyAddMaxHealth(ValueToAdd);
}

void UAAA_HealthSystemComponent::ServerReduceHealth_Implementation(float ValueToReduce)
{
	bool bIsDead = false;
	ApplyReduceHealth(ValueToReduce, bIsDead);
}

void UAAA_HealthSystemComponent::ServerReduceMaxHealth_Implementation(float ValueToReduce)
{
	ApplyReduceMaxHealth(ValueToReduce);
}

void UAAA_HealthSystemComponent::ServerRegenerateHealth_Implementation(float InTime, float HealthToRegin)
{
	RegenerateHealth(InTime, HealthToRegin);
}

void UAAA_HealthSystemComponent::ApplySetCurrentHealth(float SetHealth)
{
	Health = SetHealth;
	BroadcastHealthUpdated();
}

void UAAA_HealthSystemComponent::ApplySetCurrentArmor(float SetArmor)
{
	Armor = SetArmor;
	BroadcastArmorUpdated();
}

void UAAA_HealthSystemComponent::ApplySetMaxHealth(float SetHealth)
{
	MaxHealth = SetHealth;
	BroadcastMaxHealthUpdated();

	if (Health > MaxHealth)
	{
		Health = MaxHealth;
		BroadcastHealthUpdated();
	}
}

void UAAA_HealthSystemComponent::ApplyAddHealth(bool bOverrideMaxHealth, float ValueToAdd)
{
	if (bOverrideMaxHealth)
	{
		Health = Health + ValueToAdd;
	}
	else if (Health < MaxHealth)
	{
		Health = FMath::Min(Health + ValueToAdd, MaxHealth);
	}

	BroadcastHealthUpdated();
}

void UAAA_HealthSystemComponent::ApplyAddMaxHealth(float ValueToAdd)
{
	MaxHealth = MaxHealth + ValueToAdd;
	BroadcastMaxHealthUpdated();
}

void UAAA_HealthSystemComponent::ApplyReduceHealth(float ValueToReduce, bool& bIsDead)
{
	if (bHasArmor)
	{
		const float EffectiveArmorDamageMultiplier = FMath::Max(ArmorDamageMultiplier, KINDA_SMALL_NUMBER);
		const float ArmorReduction = ValueToReduce * EffectiveArmorDamageMultiplier;

		if (Armor > 0 && Armor >= ArmorReduction)
		{
			Armor -= ArmorReduction;
			BroadcastArmorUpdated();
			bIsDead = GetIsDead();
			return;
		}

		const float ValueToReduceAfterArmor = ArmorReduction - Armor;
		ValueToReduce = ValueToReduceAfterArmor / EffectiveArmorDamageMultiplier;
		Armor = 0;
		BroadcastArmorUpdated();
	}

	Health = Health - ValueToReduce;
	bIsDead = (Health <= 0);
	BroadcastHealthUpdated();
}

void UAAA_HealthSystemComponent::ApplyReduceMaxHealth(float ValueToReduce)
{
	MaxHealth = MaxHealth - ValueToReduce;
	BroadcastMaxHealthUpdated();

	if (Health > MaxHealth)
	{
		Health = MaxHealth;
		BroadcastHealthUpdated();
	}
}

void UAAA_HealthSystemComponent::BroadcastHealthUpdated()
{
	OnCurrentHealthChanged.Broadcast(GetIsDead(), Health);
	OnHealthBarUpdated.Broadcast(GetIsDead(), GetHealthBarValue());
}

void UAAA_HealthSystemComponent::BroadcastArmorUpdated()
{
	OnCurrentArmorChanged.Broadcast(Armor);
	OnArmorBarUpdated.Broadcast(GetArmorBarValue());
}

void UAAA_HealthSystemComponent::BroadcastMaxHealthUpdated()
{
	OnMaxHealthChanged.Broadcast(MaxHealth);
	OnMaxHealthBarUpdated.Broadcast(GetIsDead(), GetMaxHealthBarValue());
}

void UAAA_HealthSystemComponent::BroadcastMaxArmorUpdated()
{
	OnMaxArmorChanged.Broadcast(MaxArmor);
	OnMaxArmorBarUpdated.Broadcast(GetIsDead(), GetMaxArmorBarValue());
}



