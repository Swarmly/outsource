#include "groundstation.h"
#include "hermes.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{

#ifdef HAVE_QT3D
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setSamples(4);
    format.setVersion(3, 0);
    QSurfaceFormat::setDefaultFormat(format);
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif


    QApplication a(argc, argv);
    GroundStation w;
    QJsonObject props;
    Hermes::getHMSensor()->Init(props);
    w.show();
    return a.exec();
}
