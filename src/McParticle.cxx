//                                                                       
// The McParticle class provides an interface to a particle for use in
// GLAST Monte Carlo applications
//
// $Header$

#include "mcRootData/McParticle.h"
#include <iostream>

ClassImp(McParticle)

McParticle::McParticle() : 
m_particleId(0), m_statusFlags(0)
{
    Clear();
}

McParticle::McParticle(const McParticle &p) {
    // Purpose and Method:  The copy constructor.  This will make it easier to
    //    read in the McParticles within the simulation.

    m_particleId = p.m_particleId;
    m_statusFlags = p.m_statusFlags;
    m_initialPosition = TVector3(p.m_initialPosition.X(), p.m_initialPosition.Y(),
        p.m_initialPosition.Z());
    m_finalPosition = TVector3(p.m_finalPosition.X(), p.m_finalPosition.Y(),
        p.m_finalPosition.Z());
    m_initialFourMomentum = TLorentzVector(p.m_initialFourMomentum.X(),
        p.m_initialFourMomentum.Y(), p.m_initialFourMomentum.Z(), 
        p.m_initialFourMomentum.T());
    m_finalFourMomentum = TLorentzVector(p.m_finalFourMomentum.X(),
        p.m_finalFourMomentum.Y(), p.m_finalFourMomentum.Z(),
        p.m_finalFourMomentum.T());
    m_mother = p.m_mother;
    // TRefArrayIter::Next does not seem to be implemented in Root 3.02.03
    // or ROOT 3.03.04 - just iterating over the entries for now.
    //TRefArrayIter daughterIter(&p.m_daughters);
    McParticle *curRef;
    UInt_t iPart;
    for (iPart = 0; iPart < p.m_daughters.GetEntries(); iPart++) {
    //while ( (curRef = (McParticle*)daughterIter.Next()) ){
        curRef = (McParticle*)p.m_daughters.At(iPart);
        m_daughters.Add(curRef);
    }
}

McParticle::~McParticle() {
    Clear();
}

void McParticle::Clear(Option_t *option) {
    m_particleId = 0;
    m_statusFlags = 0;
    m_initialPosition = TVector3(0., 0., 0.);
    m_finalPosition = TVector3(0., 0., 0.);
    m_initialFourMomentum = TLorentzVector(0., 0., 0., 0.);
    m_finalFourMomentum = TLorentzVector(0., 0., 0., 0.);
    m_daughters.Clear();
}

void McParticle::Print(Option_t *option) const {
    using namespace std;
    TObject::Print(option);
    cout.precision(2);
    cout << "ParticleId: " << m_particleId;
    cout << " StatusFlag: " << m_statusFlags << endl;
    cout << "InitialPos: (" << m_initialPosition.X() << "," << m_initialPosition.Y()
        << "," << m_initialPosition.Z() << ") ";
    cout << "FinalPos: (" << m_finalPosition.X() << "," << m_finalPosition.Y()
        << "," << m_finalPosition.Z() << ")" << endl;
    cout << "InitMom: (" << m_initialFourMomentum.X() << "," << m_initialFourMomentum.Y() << ","
        << m_initialFourMomentum.Z() << "," << m_initialFourMomentum.T() << ")    ";
    cout << "FinalMom: (" << m_finalFourMomentum.X() << "," << m_finalFourMomentum.Y() << ","
        << m_finalFourMomentum.Z() << "," << m_finalFourMomentum.T() << ")" << endl;
    cout << "Mother:  ";
    m_mother.Print();
    cout << "Number of Daughters: " << m_daughters.GetEntries() << endl;
}


void McParticle::initialize( McParticle* mother, Int_t id, UInt_t statusBits,
                      const TLorentzVector& initMom,
                      const TLorentzVector& finalMom,
                      const TVector3& initPos,
                      const TVector3& finalPos)
{
    m_mother = mother;
    m_particleId = id;
    m_statusFlags = statusBits;
    m_initialFourMomentum.SetXYZT(initMom.X(), initMom.Y(), initMom.Z(), initMom.T());
    m_finalFourMomentum.SetXYZT(finalMom.X(), finalMom.Y(), finalMom.Z(), finalMom.T());
    m_initialPosition.SetXYZ(initPos.X(), initPos.Y(), initPos.Z());
    m_finalPosition.SetXYZ(finalPos.X(), finalPos.Y(), finalPos.Z());
    if ( mother == 0 ) return;
    if( mother != this) mother->m_daughters.Add(this);
}


const McParticle* McParticle::getMother() {

    return ( (McParticle*)(m_mother.GetObject()) );
}

const McParticle* McParticle::getDaughter(Int_t index) const {
    return ( (McParticle*)m_daughters.At(index) );
}

Int_t McParticle::getParticleProperty() const {
    return m_statusFlags;
}

Bool_t McParticle::primaryParticle() const
{
  return (m_statusFlags & PRIMARY)==PRIMARY;
}


const TVector3& McParticle::getInitialPosition()const
{
    return m_initialPosition;
}


const TVector3& McParticle::getFinalPosition() const {
    return m_finalPosition;
}

    
const TLorentzVector& McParticle::getInitialFourMomentum() const { 
    return m_initialFourMomentum; 
}

const TLorentzVector& McParticle::getFinalFourMomentum() const {
    return m_finalFourMomentum;
};
