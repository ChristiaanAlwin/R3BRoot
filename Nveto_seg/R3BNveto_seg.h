#ifndef R3BNVETO_SEG_H
#define R3BNVETO_SEG_H

#include "R3BDetector.h"
#include "TLorentzVector.h"

#include <string>
#include <memory>

class FairVolume;
class TClonesArray;

/**
 * Nveto_seg detector simulation class
 * @author Jan Mayer
 * @since 12.01.2016
 * For each simulated event, TClonesArrays are filled:
 * - Nveto_segPoints (R3BNveto_segPoint), each representing energy deposition and light yield of a track in a paddle
 * - Nveto_segPrimaryNeutronInteractionPoints (FairMCPoint), each representing the first interaction points of
 *   primary neutrons.
 * Suitable geometry files require proper naming of the active volume (see CheckIfSensitive) and copy numbers.
 */

class R3BNveto_seg : public R3BDetector
{
  public:
    /** Default constructor */
    R3BNveto_seg();

    /** Standard constructor.
     *@param name    detector name
     *@param active  sensitivity flag
     */
    R3BNveto_seg(const char* name, Bool_t active);

    /** Default Destructor */
    // virtual ~R3BNveto_seg() = default;
    virtual ~R3BNveto_seg(){};

    virtual void Initialize(); // override;

    virtual Bool_t ProcessHits(FairVolume* = nullptr); // override;

    virtual void EndOfEvent(); // override;

    virtual void Register(); // override;

    virtual TClonesArray* GetCollection(Int_t iColl) const; // override;

    virtual void Print(Option_t* = "") const; // override;

    virtual void Reset(); // override;

    virtual void ConstructGeometry(); // override;

    virtual void PostTrack(); // override;

    virtual Bool_t CheckIfSensitive(std::string name); // override;

    /** May not be copy constructed */
    // R3BNveto_seg(const R3BNveto_seg &) = delete;
    // R3BNveto_seg &operator=(const R3BNveto_seg &) = delete;

  private:
    // std::unique_ptr<TClonesArray> fNveto_segPoints;
    TClonesArray* fNveto_segPoints;
    // std::unique_ptr<TClonesArray> fNveto_segPrimaryNeutronInteractionPoints;
    TClonesArray* fNveto_segPrimaryNeutronInteractionPoints;

    /** Track information to be stored until the track leaves the active volume. */
    // TODO: Just fill an existing R3Bveto_segPoint in the TClonesArray or
    //       Use an R3Bveto_segPoint as Member Variable to move construct into TClonesarray?
    Int_t fTrackID;
    Int_t fPaddleID;
    TLorentzVector fPosIn, fPosOut;
    TLorentzVector fMomIn, fMomOut;
    Double_t fTime;
    Double_t fLength;
    Double_t fELoss;
    Double_t fLightYield;
    Bool_t fLastHitDone;

    void ResetValues();

    void WriteParameterFile();

    /** ROOT ClassDef
     * ClassDefOverride to avoid inconsistent-missing-override warnings
     * ClassVersionID = 0, this class should not be read/written (no IO necessary)
     */
    // ClassDefOverride(R3BNveto_seg, 0);
    ClassDef(R3BNveto_seg, 0);
};

#endif // R3BNVETO_SEG_H
