#include "vdwidget.h"
#include "commoncommands.h"
//#include "configmanager.h"
//#include "gtuplugin-av/include/ffpackagedatasample.h"
#include "ui_vdwidget.h"
#include "vdwidget.h"
#include "video/videofiltergmb.h"
#include "video/videofilterice.h"
#include "video/videofilterosd.h"
#include "video/videofilterrfstat.h"
#include <QKeyEvent>
#include <QMenu>
#include <QMessageBox>
#include <QTimer>
#include <QtAVWidgets/global.h>

static QString showOSD("SHOWOSD");
static QString showRF("SHOWRF");
static QString showICE("SHOWICE");
static QString showGimbalOSD("SHOWGIMBALOSD");
static QString renderNameInCFG(QString("RENDER_NAME") + define2string(GIT_COMMIT));

int renderName2RenderId(QString renderName)
{
    if (renderName.isEmpty()) {
        qInfo() << "Render not specfied. Will use default: OpenGLWidget";
        return QtAV::VideoRendererId_OpenGLWidget;
    }
    if (renderName.compare("Widget", Qt::CaseInsensitive) == 0) {
        return QtAV::VideoRendererId_Widget;
    } else if (renderName.compare("GraphicsItem", Qt::CaseInsensitive) == 0) {
        return QtAV::VideoRendererId_GraphicsItem;
    } else if (renderName.compare("GL", Qt::CaseInsensitive) == 0) {
        return QtAV::VideoRendererId_GLWidget;
    } else if (renderName.compare("GDI", Qt::CaseInsensitive) == 0) {
        return QtAV::VideoRendererId_GDI;
    } else if (renderName.compare("DX2D", Qt::CaseInsensitive) == 0) {
        return QtAV::VideoRendererId_Direct2D;
    } else if (renderName.compare("XV", Qt::CaseInsensitive) == 0) {
        return QtAV::VideoRendererId_XV;
    } else if (renderName.compare("X11", Qt::CaseInsensitive) == 0) {
        return QtAV::VideoRendererId_X11;
    } else if (renderName.compare("GL2", Qt::CaseInsensitive) == 0) {
        return QtAV::VideoRendererId_GLWidget2;
    } else if (renderName.compare("OpenGL", Qt::CaseInsensitive) == 0) {
        return QtAV::VideoRendererId_OpenGLWidget;
    }
    qCritical() << "Unsupported render, supported only: Widget, GraphicsItem, GL, GDI, DX2D, XV, "
                   "X11, GL2, OpenGL.";
    return QtAV::VideoRendererId_OpenGLWidget;
}

VdWidget::VdWidget(QWidget *parent)
    : BaseGuiWidget(parent)
    , ui(new Ui::VdWidget)
{
    //this->setParent(parent);
    ui->setupUi(this);

    m_noSignal = QtAV::VideoFrame(QImage("://no_signal.jpg"));
    m_connecting = QtAV::VideoFrame(QImage("://loading.png"));
    m_noNetwork = QtAV::VideoFrame(QImage("://no_network.jpg"));

    m_filterOSD = GTUSharedPTR<BaseOSDVideoFilter>(new VideoFilterOSD(this));
    m_filterGMB = GTUSharedPTR<BaseOSDVideoFilter>(new GimbalStatOSDVideoFilter(this));
    m_filterRF = GTUSharedPTR<BaseOSDVideoFilter>(new VideoFilterRFStat(this));
    m_filterICE = GTUSharedPTR<BaseOSDVideoFilter>(new VideoFilterICE(this));

    addFilter(m_filterOSD);
    addFilter(m_filterGMB);
    addFilter(m_filterRF);
    addFilter(m_filterICE);
}

VdWidget::~VdWidget()
{
    m_filters.clear();
    //m_videoSamples.clear();
    m_filterOSD.reset();
    m_filterGMB.reset();
    m_filterRF.reset();
    m_filterICE.reset();
    if (m_videoRenderer) {
        ui->VdLayout->removeWidget(m_videoRenderer->widget());
        delete m_videoRenderer;
    }
    delete ui;
}

