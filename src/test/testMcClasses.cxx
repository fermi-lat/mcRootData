#include "Riostream.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TRandom.h"
#include "mcRootData/McEvent.h"

/** @file testMcClasses.cxx
* @brief This defines a test routine for the Monte Carlo ROOT classes.
*
* This program create a new Monte Carlo ROOT file, and the opens it up again
* for reading.  The contents are checked against the values known to be stored
* during the original writing of the file.
* The contents of the file are printed to the screen.
* The program returns 0 if the test passed.
* If failure, the program returns -1.
*
* $Header$
*/
const UInt_t runNum = 1;
const UInt_t numParticles = 5;
const Int_t sourceId = 7;
const UInt_t sequence = 4;
Float_t randNum;

bool floatInRange(Double_t actual, Double_t desired) {
  const Double_t fudge=0.00001;
  if ( (actual >= (desired - fudge)) && (actual <= (desired+fudge)) ){
    return true;
  }
  return false;

}

int checkMcEvent(const McEvent* mcEvt, UInt_t ievent) {
    // Purpose and Method: Check the contents of the McEvent read in from the 
    //    ROOT file.

    if (mcEvt->getEventId() != ievent) {
        std::cout << "EventId does not match!!" << std::endl;
        return -1;
    }
    if (mcEvt->getRunId() != runNum) {
        std::cout << "Run Number does not match!!" << std::endl;
        return -1;
    }
	if (mcEvt->getSourceId() != sourceId) {
		std::cout << "Source Id does not match!!" << std::endl;
		return -1;
	}
	if (mcEvt->getSequence() != sequence) {
		std::cout << "Sequence does not match!!" << std::endl;
		return -1;
	}
    
    return 0;
}

int checkMcParticle(McParticle* mcPart, Int_t ipart, UInt_t ievent) {
    // Purpose and Method: Check the contents of the McParticle read in from 
    //   the ROOT file
    McParticle particleRef ;
    particleRef.Fake(ievent,ipart,randNum) ;
    if (mcPart->CompareInRange(particleRef))
      return 0 ;
    else
      return -1 ;

}

int checkMcPositionHit(const McPositionHit* mcPosHit, Int_t ipart, UInt_t ievent) {
    // Purpose and Method: Check the contents of the McPositionHit read in from 
    //   the ROOT file
    McPositionHit hitRef ;
    hitRef.Fake(ievent,ipart,randNum) ;
    if (mcPosHit->CompareInRange(hitRef))
      return 0 ;
    else
      return -1 ;
    
}

int checkMcIntegratingHit(McIntegratingHit* mcIntHit, UInt_t, UInt_t) {
    // Purpose and Method: Check the contents of the McIntegratingHit read in 
    //    from the ROOT file
    VolumeIdentifier id = mcIntHit->getVolumeId();
    std::cout << "McIntHit volumeId: " << id.name() << std::endl;
    if ( (id.size() != 1) || (id.getBits0to31() != 0) || (id.getBits32to63() != 0) ) {
        std::cout << "McIntHit VolId is incorrect" << std::endl;
        return -1;
    }

    mcIntHit->itemizedEnergyReset();
    const McParticle *myPart;
    double energy;
    if (mcIntHit->itemizedEnergySize() != 1) {
        std::cout << "McIntHit map size: " << mcIntHit->itemizedEnergySize() << std::endl;
        return -1;
    }
    int count = 0;
    while ( (myPart = mcIntHit->itemizedEnergyNext(energy)) ) {
        ++count;
        if (!floatInRange(energy, 1.5)) {
            std::cout << "McInt map energy is incorrect: " << energy << std::endl;
            return -1;
        }
    }
    
    energy = mcIntHit->getMcParticleEnergy(McIntegratingHit::PRIMARY);
    if (!floatInRange(energy, 2.5)) {
        std::cout << "Id energy map is incorrect: " << energy << std::endl;
        return -1;
    }

    energy = mcIntHit->getMcParticleEnergy(McIntegratingHit::ELECTRON);
    if ( !floatInRange(energy, 3.0) ) {
        std::cout << "Id energy map for e- is wrong: " << energy << std::endl;
        return -1;
    }

    energy = mcIntHit->getMcParticleEnergy(McIntegratingHit::POSITRON);
    if ( !floatInRange(energy, 0.) ) {
        std::cout << "Id energy map for e+ is wrong: " << energy << std::endl;
        return -1;
    }

    double totE = mcIntHit->getTotalEnergy();
    if ( !floatInRange(totE, 5.5)) {
        std::cout << "Total E is incorrect: " << totE << std::endl;
        return -1;
    }

    TVector3 moment1 = mcIntHit->getMoment1();

    TVector3 moment2 = mcIntHit->getMoment2();

    if (count == 0) {
        std::cout << "No Error cannot read map which stores TRefs using" 
            << " compiled code and ROOT 3.02.07" << std::endl;
    }
    
    return 0;
}


