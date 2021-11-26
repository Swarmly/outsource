#pragma once
#include "QtAV/Statistics.h"
#include "baseguiwidget.h"
#include "hermes.h"
#include "timeouttimer.h"
#include <QMenu>
#include <QTimer>
#include <QWidget>
#include <QtAV/Filter.h>
#include <QtAV/VideoRenderer.h>

namespace Ui {
class VdWidget;
}

class HERMESGUI_EXPORT VdWidget : public BaseGuiWidget
{
    Q_OBJECT

public:
    explicit VdWidget(QWidget *parent = nullptr);
    ~VdWidget();
    void addFilter(GTUSharedPTR<BaseOSDVideoFilter> filter);
    void installFilters(QtAV::VideoRenderer *renderer);
    QPointF getVideoWidgetSize();

public slots:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void onCommand(const QString &command, const boost::any &params) override;
    void onDataFrame(const DataSampleCSPtr &frame) override;
    void onInitProps(const QJsonObject &props) override;
    void onMavlinkMessage(const std::list<mavlink_message_t> &messages) override;

private slots:
    void onMenuClicked(QString value);

    void checkState();

    void on_VdWidget_customContextMenuRequested(const QPoint &pos);

private:
    void addMenuItem(QMenu &menu, QString name);
    void createVideoRender(QString render);
    void onMetaInfo(const SampleMetaInfo &metaInfo);
    void mavlinkMessage(const QMap<int, mavlink_message_t> &messages);
    QTimer m_timer;
    TimeOutTimer m_dataReceived;
    TimeOutTimer m_statReceived;
    signalStatus m_status;
    QList<GTUSharedPTR<BaseOSDVideoFilter>> m_filters;
    QMutex m_renderGuard;
    QtAV::Statistics statistics;
    QtAV::VideoFrame m_noSignal;
    QtAV::VideoFrame m_connecting;
    QtAV::VideoFrame m_noNetwork;
    Ui::VdWidget *ui = nullptr;
    QtAV::VideoRenderer *m_videoRenderer = nullptr;
    bool m_sliderPressed = false;
    bool m_isPaused = false;
    GTUSharedPTR<BaseOSDVideoFilter> m_filterOSD;
    GTUSharedPTR<BaseOSDVideoFilter> m_filterGMB;
    GTUSharedPTR<BaseOSDVideoFilter> m_filterRF;
    GTUSharedPTR<BaseOSDVideoFilter> m_filterICE;
    QString m_renderName;
};