void VdWidget::onInitProps(const QJsonObject &props)
{
    QString renderName;
    renderName = GetJsonValue(props, "RENDER").toString(renderName);
    createVideoRender(renderName);

//    bool showICEBool = GTUCore::Impl().getConfigmanager()->GetValue(showICE, true).toBool();
//    bool showOSDBool = GTUCore::Impl().getConfigmanager()->GetValue(showOSD, true).toBool();
//    bool showRFBool = GTUCore::Impl().getConfigmanager()->GetValue(showRF, true).toBool();
//    bool showGimbalOSDBool
//        = GTUCore::Impl().getConfigmanager()->GetValue(showGimbalOSD, true).toBool();

    m_filterOSD->setEnabled(true);
    m_filterRF->setEnabled(true);
    m_filterGMB->setEnabled(true);
    m_filterICE->setEnabled(true);
    m_statReceived.SetNewTimeout(2500);
    m_dataReceived.SetNewTimeout(2500);
    m_statReceived.setElapsed();
    m_dataReceived.setElapsed();

    LOCAL_ASSERT(connect(&m_timer, &QTimer::timeout, this, &VdWidget::checkState));
    m_timer.start(100);
}

void VdWidget::createVideoRender(QString render)
{
    bool isError = false;
    QString errorRender;
    {
        QMutexLocker locker(&m_renderGuard);
        if (m_videoRenderer) {
            ui->VdLayout->removeWidget(m_videoRenderer->widget());
            delete m_videoRenderer;
        }
        LOG_INFO_EXT << "Ask to create render: " << render;

//        m_renderName
//            = GTUCore::Impl().getConfigmanager()->GetValue(renderNameInCFG, render).toString();
        LOG_INFO_EXT << "Will create render from the config: " << m_renderName;

        m_videoRenderer = QtAV::VideoRenderer::create(renderName2RenderId(m_renderName));
        if (!m_videoRenderer || !m_videoRenderer->widget()) {
            errorRender = m_renderName;
            m_renderName = "GL2";
            m_videoRenderer = QtAV::VideoRenderer::create(renderName2RenderId(m_renderName));
           // GTUCore::Impl().getConfigmanager()->PutValue(renderNameInCFG, m_renderName);
            isError = true;
        }

        ui->VdLayout->addWidget(m_videoRenderer->widget());

        installFilters(m_videoRenderer);

        m_videoRenderer->setStatistics(&statistics);
        m_videoRenderer->setOutAspectRatioMode(QtAV::VideoRenderer::RendererAspectRatio);
        m_videoRenderer->receive(m_noNetwork);
    }
    if (isError) {
        QMessageBox::critical(this,
                              tr("ERROR"),
                              "Can't create render:'" + errorRender + "' Will use:'" + m_renderName
                                  + "'",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
    }
}

void VdWidget::mavlinkMessage(const QMap<int, mavlink_message_t> &messages)
{
    for (auto &it : m_filters) {
        it->onMavlinkMessage(messages);
    }
}
void VdWidget::onMetaInfo(const SampleMetaInfo &metaInfo)
{
    for (auto &it : m_filters) {
        it->onMetaInfo(metaInfo);
    }
}

void VdWidget::checkState()
{
    if (m_dataReceived.IsElapsed()) {
        if (m_statReceived.IsElapsed()) {
            QMutexLocker locker(&m_renderGuard);
            m_videoRenderer->receive(m_noNetwork);
            m_status = signalStatus();
            m_filterRF->setRxStat(m_status);
        } else {
            if (0 == m_status.chipID) {
                QMutexLocker locker(&m_renderGuard);
                if (m_status.signalLocked) {
                    m_videoRenderer->receive(m_connecting);
                } else {
                    m_videoRenderer->receive(m_noSignal);
                }
            }
        }
    }
}
QPointF VdWidget::getVideoWidgetSize()
{
    QPointF size;
    size.setX(width());
    size.setY(height());
    return size;
}
void VdWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        float videoWidth = 1280;
        float videoHeight = 720;
        QPointF point;
        QPointF size(getVideoWidgetSize());
        point.setX(event->x() / size.x() * videoWidth);
        point.setY(event->y() / size.y() * videoHeight);
        commandSend(cmd_gimbal_TRACK, point);
    }
    BaseGuiWidget::mousePressEvent(event);
}

