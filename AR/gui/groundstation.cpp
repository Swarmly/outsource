#include "groundstation.h"
#include "commoncommands.h"
//#include "configmanager.h"
#include "coreutils.h"
//#include "debugwindow.h"
#include "gimbalview.h"
#include "global.h"
#include "hermes.h"
#include "irmode.h"
#include "ui_groundstation.h"

#include <restorevideo.h>
#include <QKeyEvent>
#include <QMenu>
#include <QPainter>
#include <QPair>
#include <QSignalMapper>
#include <QThread>
#include <QTimer>

extern bool renderType;

QString GetHASH()
{
    return define2string(GIT_COMMIT);
}

GroundStation::GroundStation(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GroundStation)
{
    //bool hideQuick = GetJsonValue(props, "HIDE_QUICK").toBool(false);

    ui->setupUi(this);
    this->setWindowTitle(this->windowTitle() + " [" + define2string(APP_VERSION) + "_" + GetHASH()
                         + "]");
    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowCloseButtonHint;
    flags |= Qt::WindowMinMaxButtonsHint;
    setWindowFlags(flags);

    LOCAL_ASSERT(QThread::connect(&m_timer, &QTimer::timeout, this, &GroundStation::onTimer));
    m_timer.setInterval(500);
    m_timer.start();

    //    if (hideQuick) {
    //        ui->widget->setHidden(hideQuick);
    //    }

    memset(&m_ground_crossing_report, 0, sizeof(m_ground_crossing_report));
    LOCAL_ASSERT(
                connect(this, &GroundStation::commandSend, Hermes::getImpl(), &Hermes::commandReSend));
    LOCAL_ASSERT(connect(Hermes::getImpl(), &Hermes::dataFrame, this, &GroundStation::onDataFrame));
}

GroundStation::~GroundStation()
{
    delete ui;
}

QPointF GroundStation::getVideoWidgetSize()
{
    QPointF size;
    size.setX(ui->videoWidget->width());
    size.setY(ui->videoWidget->height());
    return size;
}

void GroundStation::closeEvent(QCloseEvent * /*event*/)
{
    //GTUCore::Impl().getPluginManager()->StopAllSensors();
}

void GroundStation::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
            && event->modifiers() == Qt::AltModifier) {
        changeSize();
    }

    //    if ((event->key() == Qt::Key_4) && event->modifiers() == Qt::AltModifier) {
    //        m_ice->setEnabled(!m_ice->isEnabled());
    //        GTUCore::Impl().getConfigmanager()->PutValue(showICE, m_ice->isEnabled());
    //    }
    if ((event->key() == Qt::Key_P) && event->modifiers() == Qt::AltModifier) {
        ui->widget->setHidden(!ui->widget->isHidden());
    }
}

void GroundStation::changeSize()
{
    if (this->isFullScreen()) {
        this->showNormal();
    } else {
        this->showFullScreen();
    }
}

QImage convertToGray(QImage *pInputImage)
{
    if (pInputImage && !pInputImage->isNull()) {
        QImage retImg(pInputImage->width(), pInputImage->height(), QImage::Format_Indexed8);
        QVector<QRgb> table(256);
        for (int i = 0; i < 256; ++i) {
            table[i] = qRgb(i, i, i);
        }

        retImg.setColorTable(table);

        for (int i = 0; i < pInputImage->width(); i++) {
            for (int j = 0; j < pInputImage->height(); j++) {
                QRgb value = pInputImage->pixel(i, j);
                retImg.setPixel(i, j, qGray(value));
            }
        }
        return retImg;
    }
    return QImage();
}

void GroundStation::onTimer()
{
    if (m_zoomIN) {
        emit commandSend(cmd_gimbal_ZOOM_IN);
    }
    if (m_zoomOUT) {
        emit commandSend(cmd_gimbal_ZOOM_OUT);
    }
}

void GroundStation::OnDebugDialog()
{
    //DebugWindow::instance()->exec();
}

void GroundStation::on_pushButtonRetract_clicked(bool checked)
{
    emit commandSend(cmd_gimbal_RETRACT, checked);
}

void GroundStation::on_GroundStation_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction("&Debug window", this, SLOT(OnDebugDialog()));
    setCursor(Qt::ArrowCursor);
    menu.exec(this->mapToGlobal(pos));
}

void GroundStation::on_pushButtonDayIr_clicked()
{
    IRMode dialog(ui->pushButtonDayIr);
    LOCAL_ASSERT(connect(&dialog, &IRMode::commandSend, Hermes::getImpl(), &Hermes::commandReSend));
    dialog.exec();
}

void GroundStation::on_pushButtonZoomIN_pressed()
{
    m_zoomIN = true;
}

void GroundStation::on_pushButtonZoomOUT_pressed()
{
    m_zoomOUT = true;
}

void GroundStation::on_pushButtonSnap_clicked()
{
    emit commandSend(cmd_gimbal_IMG_CAPTURE);
}

void GroundStation::on_pushButtonRec_clicked()
{
    emit commandSend(cmd_gimbal_RECORD);
}

void GroundStation::on_pushButtonZoomIN_released()
{
    m_zoomIN = false;
}

void GroundStation::on_pushButtonZoomOUT_released()
{
    m_zoomOUT = false;
}

void GroundStation::onDataFrame(const DataSampleCSPtr &frame)
{
    if (frame->format() == typeMAV2) {
        if (frame->metaInfo().hasProp("mavlink_nv_ext_ground_crossing_report_t")) {
            m_ground_crossing_report = frame->metaInfo()
                    .getProp("mavlink_nv_ext_ground_crossing_report_t",
                             m_ground_crossing_report);
        }
    }
}

void GroundStation::on_pushButtonSendTarget_clicked()
{
    QList<double> targetParams;
    targetParams.push_back(m_ground_crossing_report.gnd_crossing_lat);
    targetParams.push_back(m_ground_crossing_report.gnd_crossing_lon);
    targetParams.push_back(m_ground_crossing_report.gnd_crossing_alt);
    emit commandSend(cmd_bombing_SEND_TARGET, targetParams);
}

void GroundStation::on_pushButtonRec_clicked(bool checked)
{
    emit commandSend(cmd_gimbal_RECORD, checked);
}

void GroundStation::on_pushButtonSyncVD_clicked()
{
    RestoreVideo rstDlg(ui->pushButtonSyncVD);
    LOCAL_ASSERT(
                connect(&rstDlg, &RestoreVideo::commandSend, Hermes::getImpl(), &Hermes::commandReSend));
    rstDlg.exec();
}

void GroundStation::on_pushButtonView_clicked()
{
    GimbalView dialog(ui->pushButtonView);
    LOCAL_ASSERT(
                connect(&dialog, &GimbalView::commandSend, Hermes::getImpl(), &Hermes::commandReSend));
    dialog.exec();
}

void GroundStation::on_pushRollDerot_clicked(bool checked)
{
    emit commandSend(cmd_gimbal_ROLL_DEROT, checked);
}

void GroundStation::on_pushButtonRender_clicked(bool checked)
{
    renderType = checked;
}

