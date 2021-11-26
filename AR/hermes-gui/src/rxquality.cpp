#include "rxquality.h"
#include "coreutils.h"
#include <QDataStream>

signalStatus::signalStatus()
    : timeOffsetMS(GetStartTimeMs())
{}

QByteArray signalStatus::toByteArray()
{
    QByteArray data;
    QDataStream dataStream(&data, QIODevice::ReadWrite);

    dataStream << timeOffsetMS;
    dataStream << fe_status;
    dataStream << snr;
    dataStream << signal;
    dataStream << ber;
    dataStream << uncorrected_blocks;
    dataStream << strengthDbm;
    dataStream << signalPresented;
    dataStream << signalLocked;
    dataStream << signalQuality;
    dataStream << signalStrength;
    dataStream << reg_0xF73B;
    dataStream << postVitErrorCount;
    dataStream << postVitBitCount;
    dataStream << abortCount;
    dataStream << frameErrorCount;
    dataStream << mpefecFrameErrorCount;
    dataStream << linkStatus;
    dataStream << reg_0xF733;
    dataStream << reg_0xF734;
    dataStream << frequency;
    dataStream << bandwith;
    dataStream << channleStatistic1;
    dataStream << channleStatistic2;
    dataStream << errorPackages;
    dataStream << activeChip;
    dataStream << chipID;
    dataStream << statVersion;
    dataStream << deviceIDS;
    dataStream << reserv1;
    dataStream << reserv2;
    dataStream << reserv3;
    dataStream << reserv4;
    dataStream << reserv5;
    dataStream << reserv6;
    dataStream << reserv7;
    dataStream << reserv8;
    dataStream << reserv9;
    return data;
}

void signalStatus::fromByteArray(QByteArray &data)
{
    QDataStream dataStream(&data, QIODevice::ReadOnly);

    dataStream >> timeOffsetMS;
    dataStream >> fe_status;
    dataStream >> snr;
    dataStream >> signal;
    dataStream >> ber;
    dataStream >> uncorrected_blocks;
    dataStream >> strengthDbm;
    dataStream >> signalPresented;
    dataStream >> signalLocked;
    dataStream >> signalQuality;
    dataStream >> signalStrength;
    dataStream >> reg_0xF73B;
    dataStream >> postVitErrorCount;
    dataStream >> postVitBitCount;
    dataStream >> abortCount;
    dataStream >> frameErrorCount;
    dataStream >> mpefecFrameErrorCount;
    dataStream >> linkStatus;
    dataStream >> reg_0xF733;
    dataStream >> reg_0xF734;
    dataStream >> frequency;
    dataStream >> bandwith;
    dataStream >> channleStatistic1;
    dataStream >> channleStatistic2;
    dataStream >> errorPackages;
    dataStream >> activeChip;
    dataStream >> chipID;
    dataStream >> statVersion;
    dataStream >> deviceIDS;
    dataStream >> reserv1;
    dataStream >> reserv2;
    dataStream >> reserv3;
    dataStream >> reserv4;
    dataStream >> reserv5;
    dataStream >> reserv6;
    dataStream >> reserv7;
    dataStream >> reserv8;
    dataStream >> reserv9;
}
