// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "A2_Goals/A2_GoalsGameModeBase.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeA2_GoalsGameModeBase() {}
// Cross Module References
	A2_GOALS_API UClass* Z_Construct_UClass_AA2_GoalsGameModeBase_NoRegister();
	A2_GOALS_API UClass* Z_Construct_UClass_AA2_GoalsGameModeBase();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_A2_Goals();
// End Cross Module References
	void AA2_GoalsGameModeBase::StaticRegisterNativesAA2_GoalsGameModeBase()
	{
	}
	UClass* Z_Construct_UClass_AA2_GoalsGameModeBase_NoRegister()
	{
		return AA2_GoalsGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_AA2_GoalsGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AA2_GoalsGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_A2_Goals,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AA2_GoalsGameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "A2_GoalsGameModeBase.h" },
		{ "ModuleRelativePath", "A2_GoalsGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AA2_GoalsGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AA2_GoalsGameModeBase>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AA2_GoalsGameModeBase_Statics::ClassParams = {
		&AA2_GoalsGameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(Z_Construct_UClass_AA2_GoalsGameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AA2_GoalsGameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AA2_GoalsGameModeBase()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AA2_GoalsGameModeBase_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AA2_GoalsGameModeBase, 1254004846);
	template<> A2_GOALS_API UClass* StaticClass<AA2_GoalsGameModeBase>()
	{
		return AA2_GoalsGameModeBase::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AA2_GoalsGameModeBase(Z_Construct_UClass_AA2_GoalsGameModeBase, &AA2_GoalsGameModeBase::StaticClass, TEXT("/Script/A2_Goals"), TEXT("AA2_GoalsGameModeBase"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AA2_GoalsGameModeBase);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
