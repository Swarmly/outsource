#include "video/videofilterice.h"
#include "video/videofiltervertical.h"
#include <stddef.h>

QString getModeFltMode(uint32_t mode)
{
    enum Mode {
        MANUAL = 0,
        CIRCLE = 1,
        STABILIZE = 2,
        TRAINING = 3,
        ACRO = 4,
        FLY_BY_WIRE_A = 5,
        FLY_BY_WIRE_B = 6,
        CRUISE = 7,
        AUTOTUNE = 8,
        RESERVED_9 = 9, // RESERVED FOR FUTURE USE
        AUTO = 10,
        RTL = 11,
        LOITER = 12,
        RESERVED_13 = 13, // RESERVED FOR FUTURE USE
        RESERVED_14 = 14, // RESERVED FOR FUTURE USE
        GUIDED = 15,
        INITIALIZING = 16,
        QSTABILIZE = 17,
        QHOVER = 18,
        QLOITER = 19,
        QLAND = 20,
        QRTL = 21,
        modeCount
    };

    QMap<uint32_t, QString> enumToString;
    if (enumToString.empty()) {
        enumToString.insert(MANUAL, "Manual");
        enumToString.insert(CIRCLE, "Circle");
        enumToString.insert(STABILIZE, "Stabilize");
        enumToString.insert(TRAINING, "Training");
        enumToString.insert(ACRO, "Acro");
        enumToString.insert(FLY_BY_WIRE_A, "FBW A");
        enumToString.insert(FLY_BY_WIRE_B, "FBW B");
        enumToString.insert(CRUISE, "Cruise");
        enumToString.insert(AUTOTUNE, "Autotune");
        enumToString.insert(AUTO, "Auto");
        enumToString.insert(RTL, "RTL");
        enumToString.insert(LOITER, "Loiter");
        enumToString.insert(GUIDED, "Guided");
        enumToString.insert(INITIALIZING, "Initializing");
        enumToString.insert(QSTABILIZE, "QuadPlane Stabilize");
        enumToString.insert(QHOVER, "QuadPlane Hover");
        enumToString.insert(QLOITER, "QuadPlane Loiter");
        enumToString.insert(QLAND, "QuadPlane Land");
        enumToString.insert(QRTL, "QuadPlane RTL");
    }
    return enumToString[mode];
}

// compile time DGB hash function
unsigned constexpr string_hash(const char *input)
{
    return *input ? static_cast<unsigned int>(*input) + 33 * string_hash(input + 1) : 5381;
}

constexpr long long operator"" _sh(const char *s, size_t)
{
    return string_hash(s);
}

VideoFilterICE::VideoFilterICE(QObject *parent)
    : BaseOSDVideoFilter(parent)
{

}

