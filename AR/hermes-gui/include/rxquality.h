#pragma once
#include "gtu-core_global.h"
#include <QString>

struct GTUCORESHARED_EXPORT signalStatus
{
    signalStatus();
    QByteArray toByteArray();
    void fromByteArray(QByteArray &data);
    uint32_t timeOffsetMS = 0;
    int statVersion = 3;
    uint8_t fe_status = 0;
    float snr = std::numeric_limits<float>::signaling_NaN();    // = -1;
    float signal = std::numeric_limits<float>::signaling_NaN(); // = -1;
    float ber = std::numeric_limits<float>::signaling_NaN();    // = -1;
    uint32_t uncorrected_blocks = 0;                            // = 0;
    float strengthDbm = std::numeric_limits<float>::signaling_NaN(); // = -1;
    bool signalPresented = false;                                    /** Signal is presented.    */
    bool signalLocked = false;                                    /** Signal is locked.       */
    int signalQuality = -1;  /** Signal quality, from 0 (poor) to 100 (good).      */
    int signalStrength = 0;  /** Signal strength from 0 (weak) to 100 (strong).     */
    int reg_0xF73B = 0;
    int linkStatus = -1;
    //-1 no value, 0 closed, 1 oppenned, 9 setA, 17 set B, 33 Set ALL to zero

    int postVitErrorCount = 0; // ErrorBitCount.
    int postVitBitCount = 0;   // TotalBitCount.
    int abortCount = 0;        // Number of abort RSD packet.
    int frameErrorCount = 0; // Frame Error Ratio (error ratio before MPE-FEC) = frameErrorRate / 128.
    int mpefecFrameErrorCount
        = 0; // MPE-FEC Frame Error Ratio (error ratio after MPE-FEC) = mpefecFrameErrorCount / 128.

    int reg_0xF733 = 0;
    int reg_0xF734 = 0;
    int frequency = 0;
    int bandwith = 0;
    int channleStatistic1 = 0;
    int channleStatistic2 = 0;
    int errorPackages = 0;
    int activeChip = -1;
    int chipID = -1;
    int deviceIDS;
    float reserv1 = std::numeric_limits<float>::signaling_NaN(); // = -1;
    float reserv2 = std::numeric_limits<float>::signaling_NaN();
    float reserv3 = std::numeric_limits<float>::signaling_NaN();
    float reserv4 = std::numeric_limits<float>::signaling_NaN();
    uint32_t reserv5;
    uint32_t reserv6;
    int reserv7;
    int reserv8;
    int reserv9;
};
