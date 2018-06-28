#include "Application.h"

#include "Document.h"
#include "Translate.h"
#include "View.h"

#include <qvbox.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qworkspace.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qsignalmapper.h>

#include <stdlib.h>

static ApplicationWindow* stApp;
static QWorkspace* stWs;

ApplicationWindow::ApplicationWindow()
    : QMainWindow( 0, "Sample ImportExport main window" ),
myImportPopup( 0 ),
myExportPopup( 0 ),
myWindowPopup( 0 ),
myFilePopup( 0 ),
myCasCadeBar( 0 ),
myStdToolBar( 0 )
{
    myNbDocuments = 0;
    stApp = this;
	myIsDocuments = false;

    // create and define the central widget
    QVBox* vb = new QVBox( this );
    vb->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    stWs = new QWorkspace( vb );
    setCentralWidget( vb );

	createStandardOperations();
	createCasCadeOperations();

    statusBar()->message( tr("INF_READY"), 5000 );
    resize( 1000, 700 );
}

ApplicationWindow::~ApplicationWindow()
{
}

void ApplicationWindow::createStandardOperations()
{
    QPixmap newIcon, helpIcon, closeIcon;

	QString dir = getResourceDir() + QString( "/" );

    newIcon = QPixmap( dir+tr("ICON_NEW") );
	helpIcon = QPixmap( dir+tr("ICON_HELP") );
	closeIcon = QPixmap( dir+tr("ICON_CLOSE") );

	QAction * fileNewAction, * fileCloseAction, * fileQuitAction,
			* viewToolAction, * viewStatusAction,
			* helpAboutAction;

	fileNewAction = new QAction( tr("TBR_NEW"), newIcon, tr("MNU_NEW"), CTRL+Key_N, this, "new" );
	connect( fileNewAction, SIGNAL( activated() ) , this, SLOT( onNewDoc() ) );
	myStdActions.insert(FileNewId, fileNewAction);

    fileCloseAction = new QAction( tr("TBR_CLOSE"), closeIcon, tr("MNU_CLOSE"), CTRL+Key_W, this, "close");
    connect( fileCloseAction, SIGNAL( activated() ) , this, SLOT( onCloseWindow() ) );
	myStdActions.insert(FileCloseId, fileCloseAction);

    fileQuitAction = new QAction( tr("TBR_QUIT"), tr("MNU_QUIT"), CTRL+Key_Q, this, "quit" );
    connect( fileQuitAction, SIGNAL( activated() ) , qApp, SLOT( closeAllWindows() ) );
	myStdActions.insert(FileQuitId, fileQuitAction);

	viewToolAction = new QAction( tr("TBR_TOOL_BAR"), tr("MNU_TOOL_BAR"), 0, this, "toolbar");
	connect( viewToolAction, SIGNAL( activated() ) , this, SLOT( onViewToolBar() ));
	viewToolAction->setToggleAction(true);
	viewToolAction->setOn( true );
	myStdActions.insert(ViewToolId, viewToolAction);

	viewStatusAction = new QAction( tr("TBR_STATUS_BAR"), tr("MNU_STATUS_BAR"), 0, this, "statusbar");
	connect( viewStatusAction, SIGNAL( activated() ), this, SLOT( onViewStatusBar() ));
	viewStatusAction->setToggleAction(true);
	viewStatusAction->setOn( true );
	myStdActions.insert(ViewStatusId, viewStatusAction);

	helpAboutAction = new QAction( tr("TBR_ABOUT"), helpIcon, tr("MNU_ABOUT"), Key_F1, this, "about" );
	connect( helpAboutAction, SIGNAL( activated() ) , this, SLOT( onAbout() ) );
	myStdActions.insert(HelpAboutId, helpAboutAction);

    // populate a tool bar with some actions

    myStdToolBar = new QToolBar( this, "file operations" );
    myStdToolBar->setLabel( tr( "File Operations" ) );
    fileNewAction->addTo( myStdToolBar );
	helpAboutAction->addTo( myStdToolBar );

    // popuplate a menu with all actions

    myFilePopup = new QPopupMenu( this );
    menuBar()->insertItem( tr("MNU_FILE"), myFilePopup );
    fileNewAction->addTo( myFilePopup );
    fileCloseAction->addTo( myFilePopup );
    myFilePopup->insertSeparator();
    fileQuitAction->addTo( myFilePopup );

	// add a view menu

	QPopupMenu * view = new QPopupMenu( this );
	menuBar()->insertItem( tr("MNU_VIEW"), view);
	viewToolAction->addTo(view);
	viewStatusAction->addTo(view);

	// add a help menu

    QPopupMenu * help = new QPopupMenu( this );
    menuBar()->insertSeparator();
    menuBar()->insertItem( tr("MNU_HELP"), help );
	helpAboutAction->addTo( help );

  myStdActions.at(FileCloseId)->setEnabled(myDocuments.count() > 0);
}

