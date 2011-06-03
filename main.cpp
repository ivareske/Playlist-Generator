#include <QApplication>
#include "PlaylistManager.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationVersion(APP_VERSION);
    PlaylistManager plm;
    plm.show();
    return app.exec();
}
