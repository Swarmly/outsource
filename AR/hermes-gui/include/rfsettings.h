#pragma once
#include "baseguiwidget.h"

namespace Ui {
class RFSettings;
}

class HERMESGUI_EXPORT RFSettings : public BaseGuiWidget
{
    Q_OBJECT

public:
    explicit RFSettings(QWidget *parent = nullptr);
    ~RFSettings();

private slots:
    void on_pushButtonSetChannel_clicked();

    void on_pushButtonSetGain_clicked();

private:
    Ui::RFSettings *ui;
};
