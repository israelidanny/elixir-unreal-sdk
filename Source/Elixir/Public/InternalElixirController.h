#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Dom/JsonObject.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/SaveGame.h"
#include "InternalElixirController.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FCallback, bool, bSuccess);

USTRUCT(BlueprintType, Category = "Elixir")
struct FElixirUserData
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Basic)
	FString sub; // Elixir Id
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Basic)
	FString iss;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Basic)
	TArray<FString> wallets;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Basic)
	FString nickname;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Basic)
	FString picture;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Basic)
	FString aud;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Basic)
	FString status;
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FUserDataCallback, bool, bSuccess, FElixirUserData, userData);


USTRUCT(BlueprintType, Category = "Elixir")
struct FElixirNFTAttribute {
	GENERATED_USTRUCT_BODY();
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Basic)
	FString trait_type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Basic)
	FString value;
};

USTRUCT(BlueprintType, Category = "Elixir")
struct FElixirNFT {
	GENERATED_USTRUCT_BODY();
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Basic)
	FString tokenId;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Basic)
	FString name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Basic)
	FString image;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Basic)
	TArray<FElixirNFTAttribute> attributes;
};

USTRUCT(BlueprintType, Category = "Elixir")
struct FElixirCollection {
	GENERATED_USTRUCT_BODY();
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Basic)
	FString collection;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Basic)
	FString collectionName;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Basic)
	TArray<FElixirNFT> nfts;
};
DECLARE_DYNAMIC_DELEGATE_TwoParams(FCollectionsCallback, bool, bSuccess, const TArray<FElixirCollection> &, collections);

UCLASS()
class UElixirSaveData : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString RefreshToken;
};

class InternalElixirController
{
private:
	static InternalElixirController *_Instance;

	void RequestSession(UObject *WorldContextObject, FCallback OnComplete);
	void MakeRequest(FString uri, FString body, TFunction<void(TSharedPtr<FJsonObject> JsonObject)> OnSuccess, TFunction<void(int errorCode, FString message)> OnError);
	void SaveRefreshToken();
	void LoadRefreshToken();

	FString HashSignature(FString signature);

	FString BaseURL;
	FString REIKey;
	FString APIKey;
	FString Token;
	FString RefreshToken;

	FTimerDelegate SessionTimerCallback;
	FTimerHandle SessionTimerHandle;

public:
	static InternalElixirController *Instance() { if (!_Instance) _Instance = new InternalElixirController(); return _Instance; }
	void PrepareElixir(FString _APIKey);
	void InitElixir(UObject *WorldContextObject, FCallback OnComplete);
	void GetUserData(UObject *WorldContextObject, FUserDataCallback OnComplete);
	void GetCollections(UObject *WorldContextObject, FCollectionsCallback OnComplete);
	void CloseElixir(UObject *WorldContextObject, FCallback OnComplete);
	void Refresh(UObject *WorldContextObject, TFunction<void(bool result)> OnComplete);
	FString GetCurrentToken();
};