void ApplicationWindow::createCasCadeOperations()
{
	createTranslatePopups();
	createWindowPopup();

    // populate a tool bar with some actions
    myCasCadeBar = new QToolBar( this, "shape operations" );
    myCasCadeBar->setLabel( tr( "Shape Operations" ) );

    QString dir = ApplicationWindow::getResourceDir() + QString( "/" );
    QAction* a;

    a = new QAction( tr("TBR_TOOL_WIRE"),QPixmap( dir+tr("ICON_TOOL_WIRE") ), tr("MNU_TOOL_WIRE"), 0, this, "wireframe" );
    connect( a, SIGNAL( activated() ) , this, SLOT( onToolAction() ) );
    myToolActions.insert(ToolWireframeId, a);

    a = new QAction( tr("TBR_TOOL_SHAD"),QPixmap( dir+tr("ICON_TOOL_SHAD") ), tr("MNU_TOOL_SHAD"), 0, this, "shading" );
    connect( a, SIGNAL( activated() ) , this, SLOT( onToolAction() ) );
    myToolActions.insert(ToolShadingId, a);

    a = new QAction( tr("TBR_TOOL_COLOR"),QPixmap( dir+tr("ICON_TOOL_COLOR") ), tr("MNU_TOOL_COLOR"), 0, this, "color" );
    connect( a, SIGNAL( activated() ) , this, SLOT( onToolAction() ) );
    myToolActions.insert(ToolColorId, a);

    a = new QAction( tr("TBR_TOOL_MATER"),QPixmap( dir+tr("ICON_TOOL_MATER") ), tr("MNU_TOOL_MATER"), 0, this, "material" );
    connect( a, SIGNAL( activated() ) , this, SLOT( onToolAction() ) );
    myToolActions.insert(ToolMaterialId, a);

    a = new QAction( tr("TBR_TOOL_TRANS"),QPixmap( dir+tr("ICON_TOOL_TRANS") ), tr("MNU_TOOL_TRANS"), 0, this, "transparency" );
    connect( a, SIGNAL( activated() ) , this, SLOT( onToolAction() ) );
    myToolActions.insert(ToolTransparencyId, a);

    a = new QAction( tr("TBR_TOOL_DEL"),QPixmap( dir+tr("ICON_TOOL_DEL") ), tr("MNU_TOOL_DEL"), 0, this, "delete" );
    connect( a, SIGNAL( activated() ) , this, SLOT( onToolAction() ) );
    myToolActions.insert(ToolDeleteId, a);

    QSignalMapper* sm = new QSignalMapper( this );
    connect( sm, SIGNAL( mapped( int ) ), this, SLOT( onSetMaterial( int ) ) );

    a = new QAction( tr("TBR_BRASS"), tr("MNU_BRASS"), 0, this, "brass" );
    sm->setMapping(a,(int) Graphic3d_NOM_BRASS);
    connect(a,SIGNAL(activated()),sm,SLOT(map()));
    myMaterialActions.insert(Graphic3d_NOM_BRASS,a);

    a = new QAction( tr("TBR_BRONZE"), tr("MNU_BRONZE"), 0, this, "bronze" );
    sm->setMapping(a,(int) Graphic3d_NOM_BRONZE);
    connect(a,SIGNAL(activated()),sm,SLOT(map()));
    myMaterialActions.insert(Graphic3d_NOM_BRONZE,a);

    a = new QAction( tr("TBR_COPPER"), tr("MNU_COPPER"), 0, this, "copper" );
    sm->setMapping(a,(int) Graphic3d_NOM_COPPER);
    connect(a,SIGNAL(activated()),sm,SLOT(map()));
    myMaterialActions.insert(Graphic3d_NOM_COPPER,a);

    a = new QAction( tr("TBR_GOLD"), tr("MNU_GOLD"), 0, this, "gold" );
    sm->setMapping(a,(int) Graphic3d_NOM_GOLD);
    connect(a,SIGNAL(activated()),sm,SLOT(map()));
    myMaterialActions.insert(Graphic3d_NOM_GOLD,a);

    a = new QAction( tr("TBR_PEWTER"), tr("MNU_PEWTER"), 0, this, "pewter" );
    sm->setMapping(a,(int) Graphic3d_NOM_PEWTER);
    connect(a,SIGNAL(activated()),sm,SLOT(map()));
    myMaterialActions.insert(Graphic3d_NOM_PEWTER,a);

    a = new QAction( tr("TBR_PLASTER"), tr("MNU_PLASTER"), 0, this, "plaster" );
    sm->setMapping(a,(int) Graphic3d_NOM_PLASTER);
    connect(a,SIGNAL(activated()),sm,SLOT(map()));
    myMaterialActions.insert(Graphic3d_NOM_PLASTER,a);

    a = new QAction( tr("TBR_PLASTIC"), tr("MNU_PLASTIC"), 0, this, "plastic" );
    sm->setMapping(a,(int) Graphic3d_NOM_PLASTIC);
    connect(a,SIGNAL(activated()),sm,SLOT(map()));
    myMaterialActions.insert(Graphic3d_NOM_PLASTIC,a);

    a = new QAction( tr("TBR_SILVER"), tr("MNU_SILVER"), 0, this, "silver" );
    sm->setMapping(a,(int) Graphic3d_NOM_SILVER);
    connect(a,SIGNAL(activated()),sm,SLOT(map()));
    myMaterialActions.insert(Graphic3d_NOM_SILVER,a);

    for ( QAction* anAction = myToolActions.first(); anAction; anAction = myToolActions.next() )
		anAction->addTo(myCasCadeBar);
	myCasCadeBar->hide();
}

