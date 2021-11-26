#pragma once
#include "baseguiwidget.h"
#include <QWidget>

namespace Ui {
class IRMode;
}

class HERMESGUI_EXPORT IRMode : public BaseGuiWidget
{
    Q_OBJECT

public:
    explicit IRMode(QWidget *parent = nullptr);
    ~IRMode();

private slots:
    void on_pushButtonDay_clicked();
    void on_pushButtonDayIR_clicked();
    void on_pushButtonNUC_clicked();
    void on_pushButtonWhColorPalette_clicked();
    void on_pushButtonWhBWPalette_clicked();
    void on_pushButtonBH_clicked();
    void on_pushButtonWH_clicked();

private:
    Ui::IRMode *ui;
};
