#include "Digitizin_segEngine.h"

namespace Nveto_seg {

const Double_t Digitizin_segEngine::fSaturationCoefficient = 0.012;

const Double_t Digitizin_segEngine::fIntegrationTime = 400.;

//  c = 2.99792458E8;  // m/s
//  cMedia = c / 1.58 * 1e9; // speed of light in material in [cm/ns]
Double_t Digitizin_segEngine::fcMedium = 14.; // speed of light in material in [cm/ns]

// light attenuation of plastic scintillator
//att = 0.0047619047619; // [1/cm]
Double_t Digitizin_segEngine::fAttenuation = 0.008; // [1/cm]

// decay constant of BC408 scintillator
const Double_t Digitizin_segEngine::fLambda = 1. / 2.1; // [1/ns]

Double_t Digitizin_segEngine::fThresh = 0.16;

// TODO: So, a detector with less than 200 paddles is VETO_SEG?
/*if (!fveto_segDigiPar->GetGeometryFileName().Contains("proto") &&
        !fveto_segDigiPar->GetGeometryFileName().Contains("jap") &&
        npaddles < 202) {
    // VETO_SEG detector
    timeRes = 0.25; // ns
}
// FIXME: So, "jap" has a different time resolution?
if (fveto_segDigiPar->GetGeometryFileName().Contains("jap")) {
    timeRes = 0.010;
}*/
Double_t Digitizin_segEngine::fTimeRes = 0.15; // ns

Double_t Digitizin_segEngine::fPaddleHalfLength = 0.;

TRandom3 *Digitizin_segEngine::fRnd = new TRandom3();


Double_t Digitizin_segEngine::GetTriggerTime() const
{
    Double_t triggerTime = 1e100;
    for (const auto &kv : paddles) {
        const Paddle &paddle = kv.second;

        // TODO: Should be easier with std::min?
        if (paddle.leftPMT.HasFired() && paddle.leftPMT.GetTDC() < triggerTime) {
            triggerTime = paddle.leftPMT.GetTDC();
        }
        if (paddle.rightPMT.HasFired() && paddle.rightPMT.GetTDC() < triggerTime) {
            triggerTime = paddle.rightPMT.GetTDC();
        }
    }
    return triggerTime;
}


bool Digitizin_segEngine::PMT::HasFired() const
{
    if (!cachedFirstHitOverThresh.valid()) {
        cachedFirstHitOverThresh.set(FindThresholdExceedingHit());
        cachedQDC.invalidate();
        cachedTDC.invalidate();
        cachedEnergy.invalidate();
    }
    // If there is a hit that exceeded the threshold, the PMT fill fire
    return cachedFirstHitOverThresh.get() != hits.end();
}


Double_t Digitizin_segEngine::PMT::GetQDC() const
{
    if (!cachedQDC.valid()) {
        cachedQDC.set(BuildQDC());
    }
    return cachedQDC;
}


Double_t Digitizin_segEngine::PMT::BuildQDC() const
{
    if (HasFired()) {
        // @mheil: Should that be calculated using an exponetial with the
        // prior accumulated light / eponentail decay or just all the light that arrives?
        // Maxmimul pulse height or just sum over all light = charge?
        Double_t light = 0.;
        for (auto hit_it = cachedFirstHitOverThresh.get(); hit_it != hits.end(); hit_it++) {
            const PMTHit &hit = *hit_it;
            if (hit.time < (*cachedFirstHitOverThresh.get()).time + fIntegrationTime) {
                light += hit.light;
            }
        }
        return light;
    } else {
        return 0.;
    }
}


Double_t Digitizin_segEngine::PMT::GetTDC() const
{
    if (!cachedTDC.valid()) {
        cachedTDC.set(BuildTDC());
    }
    return cachedTDC;
}


Double_t Digitizin_segEngine::PMT::BuildTDC() const
{
    if (HasFired()) {
        return (*cachedFirstHitOverThresh.get()).time + fRnd->Gaus(0., fTimeRes);
    } else {
        return -1.;
    }
}


Double_t Digitizin_segEngine::PMT::GetEnergy() const
{
    if (!cachedEnergy.valid()) {
        cachedEnergy.set(BuildEnergy());
    }
    return cachedEnergy;
}


Double_t Digitizin_segEngine::PMT::BuildEnergy() const
{
    Double_t e = GetQDC();
    // Apply reverse attenuation (TODO: Should be last?)
    e = e * exp((2.*(fPaddleHalfLength)) * fAttenuation / 2.);
    // Apply saturation
    e = e / (1. + fSaturationCoefficient * e);
    // Apply energy smearing
    e = fRnd->Gaus(e, 0.05 * e);
    // Apply reverse saturation
    e = e / (1. - fSaturationCoefficient * e);
    return e;
}


std::vector<Digitizin_segEngine::PMTHit>::const_iterator Digitizin_segEngine::PMT::FindThresholdExceedingHit() const
{
    // Note that this accumulated light is NOT used for the QDC Value
    // @mheil: Is that actually correct?
    Double_t currentHeightOfLightPulse = 0.;
    Double_t previousTime = 0.;

    // TODO: Could do this shorter with find_if?
    for (std::vector<PMTHit>::const_iterator hit_it = hits.begin(); hit_it != hits.end(); hit_it++) {
        const PMTHit &hit = *hit_it;

        // Until the light of this hit arrives at the pmt, the previous light pulses have decayed
        currentHeightOfLightPulse *= exp(-fLambda * (hit.time - previousTime));
        previousTime = hit.time;

        // Add the current light pulse
        currentHeightOfLightPulse += fLambda * hit.light;

        // If the light pulse is higher than the treshold, this hit causes the pmt to fire
        if (currentHeightOfLightPulse > fThresh) {
            return hit_it;
        }
    }

    // Threshold was not exceeded
    return hits.end();
}


}; //namespace