QList<QAction>* ApplicationWindow::getToolActions()
{
    return &myToolActions;
}

QList<QAction>* ApplicationWindow::getMaterialActions()
{
    return &myMaterialActions;
}

void ApplicationWindow::createTranslatePopups()
{
    if ( !myImportPopup )
        myImportPopup = new QPopupMenu( this );

    if ( !myExportPopup )
	    myExportPopup = new QPopupMenu( this );

	QAction* a;

	a = new QAction( tr( "TBR_IMPORT_BREP" ), tr( "MNU_IMPORT_BREP" ), 0, this, "import brep" );
	connect( a, SIGNAL( activated() ), this, SLOT( onImport() ) );
	myCasCadeTranslateActions.insert( FileImportBREPId, a );
	a->addTo( myImportPopup );

	a = new QAction( tr( "TBR_EXPORT_BREP" ), tr( "MNU_EXPORT_BREP" ), 0, this, "export brep" );
	connect( a, SIGNAL( activated() ), this, SLOT( onExport() ) );
	myCasCadeTranslateActions.insert( FileExportBREPId, a );
	a->addTo( myExportPopup );

	a = new QAction( tr( "TBR_IMPORT_CSFDB" ), tr( "MNU_IMPORT_CSFDB" ), 0, this, "import csfdb" );
	connect( a, SIGNAL( activated() ), this, SLOT( onImport() ) );
	myCasCadeTranslateActions.insert( FileImportCSFDBId, a );
	a->addTo( myImportPopup );

	a = new QAction( tr( "TBR_EXPORT_CSFDB" ), tr( "MNU_EXPORT_CSFDB" ), 0, this, "export csfdb" );
	connect( a, SIGNAL( activated() ), this, SLOT( onExport() ) );
	myCasCadeTranslateActions.insert( FileExportCSFDBId, a );
	a->addTo( myExportPopup );

	a = new QAction( tr( "TBR_IMPORT_IGES" ), tr( "MNU_IMPORT_IGES" ), 0, this, "import iges" );
	connect( a, SIGNAL( activated() ), this, SLOT( onImport() ) );
	myCasCadeTranslateActions.insert( FileImportIGESId, a );
	a->addTo( myImportPopup );

	a = new QAction( tr( "TBR_EXPORT_IGES" ), tr( "MNU_EXPORT_IGES" ), 0, this, "export iges" );
	connect( a, SIGNAL( activated() ), this, SLOT( onExport() ) );
	myCasCadeTranslateActions.insert( FileExportIGESId, a );
	a->addTo( myExportPopup );

	a = new QAction( tr( "TBR_IMPORT_STEP" ), tr( "MNU_IMPORT_STEP" ), 0, this, "import step" );
	connect( a, SIGNAL( activated() ), this, SLOT( onImport() ) );
	myCasCadeTranslateActions.insert( FileImportSTEPId, a );
	a->addTo( myImportPopup );

	a = new QAction( tr( "TBR_EXPORT_STEP" ), tr( "MNU_EXPORT_STEP" ), 0, this, "export step" );
	connect( a, SIGNAL( activated() ), this, SLOT( onExport() ) );
	myCasCadeTranslateActions.insert( FileExportSTEPId, a );
	a->addTo( myExportPopup );

	a = new QAction( tr( "TBR_EXPORT_STL" ), tr( "MNU_EXPORT_STL" ), 0, this, "export stl" );
	connect( a, SIGNAL( activated() ), this, SLOT( onExport() ) );
	myCasCadeTranslateActions.insert( FileExportSTLId, a );
	a->addTo( myExportPopup );

	a = new QAction( tr( "TBR_EXPORT_VRML" ), tr( "MNU_EXPORT_VRML" ), 0, this, "export vrml" );
	connect( a, SIGNAL( activated() ), this, SLOT( onExport() ) );
	myCasCadeTranslateActions.insert( FileExportVRMLId, a );
	a->addTo( myExportPopup );

	myExportPopup->insertSeparator();

	a = new QAction( tr( "TBR_EXPORT_IMAGE" ), tr( "MNU_EXPORT_IMAGE" ), 0, this, "export image" );
	connect( a, SIGNAL( activated() ), this, SLOT( onExportImage() ) );
	a->addTo( myExportPopup );
}

