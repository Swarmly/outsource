#include "coreutils.h"
#include <chrono>
#include <QDateTime>
#include <QDir>
#include <QQuaternion>
#include <QUuid>
#include <QVector2D>
#include <QVector3D>

static quint64 startTime = GetCurrentTimeStampMs();

enum streamTypes {
    ID_TYPE_BIN = 1,
    ID_TYPE_RAW,
    ID_TYPE_IMU,
    ID_TYPE_POS,
    ID_TYPE_RF,
    ID_TYPE_MAV2,
    ID_TYPE_TXT,
    ID_TYPE_SYS,
    ID_TYPE_FEC,
    ID_TYPE_H264,
    ID_TYPE_H265,
    ID_TYPE_JPEG,
    ID_TYPE_MJPEG,
    ID_TYPE_PNG,
    ID_TYPE_MPEGTS,
    ID_TYPE_GPS_RAW,
    ID_TYPE_GPS_RTSM,
    ID_TYPE_SYSTEM,
    ID_TYPE_KLV,
    ID_TYPE_MPEG2VIDEO,
    ID_TYPE_MAX_END
};

//#define ID_TYPE_BIN uchar(1)
//#define ID_TYPE_RAW uchar(2)
//#define ID_TYPE_IMU uchar(3)
//#define ID_TYPE_POS uchar(4)
//#define ID_TYPE_RF uchar(5)
//#define ID_TYPE_MAV2 uchar(6)
//#define ID_TYPE_TXT uchar(7)
//#define ID_TYPE_SYS uchar(8)
//#define ID_TYPE_FEC uchar(9)
//#define ID_TYPE_H264 uchar(10)
//#define ID_TYPE_H265 uchar(11)
//#define ID_TYPE_JPEG uchar(12)
//#define ID_TYPE_MJPEG uchar(13)
//#define ID_TYPE_PNG uchar(14)
//#define ID_TYPE_MPEGTS uchar(15)
//#define ID_TYPE_GPS_RAW uchar(16)
//#define ID_TYPE_GPS_RTSM uchar(17)
//#define ID_TYPE_SYSTEM uchar(18)
//#define ID_TYPE_KLV uchar(19)
//#define ID_TYPE_MPEG2VIDEO uchar(20)
//#define ID_TYPE_MAX_END uchar(21)

const QString typeBIN("bin_data");
const QString typeRAW("raw_data");
const QString typeIMU("imu");
const QString typePOS("pos");
const QString typeRF("rf2stat");
const QString typeMAV2("mavlink2");
const QString typeTXT("txt");
const QString typeSYS("sys");
const QString typeFEC("fec");
const QString typeH264("h264");
const QString typeH265("hevc");
const QString typeJPEG("jpeg");
const QString typeMJPEG("mjpeg");
const QString typePNG("png");
const QString typeTS("mpegts");
const QString typeGpsRaw("gps_raw");
const QString typeGpsRTSM("gps_rtsm");
const QString typeGpsSYSTEM("system");
const QString typeKLVData("klv-data");
const QString typeMPEG2VIDEO("mpeg2video");
const QString typeUNKNOWN("UNKNOWN FORMAT");

static QMap<streamTypes, QString> g_IDX2Type = {{ID_TYPE_BIN, typeBIN},
                                                {ID_TYPE_RAW, typeRAW},
                                                {ID_TYPE_IMU, typeIMU},
                                                {ID_TYPE_POS, typePOS},
                                                {ID_TYPE_RF, typeRF},
                                                {ID_TYPE_MAV2, typeMAV2},
                                                {ID_TYPE_TXT, typeTXT},
                                                {ID_TYPE_SYS, typeSYS},
                                                {ID_TYPE_FEC, typeFEC},
                                                {ID_TYPE_H264, typeH264},
                                                {ID_TYPE_H265, typeH265},
                                                {ID_TYPE_JPEG, typeJPEG},
                                                {ID_TYPE_MJPEG, typeMJPEG},
                                                {ID_TYPE_PNG, typePNG},
                                                {ID_TYPE_MPEGTS, typeTS},
                                                {ID_TYPE_GPS_RAW, typeGpsRaw},
                                                {ID_TYPE_GPS_RTSM, typeGpsRTSM},
                                                {ID_TYPE_SYSTEM, typeGpsSYSTEM},
                                                {ID_TYPE_KLV, typeKLVData},
                                                {ID_TYPE_MPEG2VIDEO, typeMPEG2VIDEO}};
