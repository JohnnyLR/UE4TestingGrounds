// Copyright Jose Lloret 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonCharacter.generated.h"

UCLASS()
class TESTINGGROUNDS_API AThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AThirdPersonCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void UnPossessed() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<class AGun> GunBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float Health = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float StartingHealth = 50;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Character)
	bool IsDead;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void PullTrigger();

private:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = Camera, meta = (AllowPrivateAccess = true))
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	AGun* Gun;

	bool Dead(float DamageAmount);

	class UMaterialInstance* BrowserMaterial;
};
