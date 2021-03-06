#ifndef MCEVENT_H
#define MCEVENT_H

#include "TObject.h"
#include "TObjArray.h"
#include "TClonesArray.h"
#include "McParticle.h"
#include "McPositionHit.h"
#include "McIntegratingHit.h"
#include "McTkrStrip.h"
#include "McTrajectory.h"
#include "TString.h"

/** @class McEvent
* @brief GLAST Monte Carlo Event class.
*
* The McEvent class contains all the information about a single MC simulated 
* event.  This class contains:
* - Run id
* - Event id
* - Collection of McParticle
* - Collection of McPositionHit
* - Collection of McIntegratingHit
* - Collection of McTrajectories
* - Source Name
* The MC data stored to ROOT files mirrors the data available within the Gaudi 
* TDS.
*  
*  $Header$
*/


/** @page mcRootData_doc
    @section McEvent McEvent Documentation
    \image html mcTree.gif
    Notes 

<table>
<tr><th><EM> Method <th> Return Type </EM><th> Description <th>

<small>
<tr><td> getEventId
<td>UInt_t<td> Returns the event identifier
<tr><td> getRunId
<td>UInt_t<td> Returns the run identifier
<tr><td> getTimeStamp
<td>Double_t<td> Returns the time stamp
<tr><td> getSourceId
<td>I<td> Returns the Source Id
<tr><td> getSequence
<td>UInt_t<td> Returns the sequence
<tr><td> getSourceName
<td>char*<td> Returns the actual name of the source for generation
<tr><td> getSourceNameAsTString
<td>TString&<td> Returns source name in a TString&

<tr><td> getMcParticleCount
<td>UInt_t<td> Reports the number of McParticles stored in the collection
<tr><td> getMcParticle(UInt_t index)
<td>McParticle*<td> Retrieves an McParticle pointer from the collection, using the index
<tr><td> getMcParticleCol
<td>TObjArray<td> Returns the full TObjArray containing all McParticles
<tr><td> getMcPositionHit(UInt_t index)
<td>McPositionHit*<td> Returns a McPositionHit pointer corresponding to the index

<tr><td> getMcPositionHintCount
<td>UInt_t<td> Returns the number of McPositionHits stored in the collection
<tr><td> getMcPositionHitCol
<td>TObjArray*<td> Returns the full TObjArray containing all McPositionHits
<tr><td> getMcTkrStrip(UInt_t index)
<td>McTkrStrip*<td> Returns a McTkrStrip pointer corresponding to the index
<tr><td> getMcTkrStripCount
<td>UInt_t<td> Returns the number of McTkrStrips in the collection
<tr><td> getMcTkrStripCol
<td>TObjArray*<td> Returns the full TObjArray collection of McTkrStrips

<tr><td> getMcIntegratingHit(UInt_t index)
<td>McIntegratingHit*<td> Returns the McIntegratingHit pointer corresponding to the index
<tr><td>getIntegratingHitCount
<td>UInt_t<td> Returns the number of McIntegratingHits in the collection
<tr><td> getMcIntegrationHitCol
<td>TObjArray*<td> Returns the TObjArray* collection containing McIntegratingHits

<tr><td> getMcTracjectory(UInt_t index)
<td>McTrajectory*<td> Returns the McTrajectory pointer corresponding to index
<tr><td> getMcTrajectoryCount
<td>UInt_t<td> Returns the number of McTrajectories in the collection
<tr><td> getMcTrajectoryCol
<td>TObjArray*<td> Returns the TObjArray* collection of McTrajectories

</small>
</table>
*/

class McEvent : public TObject  
{
public:
    
    McEvent();
    
    virtual ~McEvent();
    
    /// clear lists, free pointers, etc., after read from / write to file
    void Clear(Option_t *option ="");
    // DC: I CANNOT MAKE THE USUAL Fake() and CompareInRange(),
    // because McEvent is designed as a singleton.
    void Fake( Int_t ievent, Int_t irun, Float_t randNum ) ; // for tests
    Bool_t CompareToFake( Int_t ievent, Int_t irun, Float_t randNum ) ; // for tests
    void Print(Option_t *option="") const;
    
    void initialize(UInt_t nEvent, UInt_t nRun, Int_t sourceId,
                    UInt_t sequence, Double_t timeStamp, 
                    const TString& str);

    void initSourceName(const TString &str) { m_sourceName = str; }
    
    inline UInt_t getEventId() const { return m_eventId; };
    inline UInt_t getRunId() const { return m_runId; };
    inline Int_t getSourceId() const { return m_sourceId; };
    inline UInt_t getSequence() const { return m_sequence; };
    inline const char* getSourceName() const { return m_sourceName.Data(); }
    inline const TString& getSourceNameAsTString() const { return m_sourceName; }

    
    /// add a McParticle to list
    void addMcParticle(McParticle *pPart);
    /// retrieve number of McParticles stored in the collection
    inline UInt_t getMcParticleCount() const { 
        return ((m_particleCol) ? m_particleCol->GetEntries() : 0);
    };
    /// get particle from list at index
    McParticle* getMcParticle(UInt_t index) const;
    /// return the full TObjArray containing McParticles
    TObjArray* getMcParticleCol() const { return m_particleCol; };
    /// clear of the array (necessary for converters)
    void clearMcParticleCol()  { m_particleCol->Clear(); };
   
