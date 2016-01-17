#include "RpiClock.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOverrideCursor( QCursor( Qt::BlankCursor ) );

    RpiClock mw;
    mw.resize(480, 320);
    mw.show();

    return app.exec();
}
