#pragma once
#include "baseguiwidget.h"

namespace Ui {
class setMavUavMode;
}

class HERMESGUI_EXPORT setMavUavMode : public BaseGuiWidget
{
    Q_OBJECT

public:
    explicit setMavUavMode(QWidget *parent = nullptr);
    ~setMavUavMode();

private slots:
    void on_pushButtonAuto_clicked();

    void on_pushButton433_clicked();

    void on_pushButton866_clicked();

    void on_pushButtonMixed_clicked();

private:
    Ui::setMavUavMode *ui;
};
