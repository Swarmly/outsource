#include "groundstation.h"
#include "hermes.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GroundStation w;
    QJsonObject props;
    Hermes::getHMSensor()->Init(props);
    w.show();
    return a.exec();
}
