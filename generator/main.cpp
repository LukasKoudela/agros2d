#include <QtGui/QApplication>

#include <QTranslator>
#include <QTextCodec>
#include <QDir>
#include <QString>

#include "util.h"
#include "generator.h"

int main(int argc, char *argv[])
{
    Agros2DGenerator a(argc, argv);

    QTimer::singleShot(0, &a, SLOT(run()));

    // MainWindow w;
    // w.show();

    return a.exec();
}