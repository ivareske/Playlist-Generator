#include <QApplication>
#include "PlaylistManager.h"


int main(int argc, char *argv[]){

    QApplication app(argc, argv);
    app.setApplicationVersion(QString::number(APP_VERSION));
    app.setApplicationName(QString("PlayListGenerator")); //fix me to use APP_NAME
    PlaylistManager plm;
    plm.show();
    return app.exec();
}
