// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "Projectile.h"
//#include "SoundCue.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UProjectileMovementComponent* projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("projectile"));
	projectile->InitialSpeed = 100;
	projectile->ProjectileGravityScale = 0;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("root"));
	CollisionBox->SetRelativeScale3D(FVector(0.7, 1, 0.7));
	CollisionBox->SetBoxExtent(FVector(50, 10, 10));
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);
	CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	RootComponent = CollisionBox;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_Pipe(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Pipe.Shape_Pipe'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Tech_Material(TEXT("Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse.M_Tech_Hex_Tile_Pulse'"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh"));
	Mesh->SetRelativeLocation(FVector(0, 0, -15));
	Mesh->SetRelativeRotation(FRotator(0, -90, 0));
	Mesh->SetStaticMesh(StaticMesh_Pipe.Object);
	Mesh->SetMaterial(0, Tech_Material.Object);
	Mesh->AttachParent = RootComponent;
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	static ConstructorHelpers::FObjectFinder<USoundCue> Explosion(TEXT("SoundCue'/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue'"));
	CollisionSound = Explosion.Object;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionEffect(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	CollisionEffect = ExplosionEffect.Object;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AProjectile::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	UE_LOG(LogTemp, Warning, TEXT("Projectile hit"));
	
	//PlaySoundOnActor(CollisionSound);
	UGameplayStatics::PlaySoundAttached(CollisionSound, this->GetRootComponent());
	//UGameplayStatics::SpawnEmitterAttached(CollisionEffect, this->GetRootComponent());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollisionEffect, this->GetActorLocation());
	Destroy();
}