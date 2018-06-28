#include "Document.h"

#include "Application.h"
#include "Transparency.h"
#include "Material.h"

#include <qstatusbar.h>
#include <qapplication.h>
#include <qcolor.h>
#include <qcolordialog.h>

#ifndef WNT
#include <Graphic3d_GraphicDevice.hxx>
#else
#include <Graphic3d_WNTGraphicDevice.hxx>
#endif

#include <AIS_InteractiveObject.hxx>
#include <Graphic3d_NameOfMaterial.hxx>

Handle(V3d_Viewer) Document::Viewer( const Standard_CString aDisplay,
				                     const Standard_ExtString aName,
				                     const Standard_CString aDomain,
				                     const Standard_Real ViewSize,
				                     const V3d_TypeOfOrientation ViewProj,
				                     const Standard_Boolean ComputedMode,
				                     const Standard_Boolean aDefaultComputedMode )
{
#ifndef WNT
static Handle(Graphic3d_GraphicDevice) defaultdevice;
    if( defaultdevice.IsNull() )
        defaultdevice = new Graphic3d_GraphicDevice( aDisplay );
   return new V3d_Viewer(defaultdevice,aName,aDomain,ViewSize,ViewProj,
						 Quantity_NOC_GRAY30,V3d_ZBUFFER,V3d_GOURAUD,V3d_WAIT,
						 ComputedMode,aDefaultComputedMode,V3d_TEX_NONE);
#else
static Handle(Graphic3d_WNTGraphicDevice) defaultdevice;
    if( defaultdevice.IsNull() )
        defaultdevice = new Graphic3d_WNTGraphicDevice();
   return new V3d_Viewer(defaultdevice,aName,aDomain,ViewSize,ViewProj,
	   Quantity_NOC_GRAY30,V3d_ZBUFFER,V3d_GOURAUD,V3d_WAIT,
	   ComputedMode,aDefaultComputedMode,V3d_TEX_NONE);
#endif
}

Document::Document( const int theIndex, ApplicationWindow* app, const char* name )
: QObject( app, name ),
myApp( app ),
myIndex( theIndex ),
myNbViews( 0 )
{
    TCollection_ExtendedString a3DName("Visu3D");
    myViewer = Viewer( getenv("DISPLAY"), a3DName.ToExtString(), "", 1000.0,
		               V3d_XposYnegZpos, Standard_True, Standard_True );

    myViewer->Init();
	myViewer->SetDefaultLights();
	myViewer->SetLightOn();

	myContext =new AIS_InteractiveContext(myViewer);	
	onCreateNewView();
}

Document::~Document()
{
}

ApplicationWindow* Document::getApplication()
{
	return myApp;
}

void Document::onCreateNewView() 
{
	QWorkspace* ws = myApp->getWorkspace();
    MDIWindow* w = new MDIWindow( this, ws, 0, 0 );
	myViews.append(w);

    connect( w,    SIGNAL( selectionChanged() ),
             this, SIGNAL( selectionChanged() ) );
    connect( w, SIGNAL( message( const QString&, int ) ),
	         myApp->statusBar(), SLOT( message( const QString&, int ) ) );
    connect( w, SIGNAL( sendCloseView( MDIWindow* ) ),
	         this, SLOT( onCloseView( MDIWindow* ) ) );

	QString aName;
    w->setCaption( aName.sprintf( "Document %d:%d", myIndex, ++myNbViews ) );
    QString dir = ApplicationWindow::getResourceDir() + QString( "/" );
	
    w->setIcon( QPixmap(dir+tr("ICON_DOC")) );

    if ( ws->windowList().isEmpty() )
	    w->showMaximized();
    else
	    w->show();

    w->setFocus();

    getApplication()->onSelectionChanged();
}

void Document::onCloseView(MDIWindow* theView)
{
    removeView(theView);
    if( countOfWindow() == 0 )
        emit sendCloseDocument( this );
}

void Document::removeView(MDIWindow* theView)
{
    if ( myViews.containsRef( theView ) )
    {
		myViews.removeRef(theView);
		delete theView;
	}
}
void Document::removeViews()
{
    for( MDIWindow* aView = (MDIWindow*)myViews.first(); aView; aView = (MDIWindow*)myViews.first() )
		removeView(aView);
}

int Document::countOfWindow()
{
	return myViews.count();
}

Handle(AIS_InteractiveContext) Document::getContext()
{
	return myContext;
}

void Document::fitAll()
{
	for(MDIWindow* aView = (MDIWindow*) myViews.first(); aView; aView = (MDIWindow*) myViews.next())
		aView->fitAll();
}

void Document::onWireframe()
{
    QApplication::setOverrideCursor( Qt::waitCursor );
    for( myContext->InitCurrent(); myContext->MoreCurrent(); myContext->NextCurrent() )
        myContext->SetDisplayMode( myContext->Current(), 0, false );
    myContext->UpdateCurrentViewer();
    getApplication()->onSelectionChanged();
    QApplication::restoreOverrideCursor();
}

void Document::onShading()
{
    QApplication::setOverrideCursor( Qt::waitCursor );
    for( myContext->InitCurrent(); myContext->MoreCurrent(); myContext->NextCurrent() )
        myContext->SetDisplayMode( myContext->Current(), 1, false );
    myContext->UpdateCurrentViewer();
    getApplication()->onSelectionChanged();
    QApplication::restoreOverrideCursor();
}

void Document::onColor()
{
    QColor aColor ;
    Quantity_Color aShapeColor;
    myContext->InitCurrent();
    Handle_AIS_InteractiveObject Current = myContext->Current() ;
    if ( Current->HasColor () )
    {
        aShapeColor = myContext->Color( Current );
        aColor.setRgb( aShapeColor.Red() * 255, aShapeColor.Green() * 255, aShapeColor.Blue() * 255 );
    }
    else
        aColor.setRgb( 255, 255, 255 );

    QColor aRetColor = QColorDialog::getColor( aColor );
    if ( aRetColor.isValid() )
    {
        Quantity_Color color( aRetColor.red() / 255., aRetColor.green() / 255.,
			                  aRetColor.blue() / 255., Quantity_TOC_RGB );
        for (; myContext->MoreCurrent(); myContext->NextCurrent() )
            myContext->SetColor( myContext->Current(), color.Name() );
    }
}

void Document::onMaterial( int theMaterial )
{
    for ( myContext->InitCurrent(); myContext->MoreCurrent (); myContext->NextCurrent () )
        myContext->SetMaterial( myContext->Current(), (Graphic3d_NameOfMaterial)theMaterial );
}

void Document::onMaterial()
{
    DialogMaterial* m = new DialogMaterial();
    connect( m, SIGNAL( sendMaterialChanged( int ) ), this, SLOT( onMaterial( int ) ) );
    m->exec();
}

void Document::onTransparency( int theTrans )
{
    for( myContext->InitCurrent(); myContext->MoreCurrent(); myContext->NextSelected() )
        myContext->SetTransparency( myContext->Current(), ((Standard_Real)theTrans) / 10.0 );
}

void Document::onTransparency()
{
    DialogTransparency* aDialog = new DialogTransparency();
    connect( aDialog, SIGNAL( sendTransparencyChanged( int ) ), this, SLOT( onTransparency( int ) ) );
    aDialog->exec();
}

void Document::onDelete()
{
    for ( myContext->InitCurrent(); myContext->MoreCurrent(); myContext->NextCurrent() )
        myContext->Erase( myContext->Current(), false, true );
    myContext->ClearSelected();
    getApplication()->onSelectionChanged();
}
