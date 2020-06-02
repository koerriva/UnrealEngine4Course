// CopyRight Koerriva 2020


#include "Grabber.h"

#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle==nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("Not Find UPhysicsHandleComponent on %s"),*GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent)
	{
		UE_LOG(LogTemp,Warning,TEXT("UInputComponent Found!"),*GetOwner()->GetName());
		InputComponent->BindAction("Grab",IE_Pressed,this,&UGrabber::Grab);
		InputComponent->BindAction("Grab",IE_Released,this,&UGrabber::GrabRelease);
	}
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
	}
}

void UGrabber::Grab()
{
	const FHitResult HitResult = LookAt();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	if(HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab,NAME_None,GetPlayerReach());
	}
}

void UGrabber::GrabRelease()
{
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::LookAt() const
{
	FHitResult HitResult;
	const FCollisionQueryParams QueryParams(TEXT("GrabberTrace"),false,GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult,GetPlayerWorldPosition(),GetPlayerReach()
        ,FCollisionObjectQueryParams(ECC_PhysicsBody)
        ,QueryParams);

	return HitResult;
}

FVector UGrabber::GetPlayerReach() const
{
	FVector PlayerViewPortLocation;
	FRotator PlayerViewPortRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPortLocation,OUT PlayerViewPortRotation);
	return PlayerViewPortLocation+PlayerViewPortRotation.Vector()*ViewLength;
}

FVector UGrabber::GetPlayerWorldPosition() const
{
	FVector PlayerViewPortLocation;
	FRotator PlayerViewPortRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPortLocation,OUT PlayerViewPortRotation);
	return PlayerViewPortLocation;
}

