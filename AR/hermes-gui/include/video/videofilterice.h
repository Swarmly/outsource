#pragma once
#include "hermes.h"
#include "videofilterbase.h"

class VideoFilterICE : public BaseOSDVideoFilter
{
public:
    enum engineMessages {eMSG_100_5 = 8, eMSG_107_B, eMSG_10C_F, eMSG_110_113, eMSG_114_115 };

    VideoFilterICE(QObject *parent);
    void setMavlinkMessage(const std::list<mavlink_message_t> &messages);

protected:
    void paintOSD(QPainter *painter) override;

private:
    float m_engineRPM = 0;
    float m_engineTemperatureHead1 = 0;
    float m_airTemperature = 0;
    float m_exhaustTemperatureHead1 = 0;
    float m_ecuTemperature = 0;
    float m_sensorErrorFlags = 0;
    float m_thermalLimitationFlags = 0;
    float m_injection_valve_load = 0;
    float m_injection_fuel_consumption = 0;
    float m_engineTemperatureHead2 = 0;
    float m_exhaustTemperatureHead2 = 0;
    float m_currentEngineTorqueICAOCorrected = 0;
    float m_currentEnginePowerICAOCorrected = 0;
    float m_fuelConsumptionICAOCorrected = 0;
    float m_specFuelConsumptionCurrent = 0;
    float m_mslEngineTemperatureHead1 = 0;
    float m_mslEngineTemperatureHead2 = 0;
    float m_mslExhaustTemperatureHead1 = 0;
    float m_mslExhaustTemperatureHead2 = 0;
    float m_currentOutput = 0;
    float m_supplyVoltage = 0;
    float m_bridgeVoltage = 0;
    float m_temperature = 0;
    float m_controllerTemperature = 0;
    float m_batteryCurrent = 0;
    QString m_fltMode;
};
