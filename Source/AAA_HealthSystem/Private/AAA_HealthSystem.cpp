// Copyright Epic Games, Inc. All Rights Reserved.

#include "AAA_HealthSystem.h"

#define LOCTEXT_NAMESPACE "FAAA_HealthSystemModule"

void FAAA_HealthSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FAAA_HealthSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAAA_HealthSystemModule, AAA_HealthSystem)