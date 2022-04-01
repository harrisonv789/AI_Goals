// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef A2_GOALS_Ship_generated_h
#error "Ship.generated.h already included, missing '#pragma once' in Ship.h"
#endif
#define A2_GOALS_Ship_generated_h

#define A2_Goals_Source_A2_Goals_Ship_h_12_SPARSE_DATA
#define A2_Goals_Source_A2_Goals_Ship_h_12_RPC_WRAPPERS
#define A2_Goals_Source_A2_Goals_Ship_h_12_RPC_WRAPPERS_NO_PURE_DECLS
#define A2_Goals_Source_A2_Goals_Ship_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAShip(); \
	friend struct Z_Construct_UClass_AShip_Statics; \
public: \
	DECLARE_CLASS(AShip, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/A2_Goals"), NO_API) \
	DECLARE_SERIALIZER(AShip)


#define A2_Goals_Source_A2_Goals_Ship_h_12_INCLASS \
private: \
	static void StaticRegisterNativesAShip(); \
	friend struct Z_Construct_UClass_AShip_Statics; \
public: \
	DECLARE_CLASS(AShip, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/A2_Goals"), NO_API) \
	DECLARE_SERIALIZER(AShip)


#define A2_Goals_Source_A2_Goals_Ship_h_12_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AShip(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AShip) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AShip); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AShip); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AShip(AShip&&); \
	NO_API AShip(const AShip&); \
public:


#define A2_Goals_Source_A2_Goals_Ship_h_12_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AShip(AShip&&); \
	NO_API AShip(const AShip&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AShip); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AShip); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AShip)


#define A2_Goals_Source_A2_Goals_Ship_h_12_PRIVATE_PROPERTY_OFFSET
#define A2_Goals_Source_A2_Goals_Ship_h_9_PROLOG
#define A2_Goals_Source_A2_Goals_Ship_h_12_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	A2_Goals_Source_A2_Goals_Ship_h_12_PRIVATE_PROPERTY_OFFSET \
	A2_Goals_Source_A2_Goals_Ship_h_12_SPARSE_DATA \
	A2_Goals_Source_A2_Goals_Ship_h_12_RPC_WRAPPERS \
	A2_Goals_Source_A2_Goals_Ship_h_12_INCLASS \
	A2_Goals_Source_A2_Goals_Ship_h_12_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define A2_Goals_Source_A2_Goals_Ship_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	A2_Goals_Source_A2_Goals_Ship_h_12_PRIVATE_PROPERTY_OFFSET \
	A2_Goals_Source_A2_Goals_Ship_h_12_SPARSE_DATA \
	A2_Goals_Source_A2_Goals_Ship_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	A2_Goals_Source_A2_Goals_Ship_h_12_INCLASS_NO_PURE_DECLS \
	A2_Goals_Source_A2_Goals_Ship_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> A2_GOALS_API UClass* StaticClass<class AShip>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID A2_Goals_Source_A2_Goals_Ship_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
