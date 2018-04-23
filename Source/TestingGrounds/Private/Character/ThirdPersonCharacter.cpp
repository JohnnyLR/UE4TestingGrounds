// Copyright Jose Lloret 2018

#include "ThirdPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Gun.h"
#include "FirstPersonProjectile.h"
#include "ConstructorHelpers.h"
#include "Materials/MaterialInstance.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AThirdPersonCharacter::AThirdPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> Material(TEXT("/Game/Static/Character/Common/Materials/MI_UE4Man_Body_Red"));
	BrowserMaterial = Material.Object;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.0f);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
}

// Called when the game starts or when spawned
void AThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GunBlueprint != NULL)
	{
		Gun = GetWorld()->SpawnActor<AGun>(GunBlueprint);

		if (IsPlayerControlled())
		{
			Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
			Health = 400.0f;
		}
		else
		{
			Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
			GetCharacterMovement()->MaxWalkSpeed = 270.0f;

			if (BrowserMaterial != NULL && Health < 100.0f)
			{
				GetMesh()->SetMaterial(0, BrowserMaterial);
			}
		}

		Gun->AnimInstanceFP = Mesh1P->GetAnimInstance();
		Gun->AnimInstanceTP = GetMesh()->GetAnimInstance();

		if (InputComponent != NULL)
		{
			InputComponent->BindAction("Fire", IE_Pressed, this, &AThirdPersonCharacter::PullTrigger);
		}
	}

	StartingHealth = Health;
}

// Called every frame
void AThirdPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AThirdPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AThirdPersonCharacter::UnPossessed()
{
	Super::UnPossessed();

	if (Gun != NULL)
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
}

void AThirdPersonCharacter::PullTrigger()
{
	if (Gun != NULL)
	{
		Gun->OnFire();

		MakeNoise(1.0f, GetController()->GetPawn(), GetActorLocation(), 0.0f, NAME_None);
	}
}

float AThirdPersonCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		if (Dead(DamageAmount) && !IsPlayerControlled())
		{
			DetachFromControllerPendingDestroy();
			GetCapsuleComponent()->SetCollisionProfileName(FName("CharacterMesh"));
		}
	}
	return DamageAmount;
}

bool AThirdPersonCharacter::Dead(float DamageAmount)
{
	Health = Health - DamageAmount;

	if (Health <= 0.0f)
	{
		IsDead = true;
	}

	return IsDead;
}
