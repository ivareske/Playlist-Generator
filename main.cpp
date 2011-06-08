#include <QApplication>
#include "PlaylistManager.h"


/*!
 \brief

 \param argc
 \param argv[]
 \return int
*/
int main(int argc, char* argv[]) {

    QApplication app(argc, argv);
    app.setApplicationVersion(APP_VERSION);
    app.setApplicationName(APP_NAME);
    PlaylistManager plm;
    plm.show();
    return app.exec();
}
