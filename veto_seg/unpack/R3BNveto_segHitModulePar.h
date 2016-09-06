#ifndef R3BNVETO_SEGHITMODULEPAR_H
#define R3BNVETO_SEGHITMODULEPAR_H

#include "FairParGenericSet.h"

#define NCHMAX 5000

class FairParamList;


class R3BNveto_segHitModulePar : public FairParGenericSet
{
public:
    /**
     * Standard constructor.
     * @param name a name of container.
     * @param title a title of container.
     * @param context context/purpose for parameter_seg and conditions.
     * @param own class ownership, if flag is kTRUE FairDB has the par.
     */
    R3BNveto_segHitModulePar ( const char* name = "Nveto_segHitModulePar",
                           const char* title = "Calibration of a Nveto_seg module",
                           const char* context = "TestDefaultContext",
                           Bool_t own = kTRUE );

    /**
     * Destructor.
     * Frees the memory allocated by the object.
     */
    virtual ~R3BNveto_segHitModulePar ( void );

    /**
     * A method to reset the parameter values. Sets all parameter_seg to 0.
     */
    void clear ( void );

    /**
     * A method to write parameter_seg using RuntimeDB.
     * @param list a list of parameter_seg.
     */
    void putParams ( FairParamList* list );

    /**
     * A method to read parameter_seg using RuntimeDB.
     * @param list a list of parameter_seg.
     * @return kTRUE if successfull, else kFALSE.
     */
    Bool_t getParams ( FairParamList* list );

    /**
     * A method to print value of parameter_seg to the standard
     * output using FairLogger.
     */
    void printParams();

    /** Accessor functions **/
    Int_t GetModuleId() const {
        return fModuleId;
    }
    Int_t GetSide() const {
        return fSide;
    }
    Double_t GetTimeOffset() const {
        return fTimeOffset;
    }
    Double_t GetTimeOffsetError() const {
	return fTimeOffsetError;
    }  
    Double_t GetEnergieGain() const {
        return fEnergieGain;
    }
    Double_t GetEnergieGainError() const{
	return fEnergieGainError;
    }
    Double_t GetEffectiveSpeed() const {
	return fEffectiveSpeed;
    }
    Double_t GetEffectiveSpeedError() const {
	return fEffectiveSpeedError;
    }
    void SetModuleId ( Int_t i ) {
        fModuleId = i;
    }
    void SetSide ( Int_t i ) {
        fSide = i;
    }
    void SetTimeOffset ( Double_t i ) {
        fTimeOffset = i;
    }
    void SetTimeOffsetError(Double_t i){
	fTimeOffsetError = i;
    }
    void SetEnergieGain ( Double_t i ) {
        fEnergieGain = i;
    }
    void SetEnergieGainError( Double_t i){
	fEnergieGainError =i;
    }
    void SetEffectiveSpeed ( Double_t i ){
	fEffectiveSpeed = i;
    }
    void SetEffectiveSpeedError( Double_t i){
	fEffectiveSpeedError = i;
    }

private:
    Int_t fModuleId;          /**< Index of a detector module. */
    Int_t fSide;              /**< Side of a module: for Nveto_seg - L/R PMT. */
    Double_t fTimeOffset;
    Double_t fTimeOffsetError;
    Double_t fEnergieGain;
    Double_t fEnergieGainError;
    Double_t fEffectiveSpeed;
    Double_t fEffectiveSpeedError;
    ClassDef ( R3BNveto_segHitModulePar, 1 );
};

#endif /* !R3BNVETO_SEGHITMODULEPAR_H*/
