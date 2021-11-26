#include "baseguiwidget.h"
#include <QWidget>

namespace Ui {
class Gimbal;
}

class Gimbal : public BaseGuiWidget
{
    Q_OBJECT

public:
    explicit Gimbal(QWidget *parent = nullptr);
    ~Gimbal();

private:
    Ui::Gimbal *ui;
};
