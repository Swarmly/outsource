#include "hermes.h"
#include "QEventLoop"
#include "commoncommands.h"
#include "coreutils.h"
#include "datasample.h"
#include "gtucore.h"
#include "variablemanager.h"

static const QString sensorName("hermes");
//static GTUSharedPTR<Hermes> g_sensorPtr;

static IBaseSensorSPtr createSensor(QObject *parent)
{
    return Hermes::getHMSensor(parent);
}
static bool result = GTUCore::Impl().registerPlugin(sensorName, &createSensor);

Q_DECLARE_METATYPE(mavlink_message_t);
Q_DECLARE_METATYPE(mavlink_message_t *);
Q_DECLARE_METATYPE(std::list<mavlink_message_t>);
Q_DECLARE_METATYPE(DataSampleCSPtr);
Q_DECLARE_METATYPE(boost::any);

GTUSharedPTR<Hermes> Hermes::getHMSensor(QObject *parent)
{
    static GTUSharedPTR<Hermes> sensor(new Hermes(parent));
    return sensor;
}

Hermes::Hermes(QObject *parent)
    : IBaseSensor(parent)
{
    m_name = sensorName;
    qRegisterMetaType<mavlink_message_t>();
    qRegisterMetaType<mavlink_message_t *>();
    qRegisterMetaType<std::list<mavlink_message_t>>();
    qRegisterMetaType<DataSampleCSPtr>();
    qRegisterMetaType<boost::any>();
    //LOCAL_ASSERT(connect(m_widget, &GroundStation::commandSend, this, &GSCRouter::commandReSend));
}

Hermes::~Hermes()
{
    disconnect();
    Stop();
}

void Hermes::Stop()
{
    IBaseSensor::Stop();
}

const QString &Hermes::GetSensorType() const
{
    return MyName();
}

const QString &Hermes::MyName()
{
    return sensorName;
}

Hermes *Hermes::getImpl(QObject *parent)
{
    return Hermes::getHMSensor(parent).get();
}

GTUSharedPTR<IBaseSensor> Hermes::getISensor(QObject *parent)
{
    return Hermes::getHMSensor(parent);
}

void Hermes::onEventDown(const QString &event, const boost::any &param, EVDownData & /*producer*/)
{
    eventDown(event, param);
}

void Hermes::WriteSample(const DataSampleCSPtr &data)
{
    if (CompareString(data->format(), typeTXT)) {
        QString name;
        if (!data->sensorName().isEmpty()) {
            name = data->sensorName() + ":";
        }
        name += QString::number(data->GetStreamID() & 7);
        QString str(data->GetByteArray().toStdString().c_str());
        txtFrame(str, name);
    } else {
        dataFrame(data);
    }
    if (data->format() == typeMAV2) {
        if (data->metaInfo().hasProp("mav2")) {
            std::list<mavlink_message_t> outMessages;
            outMessages = data->metaInfo().getProp("mav2", outMessages);
            mavlinkMessage(outMessages);
        }
    }
}

void Hermes::onLogMessage(QString str, QString name)
{
    txtFrame(str, name);
}

void Hermes::commandReSend(const QString &command, const boost::any &params)
{
    oneShotCMD(command, params);
}

void Hermes::eventUpReSend(const QString &event, const boost::any &params)
{
    onEventUp(event, params);
}

void Hermes::onCommand(const QString &command, const boost::any &params)
{
    IBaseSensor::onCommand(command, params);
    commandCame(command, params);
}

void Hermes::Init(const QJsonObject &props)
{
    IBaseSensor::Init(props);
    initProps(props);
}
