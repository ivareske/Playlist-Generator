#include <QApplication>
#include "PlaylistManager.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    PlaylistManager plm;
    plm.show();
    return app.exec();
}
