#include "Application.h"

#include <qmainwindow.h>
#include <qapplication.h>
#include <qtranslator.h>
#include <qpixmap.h>

int main ( int argc, char* argv[] )
{
    QApplication a( argc, argv );

    QString resDir = ApplicationWindow::getResourceDir();

    QTranslator strTrans( 0 );
    strTrans.load( "string.qm", resDir );
    a.installTranslator( &strTrans );

    QTranslator iconTrans( 0 );
    iconTrans.load( "icon.qm", resDir );
    a.installTranslator( &iconTrans );

    QObject::connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

    ApplicationWindow* mw = new ApplicationWindow();
    mw->setCaption( QObject::tr( "TIT_SAMPLE" ) );
    mw->setIcon( QPixmap( resDir + QString( "/" ) + QObject::tr( "ICON_SAMPLE" ) ) );
    mw->show();

    return a.exec();
}