void ApplicationWindow::createWindowPopup()
{
    if ( !myWindowPopup )
    {
	    myWindowPopup = new QPopupMenu( this );
	    connect( myWindowPopup, SIGNAL( aboutToShow() ),
		         this, SLOT( windowsMenuAboutToShow() ) );
    }
}

void ApplicationWindow::windowsMenuAboutToShow()
{
    myWindowPopup->clear();
    QAction* a;

    QString dir = getResourceDir() + QString( "/" );

    a = new QAction( tr( "TBR_WINDOW_NEW3D" ), QPixmap( dir + tr( "ICON_WINDOW_NEW3D" ) ), tr( "MNU_WINDOW_NEW3D" ), 0, this, "new3d" );
    connect( a, SIGNAL( activated() ), this, SLOT( onCreateNewView() ) );
    a->addTo( myWindowPopup );

    a = new QAction( tr( "TBR_WINDOW_CASCADE" ), QPixmap( dir + tr( "ICON_WINDOW_CASCADE" ) ), tr( "MNU_WINDOW_CASCADE" ), 0, this, "cascade" );
    connect( a, SIGNAL( activated() ), stWs, SLOT( cascade() ) );
    a->addTo( myWindowPopup );

    a = new QAction( tr( "TBR_WINDOW_TILE" ),QPixmap( dir + tr( "ICON_WINDOW_TILE" ) ), tr( "MNU_WINDOW_TILE" ), 0, this, "tile" );
    connect( a, SIGNAL( activated() ), stWs, SLOT( tile() ) );
    a->addTo( myWindowPopup );

    myWindowPopup->insertSeparator();
    QWidgetList windows = stWs->windowList();
    for ( uint i = 0; i < windows.count(); ++i )
    {
        int id = myWindowPopup->insertItem( windows.at(i)->caption(),
				                            this, SLOT( windowsMenuActivated( int ) ) );
        myWindowPopup->setItemParameter( id, i );
        myWindowPopup->setItemChecked( id, stWs->activeWindow() == windows.at(i) );
    }
}

void ApplicationWindow::windowsMenuActivated( int id )
{
    QWidget* w = stWs->windowList().at( id );
    if ( w )
	    w->setFocus();
}

QWorkspace * ApplicationWindow::getWorkspace()
{
	return stWs;
}

ApplicationWindow* ApplicationWindow::getApplication()
{
	return stApp;
}

void ApplicationWindow::updateFileActions()
{
	int count = myFilePopup->count() - 1;
	int popupCount = menuBar()->count();

	if ( stWs->windowList().isEmpty() )
    {
	    if ( !myIsDocuments )
		{
			myFilePopup->insertItem( tr( "MNU_FILE_IMPORT" ), myImportPopup, -1, count++ );
			myFilePopup->insertItem( tr( "MNU_FILE_EXPORT" ), myExportPopup, -1, count++ );
			myFilePopup->insertSeparator( count++ );
			myIsDocuments = true;
			myCasCadeBar->show();
			menuBar()->insertItem( tr( "MNU_WINDOW" ), myWindowPopup, 0, popupCount - 1 );
		}
        else
        {
			myFilePopup->removeItemAt( --count );
			myFilePopup->removeItemAt( --count );
			myFilePopup->removeItemAt( --count );
			myIsDocuments = false;
			myCasCadeBar->hide();
			menuBar()->removeItemAt( popupCount - 2 );
	    }
    }
}