int read(char* fileName, unsigned int numEvents) {
    // Purpose and Method: Read in the ROOT file just generated via the 
    //   write method

    TFile *f = new TFile(fileName, "READ");
    TTree *t = (TTree*)f->Get("Mc");
    McEvent *evt = 0;
    t->SetBranchAddress("McEvent", &evt);
    
    std::cout << "Opened the ROOT file for reading" << std::endl;
    
    UInt_t ievent, ipart;
    for (ievent = 0; ievent < numEvents; ievent++) {
        t->GetEvent(ievent);
        std::cout << "McEvent ievent = " << ievent << std::endl;
        evt->Print();
        if (checkMcEvent(evt, ievent) < 0) return -1;
        UInt_t numPart = evt->getMcParticleCol()->GetEntries();
        for (ipart = 0; ipart < numPart; ipart++) {
            evt->getMcParticle(ipart)->Print();
            if (checkMcParticle(evt->getMcParticle(ipart), ipart, ievent) < 0) return -1;
        }
        numPart = evt->getMcPositionHitCol()->GetEntries();
        for (ipart = 0; ipart < numPart; ipart++) {
            evt->getMcPositionHit(ipart)->Print();
            if (checkMcPositionHit(evt->getMcPositionHit(ipart), ipart, ievent) < 0) return -1;
        }
        numPart = evt->getMcIntegratingHitCol()->GetEntries();
        for (ipart = 0; ipart < numPart; ipart++){
            evt->getMcIntegratingHit(ipart)->Print();
            if (checkMcIntegratingHit(evt->getMcIntegratingHit(ipart), ipart, ievent) < 0) return -1;
        }
    }
    
    f->Close();
    delete f;
    
    return(0);
}

int write(char* fileName, UInt_t numEvents) {
    // Purpose and Method: Create a new Monte Carlo ROOT file

    Int_t buffer = 64000;
    Int_t splitLevel = 1;
    
    TFile *f =  new TFile(fileName, "RECREATE");
    TTree *t = new TTree("Mc", "Mc");
    McEvent *ev = new McEvent();
    t->Branch("McEvent", "McEvent", &ev, buffer, splitLevel);
    
    std::cout << "Created new ROOT file" << std::endl;
    
    TRandom randGen;
    VolumeIdentifier id;
    UInt_t ievent, ipart;
    randNum = randGen.Rndm();
    for (ievent = 0; ievent < numEvents; ievent++) {
        
        ev->initialize(ievent, runNum, sourceId, sequence, ievent*1.0);
        
        for (ipart = 0; ipart < numParticles; ipart ++) {
            
            // particle
            McParticle *mcPart = new McParticle();
            mcPart->Fake(ievent,ipart,randNum) ;
            ev->addMcParticle(mcPart);
            
            // position hit
            McPositionHit *posHit = new McPositionHit();
            posHit->Fake(ievent,ipart,randNum) ;
            ev->addMcPositionHit(posHit);
            
            McIntegratingHit *intHit = new McIntegratingHit();
            id.Clear();
            id.append(0);
            intHit->initialize(id);
            TVector3 pos = mcPart->getFinalPosition();
            TVector3 pos2(1.3, 0.0, 12.0);
            intHit->addEnergyItem(1.5, mcPart, pos);
            double totE = 5.5;
            double energyArr[3] = { 2.5, 3.0, 0.0 };
            TVector3 moment1(1.0, 2.0, 3.0);
            TVector3 moment2(2.0, 4.0, 6.0);
            intHit->setEnergyItems(totE, energyArr, moment1, moment2);
            ev->addMcIntegratingHit(intHit);
        }
        t->Fill();
        ev->Clear();
    }
    
    std::cout << "Filled ROOT file with " << numEvents << " events" << std::endl;
    delete ev;
    
    f->Write();
    f->Close();
    delete f;
    return(0);
}


int main(int argc, char **argv) {
    // Purpose and Method: Main program
    // Output:  Return 0 for success. Returns -1 for failure.

    char *fileName = "mc.root";
    int n = 1;
    unsigned int numEvents = 10;
    if (argc > 1) {
        fileName = argv[n++];
    } 
    if (argc > 2) {
        numEvents = atoi(argv[n++]);
    } 
    
    int sc = 0;
    sc = write(fileName, numEvents);
    sc = read(fileName, numEvents);
    
    if (sc == 0) {
      std::cout << "MC ROOT file writing and reading suceeded!" << std::endl;
    } else {
      std::cout << "FAILED" << std::endl;
    }
    
    return(sc);
}


