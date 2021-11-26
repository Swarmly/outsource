#pragma once
#include "QtAV/Statistics.h"
#include "VideoRenderer.h"
#include "datasample.h"
#include "timeouttimer.h"
#include "video/videofilterbase.h"
#include <QDialog>
#include <QTimer>

extern "C" {
#include <libavutil/frame.h>
}


namespace Ui {
class GroundStation;
}

class GroundStation : public QDialog
{
    Q_OBJECT
public:
    explicit GroundStation(QWidget *parent = nullptr);
    ~GroundStation();
    void changeSize();

signals:
    void commandSend(const QString &command, const boost::any &params = nullptr);

public slots:
    void onDataFrame(const DataSampleCSPtr &frame);
    void closeEvent(QCloseEvent *event);
    //void mousePressEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void OnDebugDialog();
    QPointF getVideoWidgetSize();

private slots:
    void onTimer();
    void on_GroundStation_customContextMenuRequested(const QPoint &pos);
    void on_pushButtonRetract_clicked(bool checked);
    void on_pushButtonDayIr_clicked();
    void on_pushButtonZoomIN_pressed();
    void on_pushButtonZoomOUT_pressed();
    void on_pushButtonSnap_clicked();
    void on_pushButtonRec_clicked();
    void on_pushButtonZoomIN_released();
    void on_pushButtonZoomOUT_released();
    void on_pushButtonSendTarget_clicked();
    void on_pushButtonRec_clicked(bool checked);
    void on_pushButtonSyncVD_clicked();
    void on_pushButtonView_clicked();
    void on_pushRollDerot_clicked(bool checked);

private:
    QTimer m_timer;
    Ui::GroundStation *ui;
    bool m_zoomIN = false;
    bool m_zoomOUT = false;
    mavlink_nv_ext_ground_crossing_report_t m_ground_crossing_report;
};