Document* ApplicationWindow::onNewDoc()
{
    updateFileActions();
    Document* aDoc = new Document( ++myNbDocuments, this );
    connect( aDoc, SIGNAL( sendCloseDocument( Document* ) ),
             this, SLOT( onCloseDocument( Document* ) ) );
    connect( stWs, SIGNAL( windowActivated( QWidget* ) ),
             this, SLOT( onWindowActivated( QWidget* ) ) );
    connect( aDoc, SIGNAL( selectionChanged() ),
             this, SLOT( onSelectionChanged() ) );
    myDocuments.append( aDoc );
    myStdActions.at( FileCloseId )->setEnabled( myDocuments.count() > 0 );
    return aDoc;
}

void ApplicationWindow::onCloseWindow(){
    MDIWindow* m = (MDIWindow*)stWs->activeWindow();
    if ( m )
    {
        Document* doc = m->getDocument();
        onCloseDocument( doc );
    }
}

void ApplicationWindow::onCloseDocument(Document* theDoc)
{
    myDocuments.removeRef( theDoc );
    theDoc->removeViews();
	delete theDoc;
	updateFileActions();
    myStdActions.at(FileCloseId)->setEnabled(myDocuments.count() > 0);
}

void ApplicationWindow::onViewToolBar()
{
    bool show = myStdActions.at( ViewToolId )->isOn();
    if (  show == myStdToolBar->isVisible() )
        return;
    if ( show )
        myStdToolBar->show();
    else
        myStdToolBar->hide();
}

void ApplicationWindow::onViewStatusBar()
{
    bool show = myStdActions.at( ViewStatusId )->isOn();
    if (  show == statusBar()->isVisible() )
        return;
    if ( show )
        statusBar()->show();
    else
        statusBar()->hide();
}

void ApplicationWindow::onAbout()
{
    QMessageBox::information( this, tr( "TIT_ABOUT" ), tr( "INF_ABOUT" ), tr("BTN_OK" ),
		  		              QString::null, QString::null, 0, 0 );
}


void ApplicationWindow::onImport()
{
    QAction* a = (QAction*)sender();
    int type = translationFormat( a );
    if ( type < 0 )
        return;

    bool stat = translate( type, true );
    if ( stat )
    {
        Document* doc = ((MDIWindow*) stWs->activeWindow())->getDocument();
        doc->fitAll();
    }
}

void ApplicationWindow::onExport()
{
    QAction* a = (QAction*)sender();
    int type = translationFormat( a );
    if ( type < 0 )
        return;

    bool stat = translate( type, false );
}

int ApplicationWindow::translationFormat( const QAction* a )
{
    int type = -1;
    for ( int i = FileImportBREPId; i <= FileExportVRMLId; i++ )
    {
        if ( myCasCadeTranslateActions.at( i ) == a )
        {
            type = i;
            break;
        }
    }
    switch ( type )
    {
    case FileImportBREPId:
    case FileExportBREPId:
        type = Translate::FormatBREP;
        break;
    case FileImportIGESId:
    case FileExportIGESId:
        type = Translate::FormatIGES;
        break;
    case FileImportSTEPId:
    case FileExportSTEPId:
        type =  Translate::FormatSTEP;
        break;
    case FileImportCSFDBId:
    case FileExportCSFDBId:
        type = Translate::FormatCSFDB;
        break;
    case FileExportSTLId:
        type = Translate::FormatSTL;
        break;
    case FileExportVRMLId:
        type = Translate::FormatVRML;
        break;
    }
    return type;
}

bool ApplicationWindow::translate( const int format, const bool import )
{
    static Translate* anTrans = new Translate( this, "Translator" );
    Document* doc = ((MDIWindow*) stWs->activeWindow())->getDocument();
    Handle(AIS_InteractiveContext) context = doc->getContext();
    bool status;
    if ( import )
        status = anTrans->importModel( format, context );
    else
        status = anTrans->exportModel( format, context );

    if ( !status )
    {
        QString msg = tr( "INF_TRANSLATE_ERROR" );
        if ( !anTrans->info().isEmpty() )
            msg += QString( "\n" ) + anTrans->info();
        QMessageBox::critical( this, tr( "TIT_ERROR" ), msg, tr( "BTN_OK" ), QString::null, QString::null, 0, 0 );
    }
    
    return status;
}

