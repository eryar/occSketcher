#include "MDIWindow.h"

#include "View.h"
#include "Document.h"
#include "Application.h"

#include <qvbox.h>
#include <qtoolbar.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qapplication.h>

MDIWindow::MDIWindow( Document* aDocument, QWidget* parent, const char* name, int wflags )
: QMainWindow( parent, name, wflags )
{
    QVBox* vb = new QVBox( this );
    vb->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	setCentralWidget(vb);

	myDocument = aDocument;
	myView = new View( myDocument->getContext(), vb, 0, 0 );

    connect( myView, SIGNAL( selectionChanged() ),
             this,   SIGNAL( selectionChanged() ) );
	createViewActions();

    resize( sizeHint() );

    setFocusPolicy( StrongFocus );
}

MDIWindow::~MDIWindow()
{
}

Document* MDIWindow::getDocument()
{
	return myDocument;
}

void MDIWindow::closeEvent(QCloseEvent* e)
{
	emit sendCloseView(this);
}

void MDIWindow::fitAll()
{
	myView->fitAll();
}

void MDIWindow::createViewActions()
{
    // populate a tool bar with some actions
    QToolBar* aToolBar = new QToolBar( this, "view operations" );
    aToolBar->setLabel( tr( "View Operations" ) );
	
    QList<QAction>* aList = myView->getViewActions();

	for(QAction* a = aList->first();a;a = aList->next())
		a->addTo(aToolBar);
    aList->at(View::ViewHlrOffId)->setOn( true );
//***************************************===MAS
	QToolBar* attrToolBar = new QToolBar( this, "view operations" );
    attrToolBar->setLabel( tr( "View Operations" ) );
 	myView->onaddComboBox(attrToolBar);
}

void MDIWindow::onWindowActivated ()
{
    getDocument()->getApplication()->onSelectionChanged();
}

void MDIWindow::dump()
{
	static QString filter = "Images Files (*.bmp *.gif *.xwd)";
	QFileDialog fd ( 0, 0, true );     
	fd.setFilters( filter );
	fd.setCaption ( tr("INF_APP_IMPORT") );
	int ret = fd.exec(); 
    
	/* update the desktop after the dialog is closed */
	qApp->processEvents();
	QString file (ret == QDialog::Accepted ? fd.selectedFile() : QString::null);
    if ( !file.isNull() )
    {
	    QApplication::setOverrideCursor( Qt::waitCursor );
        if ( !QFileInfo( file ).extension().length() )
            file += QString( ".bmp" );
        bool res = myView->dump( (Standard_CString)file.latin1() );
        QApplication::restoreOverrideCursor();                
        if ( !res )
        {
	        QMessageBox::information ( qApp->mainWidget(),tr("TIT_ERROR"), tr("INF_ERROR"), tr("BTN_OK"),
		  		                       QString::null, QString::null, 0, 0 );
	        qApp->processEvents();
	    }
	}
}

QSize MDIWindow::sizeHint() const
{
    return QSize( 450, 300 );
}
