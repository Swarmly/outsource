#pragma once
#include "gtu-core_global.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QSharedPointer>
#include <QWeakPointer>
#define CUSTOM_HEADER_SIZE_SMALL 6
#define GTUSharedPTR QSharedPointer
#define GTUWeakPTR QWeakPointer

QJsonValue GTUCORESHARED_EXPORT GetJsonValue(const QJsonObject &obj, const QString &key);
bool GTUCORESHARED_EXPORT IsJsonValueContains(const QJsonObject &obj, const QString &key);
QString GTUCORESHARED_EXPORT NormalizeFileName(QString path, int *number = nullptr);
bool GTUCORESHARED_EXPORT CompareString(const QString &str1, const QString &str2);

template<class TStrCompatValue>
QList<TStrCompatValue> load_array_or_string(const QJsonObject &props, const QString &name)
{
    QList<TStrCompatValue> values;
    if (IsJsonValueContains(props, name)) {
        QJsonValue relative_values = GetJsonValue(props, name);
        if (relative_values.isArray()) {
            QJsonArray valArray = relative_values.toArray();
            for (auto&& rel_value: valArray) {
                values.push_back(TStrCompatValue{rel_value.toString()});
            }
        } else if (relative_values.isString()) {
            values.push_back(TStrCompatValue{relative_values.toString()});
        }
    }
    return values;
}

//extern char GTUCORESHARED_EXPORT CustomPacketTemmplate[CUSTOM_HEADER_SIZE_SMALL];
//extern char GTUCORESHARED_EXPORT CustomPacketBIN[CUSTOM_HEADER_SIZE_SMALL];
//extern char GTUCORESHARED_EXPORT CustomPacketMavlink2[CUSTOM_HEADER_SIZE_SMALL];
//extern char GTUCORESHARED_EXPORT CustomPacketRFStat[CUSTOM_HEADER_SIZE_SMALL];
//extern char GTUCORESHARED_EXPORT CustomPacketTXT[CUSTOM_HEADER_SIZE_SMALL];
//extern char GTUCORESHARED_EXPORT CustomPacketSYS[CUSTOM_HEADER_SIZE_SMALL];
//extern char GTUCORESHARED_EXPORT CustomPacketH264[CUSTOM_HEADER_SIZE_SMALL];
//extern char GTUCORESHARED_EXPORT CustomPacketH265[CUSTOM_HEADER_SIZE_SMALL];
//extern char GTUCORESHARED_EXPORT CustomPacketMJPEG[CUSTOM_HEADER_SIZE_SMALL];
//extern char GTUCORESHARED_EXPORT CustomPacketFEC[CUSTOM_HEADER_SIZE_SMALL];
//extern char GTUCORESHARED_EXPORT CustomPacketNULL[CUSTOM_HEADER_SIZE_SMALL];
extern char GTUCORESHARED_EXPORT CustomPacketNULLFull[188];

extern const QString GTUCORESHARED_EXPORT typeBIN;
extern const QString GTUCORESHARED_EXPORT typeMAV2;
//extern const QString GTUCORESHARED_EXPORT typeMAV;
extern const QString GTUCORESHARED_EXPORT typeRF;
extern const QString GTUCORESHARED_EXPORT typeTXT;
extern const QString GTUCORESHARED_EXPORT typeSYS;
//extern const QString GTUCORESHARED_EXPORT typeMavStream;
extern const QString GTUCORESHARED_EXPORT typeH264;
extern const QString GTUCORESHARED_EXPORT typeH265;
extern const QString GTUCORESHARED_EXPORT typeFEC;
extern const QString GTUCORESHARED_EXPORT typeJPEG;
extern const QString GTUCORESHARED_EXPORT typeMJPEG;
extern const QString GTUCORESHARED_EXPORT typePNG;
extern const QString GTUCORESHARED_EXPORT typeIMU;
extern const QString GTUCORESHARED_EXPORT typePOS;
extern const QString GTUCORESHARED_EXPORT typeRAW;
extern const QString GTUCORESHARED_EXPORT typeTS;
extern const QString GTUCORESHARED_EXPORT typeGpsRaw;
extern const QString GTUCORESHARED_EXPORT typeGpsRTSM;
extern const QString GTUCORESHARED_EXPORT typeGpsSYSTEM;
extern const QString GTUCORESHARED_EXPORT typeKLVData;
extern const QString GTUCORESHARED_EXPORT typeMPEG2VIDEO;
extern const QString GTUCORESHARED_EXPORT typeUNKNOWN;