    /// store a new McPositionHit in the collection
    void addMcPositionHit(McPositionHit *hit);
    /// return a McPositionHit corresponding to index
    McPositionHit* getMcPositionHit(UInt_t index) const;
    /// return the number of McPositionHits stored in the collection
    inline UInt_t getMcPositionHitCount() const { 
        return ((m_positionHitCol) ? m_positionHitCol->GetEntries() : 0); 
    };
    /// return the full TObjArray containing McPositionHits
    const TObjArray* getMcPositionHitCol() const { return m_positionHitCol; };
    /// clear of the array (necessary for converters)
    void clearMcPositionHitCol()  { m_positionHitCol->Clear(); };
   
    /// store a new McTkrStrip in the collection
    void addMcTkrStrip(McTkrStrip *hit);
    /// return a McTkrStrip corresponding to index
    McTkrStrip* getMcTkrStrip(UInt_t index) const;
    /// return the number of McTkrStrips stored in the collection
    inline UInt_t getMcTkrStripCount() const { 
        return ((m_tkrStripCol) ? m_tkrStripCol->GetEntries() : 0); 
    };
    /// return the full TObjArray containing McTkrStrips
    const TObjArray* getMcTkrStripCol() const { return m_tkrStripCol; };
    /// clear of the array (necessary for converters)
    void clearMcTkrStripCol()  { m_tkrStripCol->Clear(); };
 
    /// add a new McIntegratingHit to the collection
    void addMcIntegratingHit(McIntegratingHit *hit);
    /// return a McIntegrating hit corresponding to the index 
    McIntegratingHit* getMcIntegratingHit(UInt_t index) const;
    /// return the number of McIntegratingHits stored in the collection
    inline UInt_t getIntegratingHitCount() const { 
        return ((m_integratingHitCol) ? m_integratingHitCol->GetEntries() : 0); 
    };
    /// return the full TObjArray containing McIntegratingHits
    const TObjArray* getMcIntegratingHitCol() const { return m_integratingHitCol; };
    /// clear of the array (necessary for converters)
    void clearMcIntegratingHitCol()  { m_integratingHitCol->Clear(); };
   
    /// store a new McTrajectory in the collection
    void addMcTrajectory(McTrajectory *trajectory);
    /// return a MMcTrajectory corresponding to index
    McTrajectory* getMcTrajectory(UInt_t index) const;
    /// return the number of McTrajectories stored in the collection
    inline UInt_t getMcTrajectoryCount() const { 
        return ((m_trajectoryCol) ? m_trajectoryCol->GetEntries() : 0); 
    };
    /// return the full TObjArray containing McPositionHits
    const TObjArray* getMcTrajectoryCol() const { return m_trajectoryCol; };
    /// clear of the array (necessary for converters)
    void clearMcTrajectoryCol()  { m_trajectoryCol->Clear(); };
 
/// time stamp stuff here
    inline Double_t getTimeStamp() const { return m_timeStamp; };

private:
    /// unique event id for this run
    UInt_t m_eventId;
    
    /// Run number
    UInt_t m_runId;

    // identifier of the source
    Int_t m_sourceId;

    /// Sequence number
    UInt_t m_sequence;
    
    /// Time in seconds
    Double_t m_timeStamp; /// this is supposed to coresponed to the following in MCEvent 
    /// Time stamp: use special class to encapsulate type
    ///TimeStamp           m_time;

    /// Collection of McParticles
    /// Must be TObjArray due to TRefArray which grows dynamically
    TObjArray *m_particleCol; //->
    /// static array to allow one-time array creation
    static TObjArray *m_staticParticleCol; //!
    
    /// Collection of McIntegratingHits
    /// must be a TObjArray due to map in McIntegratingHit
    TObjArray *m_integratingHitCol; //->
    /// static array to allow one-time array creation
    static TObjArray *m_staticIntegratingHitCol; //!
    
    /// Collection of McPositionHits
    TObjArray *m_positionHitCol; //->
    /// static array to allow one-time array creation
    static TObjArray *m_staticPositionHitCol; //!
    

    
    /// Collection of McTrajectories
    TObjArray *m_trajectoryCol; //->
    /// static array to allow one-time array creation
    static TObjArray *m_staticTrajectoryCol; //!

    /// name for the source, hopefully unique
    TString m_sourceName;

    /// Collection of McTkrStrips
    TObjArray *m_tkrStripCol; //->
    /// static array to allow one-time array creation
    static TObjArray *m_staticTkrStripCol; //!
    
    ClassDef(McEvent,8) // Monte Carlo Event Class
};

#endif