static QMap<QString, streamTypes> g_Type2IDX = {{typeBIN, ID_TYPE_BIN},
                                                {typeRAW, ID_TYPE_RAW},
                                                {typeIMU, ID_TYPE_IMU},
                                                {typePOS, ID_TYPE_POS},
                                                {typeRF, ID_TYPE_RF},
                                                {typeMAV2, ID_TYPE_MAV2},
                                                {typeTXT, ID_TYPE_TXT},
                                                {typeSYS, ID_TYPE_SYS},
                                                {typeFEC, ID_TYPE_FEC},
                                                {typeH264, ID_TYPE_H264},
                                                {typeH265, ID_TYPE_H265},
                                                {typeJPEG, ID_TYPE_JPEG},
                                                {typeMJPEG, ID_TYPE_MJPEG},
                                                {typePNG, ID_TYPE_PNG},
                                                {typeTS, ID_TYPE_MPEGTS},
                                                {typeGpsRaw, ID_TYPE_GPS_RAW},
                                                {typeGpsRTSM, ID_TYPE_GPS_RTSM},
                                                {typeGpsSYSTEM, ID_TYPE_SYSTEM},
                                                {typeKLVData, ID_TYPE_KLV},
                                                {typeMPEG2VIDEO, ID_TYPE_MPEG2VIDEO}};

char CustomPacketTemmplate[] = {0x47, 0x1f, char(0xff), 0x1c, 0x00, 0x00};
//char CustomPacketBIN[] = {0x47, 0x10, ID_TYPE_BIN, 0x1c, 0x00, 0x00};
//char CustomPacketMavlink2[] = {0x47, 0x10, ID_TYPE_MAV2, 0x1c, 0x00, 0x00};
//char CustomPacketFEC[] = {0x47, 0x10, ID_TYPE_FEC, 0x1c, 0x00, 0x00};
//char CustomPacketRFStat[] = {0x47, 0x10, ID_TYPE_RF, 0x1c, 0x00, 0x00};
//char CustomPacketTXT[] = {0x47, 0x10, ID_TYPE_TXT, 0x1c, 0x00, 0x00};
//char CustomPacketH264[] = {0x47, 0x10, ID_TYPE_H264, 0x1c, 0x00, 0x00};
//char CustomPacketH265[] = {0x47, 0x10, ID_TYPE_H265, 0x1c, 0x00, 0x00};
//char CustomPacketMJPEG[] = {0x47, 0x10, ID_TYPE_MJPEG, 0x1c, 0x00, 0x00};
//char CustomPacketGPS_RAW[] = {0x47, 0x10, ID_TYPE_GPS_RAW, 0x1c, 0x00, 0x00};
//char CustomPackeGPS_RTSM[] = {0x47, 0x10, ID_TYPE_GPS_RTSM, 0x1c, 0x00, 0x00};

char CustomPacketNULL[] = {0x47, 0x1f, char(0xFF), 0x1c, 0x00, 0x00};

char CustomPacketNULLFull[]
    = {char(0x47), char(0x1f), char(0xff), char(0x1c), char(0x00), char(0x00), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff),
       char(0xff), char(0xff), char(0xff), char(0xff), char(0xff), char(0xff)};

const QString &idxToFormat(uint8_t id)
{
    if (!g_IDX2Type.contains(streamTypes(id))) {
        LOG_ERROR_EXT << "Incorrect format index for custom demuxer:" << id;
        return typeUNKNOWN;
    }
    return g_IDX2Type[streamTypes(id)];
}

uint8_t formatToIdx(const QString &format)
{
    if (!g_Type2IDX.contains(format)) {
        LOG_ERROR_EXT << "Incorrect format for custom muxer: " << format;
        return 0;
    }
    return uint8_t(g_Type2IDX[format]);
}

