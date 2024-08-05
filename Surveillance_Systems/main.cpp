#include "mainwindow.h"

#include <QApplication>
#include <videoplayer.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    // videoPlayer p;
    // p.show();
    return a.exec();
}
