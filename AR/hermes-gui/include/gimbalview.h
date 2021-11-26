#pragma once
#include "baseguiwidget.h"

namespace Ui {
class GimbalView;
}

class HERMESGUI_EXPORT GimbalView : public BaseGuiWidget
{
    Q_OBJECT

public:
    explicit GimbalView(QWidget *parent = nullptr);
    ~GimbalView();

private slots:
    void on_pushButtonEpr_clicked();

    void on_pushButtonGrr_clicked();

    void on_pushButtonObs_clicked();

    void on_pushButtonStow_clicked();

    void on_pushButtonPilot_clicked();

    void on_pushButtonHold_clicked();

    void on_pushButtonPV1_clicked();

    void on_pushButtonPV2_clicked();

    void on_pushButtonGPOS_clicked();

    void on_pushButtonLPOS_clicked();

private:
    Ui::GimbalView *ui;
};
