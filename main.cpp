#include <QApplication>
#include "playlistManager.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    playlistManager plm;
    plm.show();
    return app.exec();
}