void ApplicationWindow::onCreateNewView()
{
    Document* doc = ((MDIWindow*) stWs->activeWindow())->getDocument();
    doc->onCreateNewView();
}

void ApplicationWindow::onWindowActivated ( QWidget * w )
{
    if ( w )
        ((MDIWindow*) w)->onWindowActivated();
}

void ApplicationWindow::onToolAction()
{
    QAction* sentBy = (QAction*) sender();
    QWorkspace* ws = ApplicationWindow::getWorkspace();
    Document* doc = ((MDIWindow*)ws->activeWindow())->getDocument();

    if( sentBy == myToolActions.at( ToolWireframeId ) )
        doc->onWireframe();

    if( sentBy == myToolActions.at( ToolShadingId ) )
        doc->onShading();

    if( sentBy == myToolActions.at( ToolColorId ) )
        doc->onColor();

    if( sentBy == myToolActions.at( ToolMaterialId ) )
        doc->onMaterial();

    if( sentBy == myToolActions.at( ToolTransparencyId ) )
        doc->onTransparency();

    if( sentBy == myToolActions.at( ToolDeleteId ) )
        doc->onDelete();
}

void ApplicationWindow::onSelectionChanged()
{
    QWorkspace* ws = ApplicationWindow::getWorkspace();
    Document* doc = ((MDIWindow*)ws->activeWindow())->getDocument();
    Handle(AIS_InteractiveContext) context = doc->getContext();

    bool OneOrMoreInShading = false;
    bool OneOrMoreInWireframe = false;
    int numSel = context->NbSelected();
    if ( numSel )
    {
        for ( context->InitCurrent(); context->MoreCurrent(); context->NextCurrent() )
        {
            if ( context->IsDisplayed( context->Current(), 1 ) )
                OneOrMoreInShading = true;
            if ( context->IsDisplayed( context->Current(), 0 ) )
                OneOrMoreInWireframe = true;
        }
        myToolActions.at( ToolWireframeId )->setEnabled( OneOrMoreInShading );
        myToolActions.at( ToolShadingId )->setEnabled( OneOrMoreInWireframe );
        myToolActions.at( ToolColorId )->setEnabled( true );
        myToolActions.at( ToolMaterialId )->setEnabled( true );
        myToolActions.at( ToolTransparencyId )->setEnabled( OneOrMoreInShading );
        myToolActions.at( ToolDeleteId )->setEnabled( true );
    }
    else
    {
        myToolActions.at( ToolWireframeId )->setEnabled( false );
        myToolActions.at( ToolShadingId )->setEnabled( false );
        myToolActions.at( ToolColorId )->setEnabled( false );
        myToolActions.at( ToolMaterialId )->setEnabled( false );
        myToolActions.at( ToolTransparencyId )->setEnabled( false );
        myToolActions.at( ToolDeleteId )->setEnabled( false );
    }
    myCasCadeTranslateActions.at( FileExportBREPId )->setEnabled( numSel );
    myCasCadeTranslateActions.at( FileExportCSFDBId )->setEnabled( numSel );
    myCasCadeTranslateActions.at( FileExportIGESId )->setEnabled( numSel );
    myCasCadeTranslateActions.at( FileExportSTEPId )->setEnabled( numSel );
    myCasCadeTranslateActions.at( FileExportSTLId )->setEnabled( numSel );
    myCasCadeTranslateActions.at( FileExportVRMLId )->setEnabled( numSel );
}

void ApplicationWindow::onSetMaterial( int theMaterial )
{
    QWorkspace* ws = getWorkspace();
    Document* doc = ((MDIWindow*)ws->activeWindow())->getDocument();
    doc->onMaterial( theMaterial );
}

QString ApplicationWindow::getResourceDir()
{
    static QString resDir( ::getenv( "CSF_ResourcesDefaults" ) );
	if (resDir.isEmpty()) resDir = "res";
    return resDir;
}

void ApplicationWindow::onExportImage()
{
    MDIWindow* w = (MDIWindow*)stWs->activeWindow();
    if ( w )
        w->dump();
}

void ApplicationWindow::resizeEvent( QResizeEvent* e )
{
    QMainWindow::resizeEvent( e );
    statusBar()->setSizeGripEnabled( !isMaximized() );
}