int preparePackage(uint8_t sourceID, uint8_t streamID, const QString& format, QByteArray& data)
{
    if (data.size() < int(sizeof(CustomPacketTemmplate))) {
        data.resize(sizeof(CustomPacketTemmplate));
    }
    memcpy(data.data(), CustomPacketTemmplate, sizeof(CustomPacketTemmplate));

    data[2] = formatToIdx(format);
    data[4] = sourceID & 0x7f;
    data[5] = streamID & 0x1f;

    return sizeof(CustomPacketTemmplate);
}

bool isTemplateMatched(const char *dataPtr)
{
    const uchar *dataLocal = (const uchar *) dataPtr;

    if (dataLocal[1] & 0x80) {
        return false;
    }

    if (dataLocal[0] == CustomPacketTemmplate[0] && (dataLocal[1] & 127) == CustomPacketTemmplate[1]
        && dataLocal[3] == CustomPacketTemmplate[3] && dataLocal[4] <= 127 && dataLocal[5] <= 31) {
        return true;
    }
    return false;
}

bool isRFPackage(const char *dataPtr)
{
    if (dataPtr[2] == ID_TYPE_RF) {
        return true;
    }
    return false;
}

bool isBINPackage(const char *dataPtr)
{
    if (dataPtr[2] == ID_TYPE_BIN) {
        return true;
    }
    return false;
}

int search0x47Sync(QByteArray &inOut)
{
    int offset = 0;
    int storageSize = inOut.size();
    const char *bufferPtr = inOut.data();
    for (offset = 0; offset < storageSize; ++offset) {
        if (*(bufferPtr + offset) == 'G') {
            break;
        }
    }
    if (offset) {
        inOut.remove(0, offset);
    }
    return offset;
}

bool CompareString(const QString &str1, const QString &str2)
{
    return 0 == str1.compare(str2, Qt::CaseInsensitive);
}

quint64 GetCurrentTimeStampNs()
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::system_clock::now().time_since_epoch())
            .count();
}

quint64 GetCurrentTimeStampMs()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch())
            .count();
}

quint64 GetStartTimeMs()
{
    return GetCurrentTimeStampMs() - startTime;
}

GtuQuaternion::GtuQuaternion() {}
GtuQuaternion::GtuQuaternion(const QQuaternion &quat)
    : w(quat.scalar())
    , x(quat.x())
    , y(quat.y())
    , z(quat.z())
{}
GtuQuaternion::GtuQuaternion(double inW, double inX, double inY, double inZ)
    : w(inW)
    , x(inX)
    , y(inY)
    , z(inZ)
{}

QQuaternion GtuQuaternion::toQt()
{
    return QQuaternion(w, x, y, z);
}

GtuQVector3D::GtuQVector3D() {}
GtuQVector3D::GtuQVector3D(double inX, double inY, double inZ)
    : x(inX)
    , y(inY)
    , z(inZ)
{}

QVector3D GtuQVector3D::toQt()
{
    return QVector3D(x, y, z);
}

GtuQVector2D::GtuQVector2D() {}
GtuQVector2D::GtuQVector2D(double inX, double inY)
    : x(inX)
    , y(inY)

{}

QVector2D GtuQVector2D::toQt()
{
    return QVector2D(x, y);
}

QJsonValue GTUCORESHARED_EXPORT GetJsonValue(const QJsonObject &obj, const QString &key)
{
    if (obj.contains(key)) {
        return obj.value(key);
    }
    if (obj.contains(key.toUpper())) {
        return obj.value(key.toUpper());
    }
    if (obj.contains(key.toLower())) {
        return obj.value(key.toLower());
    }
    return QJsonValue();
}

bool GTUCORESHARED_EXPORT IsJsonValueContains(const QJsonObject &obj, const QString &key)
{
    if (obj.contains(key)) {
        return true;
    }
    if (obj.contains(key.toLower())) {
        return true;
    }
    if (obj.contains(key.toUpper())) {
        return true;
    }
    return false;
}