void VdWidget::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_1) && event->modifiers() == Qt::AltModifier) {
        m_filterOSD->setEnabled(!m_filterOSD->isEnabled());
       // GTUCore::Impl().getConfigmanager()->PutValue(showOSD, m_filterOSD->isEnabled());
    }
    if ((event->key() == Qt::Key_2) && event->modifiers() == Qt::AltModifier) {
        m_filterGMB->setEnabled(!m_filterGMB->isEnabled());
       // GTUCore::Impl().getConfigmanager()->PutValue(showGimbalOSD, m_filterGMB->isEnabled());
    }
    if ((event->key() == Qt::Key_3) && event->modifiers() == Qt::AltModifier) {
        m_filterRF->setEnabled(!m_filterRF->isEnabled());
       // GTUCore::Impl().getConfigmanager()->PutValue(showRF, m_filterRF->isEnabled());
    }
    if ((event->key() == Qt::Key_4) && event->modifiers() == Qt::AltModifier) {
        m_filterICE->setEnabled(!m_filterICE->isEnabled());
        //GTUCore::Impl().getConfigmanager()->PutValue(showICE, m_filterICE->isEnabled());
    }

    if (event->key() == Qt::Key_O) {
        m_filterOSD->setEnabled(false);
        m_filterGMB->setEnabled(false);
        m_filterRF->setEnabled(false);
        m_filterICE->setEnabled(false);
//        GTUCore::Impl().getConfigmanager()->PutValue(showOSD, m_filterOSD->isEnabled());
//        GTUCore::Impl().getConfigmanager()->PutValue(showGimbalOSD, m_filterGMB->isEnabled());
//        GTUCore::Impl().getConfigmanager()->PutValue(showRF, m_filterRF->isEnabled());
//        GTUCore::Impl().getConfigmanager()->PutValue(showICE, m_filterICE->isEnabled());
   }
    BaseGuiWidget::keyPressEvent(event);
}

void VdWidget::onCommand(const QString &command, const boost::any &params)
{
//    if (cmd_rx_stat == command) {
//        signalStatus status;
//        if (!get_prop_from_any(params, status)) {
//            LOG_ERROR_EXT << "Can't extract parameters for command: " << command;
//        } else {
//            m_filterRF->setRxStat(status);
//        }
//    }
}

void VdWidget::addFilter(GTUSharedPTR<BaseOSDVideoFilter> filter)
{
    m_filters.append(filter);
}

void VdWidget::installFilters(QtAV::VideoRenderer *renderer)
{
    for (auto &it : m_filters) {
        it->installTo(renderer);
    }
}

void VdWidget::onMavlinkMessage(const std::list<mavlink_message_t> &outMessages)
{
    QMap<int, mavlink_message_t> messages;
    for (const auto &it : outMessages) {
        messages[it.msgid] = it;
    }
    mavlinkMessage(messages);
}

void VdWidget::onDataFrame(const DataSampleCSPtr &data)
{
//    if (CompareString(data->format(), "RGB") || CompareString(data->format(), "RGB24")
//        || CompareString(data->format(), "BGR") || CompareString(data->format(), "BGR24")
//        || CompareString(data->format(), "GRAY") || CompareString(data->format(), "GRAY8")
//        || CompareString(data->format(), "GRAY16") || CompareString(data->format(), "GRAY16LE")
//        || CompareString(data->format(), "GRAY16BE") || CompareString(data->format(), "yuv420p")
//        || CompareString(data->format(), "yuvj420p")) {
//        QSharedPointer<FFPackageDataSample const> dataPtr
//            = data.dynamicCast<const FFPackageDataSample>();
//        // m_videoRenderer;

//        if (dataPtr && dataPtr->GetFrame()) {
//            QSharedPointer<AVFrame> frame = dataPtr->GetFrame();

//            using namespace QtAV;
//            QSharedPointer<QtAV::VideoFrame> avFrame(
//                new QtAV::VideoFrame(frame->width,
//                                     frame->height,
//                                     QtAV::VideoFormat((int) frame->format)));

//            avFrame->setBits(frame->data);
//            avFrame->setBytesPerLine(frame->linesize);
//            avFrame->setTimestamp(-1); // in s. what about AVFrame.pts?
//            QMutexLocker locker(&m_renderGuard);
//            m_videoRenderer->receive(*avFrame.get());
//            m_dataReceived.RestartTimer();
//        }
//    } else if (data->format() == typeMAV2) {
//        onMetaInfo(data->metaInfo());
//    }
}

void VdWidget::onMenuClicked(QString value)
{
    createVideoRender(value);
    //GTUCore::Impl().getConfigmanager()->PutValue(renderNameInCFG, value);
}

void VdWidget::addMenuItem(QMenu &menu, QString name)
{
    if (name == m_renderName) {
        menu.addAction(QIcon("://selected2.ico"), "Render: " + name, this, [=]() {
            this->onMenuClicked(name);
        });
    } else {
        menu.addAction("Render: " + name, this, [=]() { this->onMenuClicked(name); });
    }
}

void VdWidget::on_VdWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    addMenuItem(menu, "Widget");
    addMenuItem(menu, "GraphicsItem");
    addMenuItem(menu, "GL");
    addMenuItem(menu, "GDI");
    addMenuItem(menu, "DX2D");
    addMenuItem(menu, "XV");
    addMenuItem(menu, "X11");
    addMenuItem(menu, "GL2");
    addMenuItem(menu, "OpenGL");
    setCursor(Qt::ArrowCursor);
    menu.exec(this->mapToGlobal(pos));
}