void VideoFilterICE::setMavlinkMessage(const std::list<mavlink_message_t> &messages)
{
    for (auto &it : messages) {
        if (it.msgid == MAVLINK_MSG_ID_DATA16) {
            mavlink_data16_t rawValue;
            mavlink_msg_data16_decode(&it, &rawValue);

            // filter by parameter name
            switch (rawValue.type) {
            case eMSG_100_5: // 1
            {
                m_engineRPM = *(uint16_t *)&rawValue.data[0];
                m_engineTemperatureHead1 = *(uint16_t *)&rawValue.data[8] / 10.0;
                m_airTemperature = *(uint16_t *)&rawValue.data[10] / 10.0;
                m_exhaustTemperatureHead1 = *(uint16_t *)&rawValue.data[12] / 10.0;
                m_ecuTemperature = *(uint16_t *)&rawValue.data[14] / 10.0;
                break;
            }
            case eMSG_107_B: // 2
            {
                m_sensorErrorFlags = *(uint16_t *)&rawValue.data[0];
                m_thermalLimitationFlags = *(uint16_t *)&rawValue.data[2];
                m_injection_valve_load = *(uint16_t *)&rawValue.data[12] / 10.0;
                m_injection_fuel_consumption = *(uint16_t *)&rawValue.data[14] / 1000.0;
                break;
            }
            case eMSG_10C_F: // 3
            {
                m_engineTemperatureHead2 = *(uint16_t *)&rawValue.data[4] / 10.0;
                m_exhaustTemperatureHead2 = *(uint16_t *)&rawValue.data[6] / 10.0;
                m_currentEngineTorqueICAOCorrected = *(uint16_t *)&rawValue.data[8] / 10.0;
                m_currentEnginePowerICAOCorrected = *(uint16_t *)&rawValue.data[10] / 100.0;
                m_fuelConsumptionICAOCorrected = *(uint16_t *)&rawValue.data[12] / 1000.0;
                m_specFuelConsumptionCurrent = *(uint16_t *)&rawValue.data[14];
                break;
            }
            case eMSG_110_113: // 4
            {
                m_mslEngineTemperatureHead1 =*(uint16_t *)&rawValue.data[0] / 10.0;
                m_mslEngineTemperatureHead2 = *(uint16_t *)&rawValue.data[2] / 10.0;
                m_mslExhaustTemperatureHead1 = *(uint16_t *)&rawValue.data[4] / 10.0;
                m_mslExhaustTemperatureHead2 = *(uint16_t *)&rawValue.data[6] / 10.0;
                m_currentOutput = *(uint16_t *)&rawValue.data[10];
                break;
            }
            case eMSG_114_115: // 5
            {
                m_supplyVoltage = *(uint16_t *)&rawValue.data[4] / 100.0;
                m_bridgeVoltage = *(uint16_t *)&rawValue.data[6] / 100.0;
                m_temperature = *(uint16_t *)&rawValue.data[8] / 10.0;
                m_controllerTemperature = *(uint16_t *)&rawValue.data[10] / 10.0;
                m_batteryCurrent = *(uint16_t *)&rawValue.data[12] / 1000.0;
                break;
            }
            default:
                break;
            }
        } else if (it.msgid == MAVLINK_MSG_ID_HEARTBEAT) {
            if (it.sysid == 1 && it.compid == 1) {
                mavlink_heartbeat_t heartbeat_msg;
                mavlink_msg_heartbeat_decode(&it, &heartbeat_msg);
                m_fltMode = getModeFltMode(heartbeat_msg.custom_mode);
            }
        }
    }
}

void VideoFilterICE::paintOSD(QPainter *painter)
{
    QOSDVerticalLayout layout;
    QString msg;

    msg = "Flt mode:: " + m_fltMode;
    layout.addText(msg);

    msg = "Engine speed: " + QString::number(m_engineRPM, 'g', 4) + " RPM";
    layout.addText(msg);

    msg = "Ecu Temperature: " + QString::number(m_ecuTemperature, 'g', 4) + " °C";
    layout.addText(msg);

    msg = "Head 1 temperature: " + QString::number(m_engineTemperatureHead1, 'g', 4) + " °C";
    layout.addText(msg);

    msg = "Head 2 temperature: " + QString::number(m_engineTemperatureHead2, 'g', 4) + " °C";
    layout.addText(msg);

    msg = "Fuel Consumption ICAO Corrected: "
            + QString::number(m_fuelConsumptionICAOCorrected, 'g', 4) + " l/h";
    layout.addText(msg);

    msg = "Generator:";
    layout.addText(msg);

    msg = "Current Output: " + QString::number(m_currentOutput, 'g', 4) + " A";
    layout.addText(msg);

    msg = "Supply Voltage: " + QString::number(m_supplyVoltage, 'g', 4) + " V";
    layout.addText(msg);

    msg = "Bridge Voltage: " + QString::number(m_bridgeVoltage, 'g', 4) + " V";
    layout.addText(msg);

    layout.paint(0, painter->window().height() - 196, painter);
}