GTUCORESHARED_EXPORT int preparePackage(uint8_t sourceID, uint8_t streamID, const QString& format, QByteArray& data);
GTUCORESHARED_EXPORT const QString & idxToFormat(uint8_t id);
GTUCORESHARED_EXPORT uint8_t formatToIdx(const QString &format);
GTUCORESHARED_EXPORT bool isTemplateMatched(const char *dataPtr);
bool GTUCORESHARED_EXPORT isRFPackage(const char *dataPtr);
bool GTUCORESHARED_EXPORT isBINPackage(const char *dataPtr);
int GTUCORESHARED_EXPORT search0x47Sync(QByteArray &inOut);

struct GTUCORESHARED_EXPORT GtuQuaternion
{
    GtuQuaternion();
    GtuQuaternion(const QQuaternion &quat);
    GtuQuaternion(double inW, double inX, double inY, double inZ);
    QQuaternion toQt();
    double w = 1;
    double x = 0;
    double y = 0;
    double z = 0;
};

struct GTUCORESHARED_EXPORT GtuQVector3D
{
    GtuQVector3D();
    GtuQVector3D(double inX, double inY, double inZ);
    QVector3D toQt();
    double x = 0;
    double y = 0;
    double z = 0;
};

struct GTUCORESHARED_EXPORT GtuQVector2D
{
    GtuQVector2D();
    GtuQVector2D(double inX, double inY);
    QVector2D toQt();
    double x = 0;
    double y = 0;
};

struct GTUCORESHARED_EXPORT GtuPOS2D
{
    GtuPOS2D();
    GtuPOS2D(double lon, double lat);
    double lon = 0;
    double lat = 0;
};

struct GTUCORESHARED_EXPORT GtuPOS3D
{
    GtuPOS3D();
    GtuPOS3D(double lon, double lat, double alt);
    double lon = 0;
    double lat = 0;
    double alt = 0;
};

struct GTUCORESHARED_EXPORT GtuEulerAngles
{
    double roll = 0;
    double pitch = 0;
    double yaw = 0;
};

using GtuQuaternionPtr = GTUSharedPTR<GtuQuaternion>;
using GtuPosition3DPtr = GTUSharedPTR<GtuQVector3D>;
using GtuPosition2DPtr = GTUSharedPTR<GtuQVector2D>;
using GtuEulerAnglesPtr = GTUSharedPTR<GtuEulerAngles>;
quint64 GTUCORESHARED_EXPORT GetCurrentTimeStampNs();
quint64 GTUCORESHARED_EXPORT GetCurrentTimeStampMs();
quint64 GTUCORESHARED_EXPORT GetStartTimeMs();

GTUCORESHARED_EXPORT int is_big_endian(void);
GTUCORESHARED_EXPORT void put_arcg_value(char *buff, int &value);
GTUCORESHARED_EXPORT void get_arcg_value(char *buff, int &value);
//GTUCORESHARED_EXPORT EulerAngles ToEulerAngles(Quaternion q);
//GTUCORESHARED_EXPORT Quaternion ToQuaternion(double yaw,
//                                             double pitch,
//                                             double roll); // yaw (Z), pitch (Y), roll (X)

GTUCORESHARED_EXPORT void ensure_directories(QString const &location);
