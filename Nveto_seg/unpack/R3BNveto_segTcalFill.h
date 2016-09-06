// ----------------------------------------------------------------
// -----                  R3BNveto_segTcalFill                  -----
// -----             Created 27-01-2015 by M.Heil             -----
// ----------------------------------------------------------------

#ifndef R3BNVETO_SEGTCALFILL_H
#define R3BNVETO_SEGTCALFILL_H

#include "FairTask.h"

class R3BTCalPar;
class TClonesArray;
class R3BEventHeader;
class R3BTCalEngine;

/**
 * An analysis task for TCAL calibration of Nveto_seg Tamex data.
 * This class fills TDC distribution for each Photomultiplier
 * of the Nveto_seg detector and calculates the calibration
 * parameter_seg using the R3BTCalEngine.
 * @author D. Kresan
 * @since September 7, 2015
 */
class R3BNveto_segTcalFill : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameter_seg.
     */
    R3BNveto_segTcalFill();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BNveto_segTcalFill(const char* name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BNveto_segTcalFill();

    /**
     * Method for task initialization.
     * This function is called by the framework before
     * the event loop.
     * @return Initialization status. kSUCCESS, kERROR or kFATAL.
     */
    virtual InitStatus Init();

    /**
     * Method for event loop implementation.
     * Is called by the framework every time a new event is read.
     * @param option an execution option.
     */
    virtual void Exec(Option_t* option);

    /**
     * A method for finish of processing of an event.
     * Is called by the framework for each event after executing
     * the tasks.
     */
    virtual void FinishEvent();

    /**
     * Method for finish of the task execution.
     * Is called by the framework after processing the event loop.
     */
    virtual void FinishTask();

    /**
     * Method for setting the update rate.
     * @param rate an update rate value (events).
     */
    inline void SetUpdateRate(Int_t rate)
    {
        fUpdateRate = rate;
    }

    /**
     * Method for setting minimum required statistics per module.
     * Only detector modules with number of entries in TDC
     * distribution greater than minimum statistics will be
     * calibrated.
     * @param minStats a value of minimum statistics required.
     */
    inline void SetMinStats(Int_t minStats)
    {
        fMinStats = minStats;
    }

    /**
     * Method for selecting events with certain trigger value.
     * @param trigger 1 - onspill, 2 - offspill, -1 - all events.
     */
    inline void SetTrigger(Int_t trigger)
    {
        fTrigger = trigger;
    }

    /**
     * Method for setting number of modules in Nveto_seg setup.
     * @param nPlanes number planes.
     * @param nBars number of bars per plane.
     * @param n17 a number of channels with stop signal (17-th channel).
     */
    inline void SetNofModules(Int_t nPlanes, Int_t nBars, Int_t n17)
    {
        fNofPlanes = nPlanes;		
        fNofBars = nBars;
        fNof17 = n17;
    }

  private:
    Int_t fUpdateRate; /**< An update rate. */
    Int_t fMinStats;   /**< Minimum statistics required per module. */
    Int_t fTrigger;    /**< Trigger value. */

    Int_t fNofPlanes; /**< Number of Nveto_seg planes. */
    Int_t fNofBars; /**< Number of Nveto_seg bars per plane. */
    Int_t fNof17;   /**< Number of channels with stop signal. */

    Int_t fNEvents;         /**< Event counter. */
    R3BTCalPar* fCal_Par;   /**< Parameter container. */
    TClonesArray* fHits;    /**< Array with Nveto_seg hits - input data. */
    R3BEventHeader* header; /**< Event header - input data. */

    R3BTCalEngine* fEngine; /**< Instance of the TCAL engine. */

  public:
    ClassDef(R3BNveto_segTcalFill, 1)
};

#endif // Test 1
