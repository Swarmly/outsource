#pragma once

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class TestAR; }
QT_END_NAMESPACE

class TestAR : public QDialog
{
    Q_OBJECT

public:
    TestAR(QWidget *parent = nullptr);
    ~TestAR();

private:
    Ui::TestAR *ui;
};
