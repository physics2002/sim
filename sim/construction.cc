#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    G4NistManager *nist = G4NistManager::Instance();

    G4Material *NaI = new G4Material("NaI", 3.67*g/cm3, 2);
    NaI->AddElement(nist->FindOrBuildElement("Na"), 1);
    NaI->AddElement(nist->FindOrBuildElement("I"), 1);


    G4Material *Scintillator = new G4Material("Scintillator", 3.67*g/cm3,1);
    Scintillator-> AddMaterial(NaI, 100.0*perCent);

    G4double energy[2] = {1.239841939*eV/0.2, 1.239841939*eV/0.9};
    G4double rindexScintillator[2] = {1.8, 1.8};
    G4double rindexWorld[2] = {1.1, 1.1};

    G4MaterialPropertiesTable *nptScintillator = new G4MaterialPropertiesTable();
    nptScintillator->AddProperty("RINDEX", energy, rindexScintillator, 2);

    Scintillator->SetMaterialPropertiesTable(nptScintillator);

    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4MaterialPropertiesTable *nptWorld = new G4MaterialPropertiesTable();
    nptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);
    worldMat->SetMaterialPropertiesTable(nptWorld);
    
    G4Box *solidWorld = new G4Box("solidWorld", 0.5*m, 0.5*m, 0.5*m);

    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

    G4Tubs *solidRadiator = new G4Tubs("solidRadiator", 0, 0.0125*m, 0.5*m, 0, 2*M_PI);

    G4LogicalVolume *logicRadiator = new G4LogicalVolume(solidRadiator,Scintillator,"logicalRadiator");

    G4VPhysicalVolume *physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicRadiator, "physRadiator", logicWorld, false, 0, true);

    G4Box *solidDetector = new G4Box("solidDetector", 0.005*m, 0.005*m, 0.01*m);
    logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicalDetector");

    for(G4int i = 0; i<100; i++)
    {
        for(G4int j = 0; j<100; j++)
        {
            G4VPhysicalVolume *physDetector = new G4PVPlacement(0,G4ThreeVector(-0.5*m+(j+0.5)*m/100, -0.49*m, -0.5*m+(i+0.5)*m/100), logicDetector, "physDetector", logicWorld, false, j+i*100, true);         
        }
    }

    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");

    logicDetector->SetSensitiveDetector(sensDet);
}
