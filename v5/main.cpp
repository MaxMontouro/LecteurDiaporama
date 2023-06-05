#include "lecteurvue.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LecteurVue w;
    //w.changerDiaporama(2);
    w.show();
    return a.exec();
}
