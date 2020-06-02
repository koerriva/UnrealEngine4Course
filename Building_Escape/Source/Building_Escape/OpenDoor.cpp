// CopyRight Koerriva 2020


#include "OpenDoor.h"

#include "Components/AudioComponent.h"

#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	FindPressurePlate();
	FindAudioComponent();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(GetTotalMassOfActors()>MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorLastOpen = GetWorld()->GetTimeSeconds();
	}else
	{
		if(DoorLastOpen+DoorCloseDelay<=GetWorld()->GetTimeSeconds())
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw,OpenAngle,DeltaTime,DoorOpenSpeed);
	// UE_LOG(LogTemp,Warning,TEXT("CurrentYaw %f"),CurrentYaw);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
	if(AudioComponent&&!DoorOpened)
	{
		AudioComponent->Play();
	}
	DoorOpened = true;
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw,InitialYaw,DeltaTime,DoorCloseSpeed);
	// UE_LOG(LogTemp,Warning,TEXT("CurrentYaw %f"),CurrentYaw);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
	if(AudioComponent&&DoorOpened)
	{
		AudioComponent->Play();
	}
	DoorOpened = false;
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if(!AudioComponent)
	{
		UE_LOG(LogTemp,Error,TEXT("Not Find UAudioComponent on %s"),*GetOwner()->GetName());
	}
}

void UOpenDoor::FindPressurePlate()
{
	if(!PressurePlate)
	{
		UE_LOG(LogTemp,Error,TEXT("%s PressurePlate Required!"),*GetOwner()->GetName())
    }
}

float UOpenDoor::GetTotalMassOfActors() const
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	if(PressurePlate)
	{
		PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	}
	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

