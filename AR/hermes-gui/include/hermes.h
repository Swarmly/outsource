#pragma once
#include "VideoRenderer.h"
#include "gui_tools.h"
#include "datasample.h"
//#include "ibasesensor.h"
#include "video/videofilterbase.h"

class HERMESGUI_EXPORT Hermes : public QObject
{
    Q_OBJECT
public:
    static Hermes *getImpl(QObject *parent = nullptr);
    Hermes(QObject *parent = nullptr);
    ~Hermes();
    const QString &GetSensorType() const ;
    static const QString &MyName();
    void WriteSample(const DataSampleCSPtr &data) ;
    void onCommand(const QString &command, const boost::any &params) ;
    void Init(const QJsonObject &props) ;
    //void onEventDown(const QString &event, const boost::any &param, EVDownData &visited) ;
    void Stop();

    static GTUSharedPTR<Hermes> getHMSensor(QObject *parent = nullptr);
    //static GTUSharedPTR<IBaseSensor> getISensor(QObject *parent = nullptr);

signals:
    void txtFrame(QString str, QString name);
    void dataFrame(const DataSampleCSPtr &frame);
    void eventDown(const QString &event, const boost::any &param);
    void commandCame(const QString &event, const boost::any &param);
    void initProps(const QJsonObject &props);
    void mavlinkMessage(const std::list<mavlink_message_t> &messages);

public slots:
    void onLogMessage(QString str, QString name);
    void commandReSend(const QString &command, const boost::any &params);
    void eventUpReSend(const QString &command, const boost::any &params);
};
