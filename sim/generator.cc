#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName="proton";
    G4ParticleDefinition *particle = particleTable->FindParticle("proton");

    G4ThreeVector pos(0.,0.7*m,0.);
    G4ThreeVector mom(0.,-1.,0.);

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleMomentum(900.*GeV);
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