QString NormalizeFileName(QString path, int *number)
{
    if (path.contains("%home%")) {
        path.replace("%home%", QDir::homePath());
    }

    if (path.contains("%time%")) {
        QString textdate = QDateTime::currentDateTime().toString(Qt::TextDate);
        textdate.replace(':', '.');
        textdate.replace(' ', '_');
        path.replace("%time%", textdate);
    }

    if (path.contains("%uid%")) {
        QUuid u = QUuid::createUuid();
        QString str = u.toString();
        str.replace('{', "");
        str.replace('}', "");
        path.replace("%uid%", str);
    }

    if (path.contains("%autoinc%")) {
        int num = 0;
        QFileInfo fileINfo(path);
        QDir myDir(fileINfo.absoluteDir());
        QStringList filesList = myDir.entryList(QStringList() << QString("*." + fileINfo.suffix()),
                                                QDir::Files);
        if (!filesList.empty()) {
            for (auto lastFile : filesList) {
                int startPos = fileINfo.fileName().indexOf("%autoinc%");
                if (startPos + 4 < lastFile.length()) {
                    lastFile = lastFile.mid(startPos, 4);
                    if (lastFile.toInt() > num) {
                        num = lastFile.toInt();
                    }
                }
            }
        }
        ++num;
        if (number) {
            if (number) {
                *number = num;
            }
        }
        path.replace("%autoinc%", QString::number(num).rightJustified(4, '0'));
    }
    return path;
}

int is_big_endian(void)
{
    union {
        uint32_t i;
        char c[4];
    } e = {0x01000000};

    return e.c[0];
}
void put_arcg_value(char *buff, int &value)
{
    char *tmpVal = (char *) &value;
    if (is_big_endian()) {
        buff[0] = *tmpVal;
        ++tmpVal;
        buff[1] = *tmpVal;
        ++tmpVal;
        buff[2] = *tmpVal;
        ++tmpVal;
        buff[3] = *tmpVal;
    } else {
        buff[3] = *tmpVal;
        ++tmpVal;
        buff[2] = *tmpVal;
        ++tmpVal;
        buff[1] = *tmpVal;
        ++tmpVal;
        buff[0] = *tmpVal;
    }
}
void get_arcg_value(char *buff, int &value)
{
    char *tmpVal = (char *) &value;
    if (is_big_endian()) {
        *tmpVal = buff[0];
        ++tmpVal;
        *tmpVal = buff[1];
        ++tmpVal;
        *tmpVal = buff[2];
        ++tmpVal;
        *tmpVal = buff[3];
    } else {
        *tmpVal = buff[3];
        ++tmpVal;
        *tmpVal = buff[2];
        ++tmpVal;
        *tmpVal = buff[1];
        ++tmpVal;
        *tmpVal = buff[0];
    }
}
//EulerAngles ToEulerAngles(Quaternion q)
//{
//    EulerAngles angles;

//    // roll (x-axis rotation)
//    double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
//    double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
//    angles.roll = std::atan2(sinr_cosp, cosr_cosp);

//    // pitch (y-axis rotation)
//    double sinp = 2 * (q.w * q.y - q.z * q.x);
//    if (std::abs(sinp) >= 1)
//        angles.pitch = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
//    else
//        angles.pitch = std::asin(sinp);

//    // yaw (z-axis rotation)
//    double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
//    double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
//    angles.yaw = std::atan2(siny_cosp, cosy_cosp);

//    return angles;
//}

//Quaternion ToQuaternion(double yaw, double pitch, double roll) // yaw (Z), pitch (Y), roll (X)
//{
//    // Abbreviations for the various angular functions
//    double cy = cos(yaw * 0.5);
//    double sy = sin(yaw * 0.5);
//    double cp = cos(pitch * 0.5);
//    double sp = sin(pitch * 0.5);
//    double cr = cos(roll * 0.5);
//    double sr = sin(roll * 0.5);

//    Quaternion q;
//    q.w = cy * cp * cr + sy * sp * sr;
//    q.x = cy * cp * sr - sy * sp * cr;
//    q.y = sy * cp * sr + cy * sp * cr;
//    q.z = sy * cp * cr - cy * sp * sr;

//    return q;
//}

void ensure_directories(QString const &location) {
    if (QDir().mkpath(QFileInfo(location).absoluteDir().absolutePath())) {
        LOG_DEBUG_EXT << "Directory added -  " << location;
    }
}
