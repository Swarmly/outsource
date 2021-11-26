#include "testar.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestAR w;
    w.show();
    return a.exec();
}
