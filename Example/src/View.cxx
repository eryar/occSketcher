#if !defined WNT
#define QT_CLEAN_NAMESPACE         /* avoid definition of INT32 and INT8 */
#endif

#include "View.h"
#include "Application.h"
#include "Sketcher_QtGUI.hxx"

#include <qapplication.h>
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qcolordialog.h>
#include <qmessagebox.h>

#ifdef WNT
#include <WNT_Window.hxx>
#include <Graphic3d_WNTGraphicDevice.hxx>
#else
#include <GL/glx.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xmu/StdCmap.h>
#include <X11/Xlib.h>
#include <Xw_Window.hxx>
#include <Graphic3d_GraphicDevice.hxx>
#endif

// the key for multi selection :
#define MULTISELECTIONKEY Qt::ShiftButton

// the key for shortcut ( use to activate dynamic rotation, panning )
#define CASCADESHORTCUTKEY Qt::ControlButton

// for elastic bean selection
#define ValZWMin 1

static QCursor* defCursor     = NULL;
static QCursor* handCursor    = NULL;
static QCursor* panCursor     = NULL;
static QCursor* globPanCursor = NULL;
static QCursor* zoomCursor    = NULL;
static QCursor* rotCursor     = NULL;

View::View( Handle(AIS_InteractiveContext) theContext, QWidget* parent, const char* name, int wflags )
: QWidget( parent, name, wflags | WRepaintNoErase | WResizeNoErase ),
myViewActions( 0 )
{
#ifndef WNT
    XSynchronize( x11Display(),true ); // it is possible to use QApplication::syncX();
#endif
    myFirst = true;
    myContext = theContext;

    myXmin = 0;
    myYmin = 0;
    myXmax = 0;
    myYmax = 0;
    myCurZoom = 0;

#if !defined WNT
    XVisualInfo* pVisualInfo;
    if ( x11Display() )
    {
        /* Initialization with the default VisualID */
        Visual *v = DefaultVisual( x11Display(), DefaultScreen( x11Display() ) );
        int visualID = XVisualIDFromVisual( v );

        /*  Here we use the settings from Optimizer_ViewInfo::TxglCreateWindow() */
        int visualAttr[] = { GLX_RGBA, GLX_DEPTH_SIZE, 1, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1,
			                 GLX_BLUE_SIZE, 1, GLX_DOUBLEBUFFER, None };
        pVisualInfo = ::glXChooseVisual( x11Display(), DefaultScreen( x11Display() ), visualAttr );

        if ( isVisible() )
            hide();

        XSetWindowAttributes a;

        Window p = RootWindow( x11Display(), DefaultScreen( x11Display() ) );
        a.colormap = XCreateColormap( x11Display(), RootWindow( x11Display(), pVisualInfo->screen ),
                                      pVisualInfo->visual, AllocNone );
        a.background_pixel = backgroundColor().pixel();
        a.border_pixel = black.pixel();
        if ( parentWidget() )
            p = parentWidget()->winId();

        Window w = XCreateWindow( x11Display(), p,  x(), y(), width(), height(),
			                      0, pVisualInfo->depth, InputOutput,  pVisualInfo->visual,
			                      CWBackPixel | CWBorderPixel | CWColormap, &a );
        Window *cmw;
        Window *cmwret;
        int count;
        if ( XGetWMColormapWindows( x11Display(), topLevelWidget()->winId(), &cmwret, &count ) )
        {
            cmw = new Window[count+1];
            memcpy( (char *)cmw, (char *)cmwret, sizeof(Window)*count );
            XFree( (char *)cmwret );
            int i;
            for ( i = 0; i < count; i++ )
            {
	            if ( cmw[i] == winId() )  /* replace old window */
                {
	                cmw[i] = w;
	                break;
	            }
            }
            if ( i >= count )			 /* append new window */
	            cmw[count++] = w;
        }
        else
        {
            count = 1;
            cmw = new Window[count];
            cmw[0] = w;
        }
        /* Creating new window (with good VisualID) for this widget */
        create(w);
        XSetWMColormapWindows( x11Display(), topLevelWidget()->winId(), cmw, count );
        delete [] cmw;

        if ( isVisible() )
            show();
        if ( pVisualInfo )
            XFree( (char *)pVisualInfo );
        XFlush( x11Display() );
    }
#endif
    myCurrentMode = CurAction3d_Nothing;
    myDegenerateModeIsOn = Standard_True;
    setMouseTracking( true );

  	Sketcher_QtGUI* mySG = new Sketcher_QtGUI(parent);
	mySketcher = new Sketcher( myContext, mySG);
	GRIDCounter = true;

    initViewActions();
    initCursors();
}

View::~View()
{
	delete mySketcher;
}

void View::init()
{
    myView = myContext->CurrentViewer()->CreateView();
    int windowHandle = (int) winId();
    short hi, lo;
    lo = (short) windowHandle;
    hi = (short) (windowHandle >> 16);
#ifdef WNT
    Handle(WNT_Window) hWnd = new WNT_Window(Handle(Graphic3d_WNTGraphicDevice)::DownCast(myContext->CurrentViewer()->Device()),(int) hi,(int) lo);
#else
    Handle(Xw_Window) hWnd = new Xw_Window(Handle(Graphic3d_GraphicDevice)::DownCast(myContext->CurrentViewer()->Device()),(int) hi,(int) lo,Xw_WQ_SAMEQUALITY);
#endif // WNT
    myView->SetWindow( hWnd );
    if ( !hWnd->IsMapped() )
        hWnd->Map();
    myView->SetBackgroundColor(Quantity_NOC_BLACK);
    myView->MustBeResized();

}

void View::paintEvent( QPaintEvent * e )
{
//  QApplication::syncX();
    if( myFirst )
    {
        init();
        myFirst = false;
    }
    myView->Redraw();
}

void View::resizeEvent( QResizeEvent * e)
{
//  QApplication::syncX();
    if( !myView.IsNull() )
    {
        myView->MustBeResized();
    }
}

void View::fitAll()
{
    myView->FitAll();
    myView->ZFitAll();
    myView->Redraw();
}

void View::fitArea()
{
    myCurrentMode = CurAction3d_WindowZooming;
}

void View::zoom()
{
    myCurrentMode = CurAction3d_DynamicZooming;
}

void View::pan()
{
    myCurrentMode = CurAction3d_DynamicPanning;
}

void View::rotation()
{
    myCurrentMode = CurAction3d_DynamicRotation;
}

void View::globalPan()
{
    // save the current zoom value
    myCurZoom = myView->Scale();
    // Do a Global Zoom
    myView->FitAll();
    // Set the mode
    myCurrentMode = CurAction3d_GlobalPanning;
}

void View::front()
{
    myView->SetProj( V3d_Xpos );
}

void View::back()
{
    myView->SetProj( V3d_Xneg );
}

void View::top()
{
    myView->SetProj( V3d_Zpos );
}

void View::bottom()
{
    myView->SetProj( V3d_Zneg );
}

void View::left()
{
    myView->SetProj( V3d_Ypos );
}

void View::right()
{
    myView->SetProj( V3d_Yneg );
}

void View::axo()
{
    myView->SetProj( V3d_XposYnegZpos );
}

void View::reset()
{
    myView->Reset();
}

void View::hlrOff()
{
    QApplication::setOverrideCursor( Qt::waitCursor );
    myView->SetDegenerateModeOn();
    myDegenerateModeIsOn = Standard_True;
    QApplication::restoreOverrideCursor();
}

void View::hlrOn()
{
    QApplication::setOverrideCursor( Qt::waitCursor );
    myView->SetDegenerateModeOff();
    myDegenerateModeIsOn = Standard_False;
    QApplication::restoreOverrideCursor();
}

void View::updateToggled( bool isOn )
{
    QAction* sentBy = (QAction*)sender();

    if( !isOn )
        return;

	if (myCurrentMode == SketcherAction)
		mySketcher->OnCancel();

    for ( int i = ViewFitAllId; i <= MyTrimCurveAction; i++ )
    {
        QAction* anAction = myViewActions->at( i );
        if ( ( anAction == myViewActions->at( ViewFitAreaId ) ) ||
             ( anAction == myViewActions->at( ViewZoomId ) ) ||
             ( anAction == myViewActions->at( ViewPanId ) ) ||
             ( anAction == myViewActions->at( ViewGlobalPanId ) ) ||
             ( anAction == myViewActions->at( ViewRotationId ) )  ||
            ( anAction == myViewActions->at( MyEraseActionId ) ) ||
            ( anAction == myViewActions->at( MyDeleteActionId ) ) ||
            ( anAction == myViewActions->at( MyPropertyActionId ) ) ||
            ( anAction == myViewActions->at( MyRedrawActionId ) ) ||
            ( anAction == myViewActions->at( MyChangePlaneAction ) ) ||
            ( anAction == myViewActions->at( MyInputPointAction ) ) ||
             ( anAction == myViewActions->at( MyInputLineAction ) ) ||
             ( anAction == myViewActions->at( MyInputCircleAction ) ) ||
             ( anAction == myViewActions->at( MyInputCircle3PAction ) ) ||
             ( anAction == myViewActions->at( MyInputCircle2PTanAction ) ) ||
             ( anAction == myViewActions->at( MyInputCircleP2TanAction ) ) ||
             ( anAction == myViewActions->at( MyInputCircle3TanAction ) ) ||
             ( anAction == myViewActions->at( MyInputArc3PAction ) )  || 
             ( anAction == myViewActions->at( MyInputArcCenter2PAction ) )  || 
             ( anAction == myViewActions->at( MyInputBezierCurveAction ) ) ||
             ( anAction == myViewActions->at( MyTrimCurveAction ) )  )
		{
            if ( anAction && ( anAction != sentBy ) )
            {
			if(( anAction == myViewActions->at( MyEraseActionId ) ) ||
            ( anAction == myViewActions->at( MyDeleteActionId ) ) ||
            ( anAction == myViewActions->at( MyPropertyActionId ) ) ||
            ( anAction == myViewActions->at( MyRedrawActionId ) ) ||
            ( anAction == myViewActions->at( MyChangePlaneAction ) ) )
				anAction->setToggleAction( false );
			else
				anAction->setToggleAction( true );
	            anAction->setOn( false );
            }
            else
            {
	            if ( sentBy == myViewActions->at( ViewFitAreaId ) )
	                setCursor( *handCursor );
	            else if	( sentBy == myViewActions->at( ViewZoomId ) )
	                setCursor( *zoomCursor );
	            else if	( sentBy == myViewActions->at( ViewPanId ) )
	                setCursor( *panCursor );
	            else if	( sentBy == myViewActions->at( ViewGlobalPanId ) )
	                setCursor( *globPanCursor );
	            else if ( sentBy == myViewActions->at( ViewRotationId ) )
	                setCursor( *rotCursor );
                else
	                setCursor( *defCursor );
	            sentBy->setToggleAction( false );
            }
        }
    }
}

void View::initCursors()
{
    if ( !defCursor )
        defCursor = new QCursor( Qt::arrowCursor );
    if ( !handCursor )
        handCursor = new QCursor( Qt::pointingHandCursor );
    if ( !panCursor )
        panCursor = new QCursor( Qt::sizeAllCursor );
    if ( !globPanCursor )
        globPanCursor = new QCursor( Qt::crossCursor );
    if ( !zoomCursor )
        zoomCursor = new QCursor( QPixmap( ApplicationWindow::getResourceDir() + QString( "/" ) + tr( "ICON_CURSOR_ZOOM" ) ) );
    if ( !rotCursor )
        rotCursor = new QCursor( QPixmap( ApplicationWindow::getResourceDir() + QString( "/" ) + tr( "ICON_CURSOR_ROTATE" ) ) );
}

QList<QAction>* View::getViewActions()
{
    initViewActions();
    return myViewActions;
}

void View::initViewActions()
{
    if ( myViewActions )
        return;

    myViewActions = new QList<QAction>();
    QString dir = ApplicationWindow::getResourceDir() + QString( "/" );
    QAction* a;

    a = new QAction( tr("TBR_FITALL"), QPixmap( dir+tr("ICON_VIEW_FITALL") ), tr("MNU_FITALL"), 0, this, "fitall" );
    connect( a, SIGNAL( activated() ) , this, SLOT( fitAll() ) );
    myViewActions->insert(ViewFitAllId, a);

    a = new QAction( tr("TBR_FITAREA"), QPixmap( dir+tr("ICON_VIEW_FITAREA") ), tr("MNU_FITAREA"), 0, this, "fitarea" );
    connect( a, SIGNAL( activated() ) , this, SLOT( fitArea() ) );
    a->setToggleAction(true);
    connect( a, SIGNAL( toggled( bool ) ) , this, SLOT( updateToggled( bool ) ) );
    myViewActions->insert(ViewFitAreaId, a);

    a = new QAction( tr("TBR_ZOOM"), QPixmap( dir+tr("ICON_VIEW_ZOOM") ), tr("MNU_ZOOM"), 0, this, "zoom" );
    connect( a, SIGNAL( activated() ) , this, SLOT( zoom() ) );
    a->setToggleAction(true);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
    myViewActions->insert(ViewZoomId, a);

    a = new QAction( tr("TBR_PAN"), QPixmap( dir+tr("ICON_VIEW_PAN") ), tr("MNU_PAN"), 0, this, "pan" );
    connect( a, SIGNAL( activated() ) , this, SLOT( pan() ) );
    a->setToggleAction(true);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
    myViewActions->insert(ViewPanId, a);

    a = new QAction( tr("TBR_GLOBALPAN"), QPixmap( dir+tr("ICON_VIEW_GLOBALPAN") ), tr("MNU_GLOBALPAN"), 0, this, "global pan" );
    connect( a, SIGNAL( activated() ) , this, SLOT( globalPan() ) );
    a->setToggleAction(true);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
    myViewActions->insert(ViewGlobalPanId, a);

    a = new QAction( tr("TBR_FRONT"), QPixmap( dir+tr("ICON_VIEW_FRONT") ), tr("MNU_FRONT"), 0, this, "front" );
    connect( a, SIGNAL( activated() ) , this, SLOT( front() ) );
    myViewActions->insert(ViewFrontId, a);

    a = new QAction( tr("TBR_BACK"), QPixmap( dir+tr("ICON_VIEW_BACK") ), tr("MNU_BACK"), 0, this, "back" );
    connect( a, SIGNAL( activated() ) , this, SLOT( back() ) );
    myViewActions->insert(ViewBackId, a);

    a = new QAction( tr("TBR_TOP"), QPixmap( dir+tr("ICON_VIEW_TOP") ), tr("MNU_TOP"), 0, this, "top" );
    connect( a, SIGNAL( activated() ) , this, SLOT( top() ) );
    myViewActions->insert(ViewTopId, a);

    a = new QAction( tr("TBR_BOTTOM"), QPixmap( dir+tr("ICON_VIEW_BOTTOM") ), tr("MNU_BOTTOM"), 0, this, "bottom" );
    connect( a, SIGNAL( activated() ) , this, SLOT( bottom() ) );
    myViewActions->insert(ViewBottomId, a);

    a = new QAction( tr("TBR_LEFT"), QPixmap( dir+tr("ICON_VIEW_LEFT") ), tr("MNU_LEFT"), 0, this, "left" );
    connect( a, SIGNAL( activated() ) , this, SLOT( left() ) );
    myViewActions->insert(ViewLeftId, a);

    a = new QAction( tr("TBR_RIGHT"), QPixmap( dir+tr("ICON_VIEW_RIGHT") ), tr("MNU_RIGHT"), 0, this, "right" );
    connect( a, SIGNAL( activated() ) , this, SLOT( right() ) );
    myViewActions->insert(ViewRightId, a);

    a = new QAction( tr("TBR_AXO"), QPixmap( dir+tr("ICON_VIEW_AXO") ), tr("MNU_AXO"), 0, this, "axo" );
    connect( a, SIGNAL( activated() ) , this, SLOT( axo() ) );
    myViewActions->insert(ViewAxoId, a);

    a = new QAction( tr("TBR_ROTATION"), QPixmap( dir+tr("ICON_VIEW_ROTATION") ), tr("MNU_ROTATION"), 0, this, "zoom" );
    connect( a, SIGNAL( activated() ) , this, SLOT( rotation() ) );
    a->setToggleAction(true);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
    myViewActions->insert(ViewRotationId, a);

    a = new QAction( tr("TBR_RESET"), QPixmap( dir+tr("ICON_VIEW_RESET") ), tr("MNU_RESET"), 0, this, "reset" );
    connect( a, SIGNAL( activated() ) , this, SLOT( reset() ) );
    myViewActions->insert(ViewResetId, a);

    QActionGroup* ag = new QActionGroup( this );

    a = new QAction( tr("TBR_HLROFF"), QPixmap( dir+tr("ICON_VIEW_HLROFF") ), tr("MNU_HLROFF"), 0, this, "hlroff" );
    connect( a, SIGNAL( activated() ) , this, SLOT( hlrOff() ) );
    a->setToggleAction(true);
    ag->insert(a);
    myViewActions->insert(ViewHlrOffId, a);

    a = new QAction( tr("TBR_HLRON"), QPixmap( dir+tr("ICON_VIEW_HLRON") ), tr("MNU_HLRON"), 0, this, "hlron" );
    connect( a, SIGNAL( activated() ) ,this, SLOT( hlrOn() ) );
    a->setToggleAction(true);
    ag->insert(a);
    myViewActions->insert(ViewHlrOnId, a);

/*********************MAS INSERT*******************************/
	a = new QAction( tr("TBR_ERASE"), QPixmap( dir+tr("ICON_ERASE") ), tr("MNU_ERASE"), 0, this, "erase all" );
	connect( a, SIGNAL( activated() ) , this, SLOT( onErase() ) );
	myViewActions->insert(MyEraseActionId, a);
    a->setToggleAction(false);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );

	a = new QAction( tr("TBR_DELETE"), QPixmap( dir+tr("ICON_DELETE") ), tr("MNU_DELETE"), 0, this, "delete selected" );
	connect( a, SIGNAL( activated() ) , this, SLOT( onDeleteSelected() ) );
	myViewActions->insert(MyDeleteActionId, a);
    a->setToggleAction(false);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );

	a = new QAction( tr("TBR_PROP"), QPixmap( dir+tr("ICON_PROP") ), tr("MNU_PROP"), 0, this, "view objects property" );
	connect( a, SIGNAL( activated() ) , this, SLOT( onProperty() ) );
    a->setToggleAction(false);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
	myViewActions->insert(MyPropertyActionId, a);

	a = new QAction( tr("TBR_REDRAW"), QPixmap( dir+tr("ICON_REDRAW") ), tr("MNU_REDRAW"), 0, this, "redraw objects" );
	connect( a, SIGNAL( activated() ) , this, SLOT( onRedrawAll() ) );
    a->setToggleAction(false);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
	myViewActions->insert(MyRedrawActionId, a);

	a = new QAction( tr("TBR_PLANE"), QPixmap( dir+tr("ICON_PLANE") ), tr("MNU_PLANE"), 0, this, "change plane" );
	connect( a, SIGNAL( activated() ) , this, SLOT( onChangePlane() ) );
    a->setToggleAction(false);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
	myViewActions->insert(MyChangePlaneAction, a);

	a = new QAction( tr("TBR_GRID"), QPixmap( dir+tr("ICON_GRID") ), tr("MNU_GRID"), 0, this, "grid" );
	connect( a, SIGNAL( activated() ) , this, SLOT( onGrid() ) );
	myViewActions->insert(MyGridActionId, a);

	a = new QAction( tr("TBR_IPOINT"), QPixmap( dir+tr("ICON_VIEW_IPOINT") ), tr("MNU_IPOINT"), 0, this, "input points" );
	connect( a, SIGNAL( activated() ) , this, SLOT( onInputPoints() ) );
    a->setToggleAction(true);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
	myViewActions->insert(MyInputPointAction, a);

	a = new QAction( tr("TBR_ILINE"), QPixmap( dir+tr("ICON_VIEW_ILINE") ), tr("MNU_ILINE"), 0, this, "input line" );
	connect( a, SIGNAL( activated() ) , this, SLOT( onInputLines() ) );
    a->setToggleAction(true);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
	myViewActions->insert(MyInputLineAction, a);

	a = new QAction( tr("TBR_ICIRCLE"), QPixmap( dir+tr("ICON_VIEW_ICIRCLE") ), tr("MNU_ICIRCLES"), 0, this, "input circles with radius" );
	connect( a, SIGNAL( activated() ) , this, SLOT( onInputCircles() ) );
    a->setToggleAction(true);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
	myViewActions->insert(MyInputCircleAction, a);

	a = new QAction( tr("TBR_ICIRCLE3P"), QPixmap( dir+tr("ICON_VIEW_ICIRCLE3P") ), tr("MNU_ICIRCLE3P"), 0, this, "input circle by three points" );
	connect( a, SIGNAL( activated() ) , this, SLOT( onInputCircles3P() ) );
    a->setToggleAction(true);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
	myViewActions->insert(MyInputCircle3PAction, a);

	a = new QAction( tr("TBR_ICIRCLE2PTAN"), QPixmap( dir+tr("ICON_VIEW_ICIRCLE2PTAN") ), tr("MNU_ICIRCLE2PTAN"), 0, this, "input circle by 2p, tangential to 3 curve" );
	connect( a, SIGNAL( activated() ) , this, SLOT( onInputCircles2PTan() ) );
    a->setToggleAction(true);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
	myViewActions->insert(MyInputCircle2PTanAction, a);

	a = new QAction( tr("TBR_ICIRCLEP2TAN"), QPixmap( dir+tr("ICON_VIEW_ICIRCLEP2TAN") ), tr("MNU_ICIRCLEP2TAN"), 0, this, "input circle by p, 2 tangential to 3 curve" );
	connect( a, SIGNAL( activated() ) , this, SLOT( onInputCirclesP2Tan() ) );
    a->setToggleAction(true);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
	myViewActions->insert(MyInputCircleP2TanAction, a);

	a = new QAction( tr("TBR_ICIRCLE3TAN"), QPixmap( dir+tr("ICON_VIEW_ICIRCLE3TAN") ), tr("MNU_ICIRCLE3TAN"), 0, this, "input circle by three tan" );
	connect( a, SIGNAL( activated() ) , this, SLOT( onInputCircles3Tan() ) );
    a->setToggleAction(true);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
	myViewActions->insert(MyInputCircle3TanAction, a);

	a = new QAction( tr("TBR_IARC3P"), QPixmap( dir+tr("ICON_VIEW_IARC3P") ), tr("MNU_IARC3P"), 0, this, "input arc by 3 points" );
	connect( a, SIGNAL( activated() ) , this, SLOT( onInputArc3P() ) );
    a->setToggleAction(true);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
	myViewActions->insert(MyInputArc3PAction, a);

	a = new QAction( tr("TBR_IARCC2P"), QPixmap( dir+tr("ICON_VIEW_IARCC2P") ), tr("MNU_IARCC2P"), 0, this, "input arc by center &2points" );
	connect( a, SIGNAL( activated() ) , this, SLOT( onInputArcCenter2P() ) );
    a->setToggleAction(true);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
	myViewActions->insert(MyInputArcCenter2PAction, a);

	a = new QAction( tr("TBR_IBEZC"), QPixmap( dir+tr("ICON_VIEW_IBEZC") ), tr("MNU_IBEZC"), 0, this, "input BezierCurve" );
	connect( a, SIGNAL( activated() ) , this, SLOT( onInputBezierCurve() ) );
    a->setToggleAction(true);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
	myViewActions->insert(MyInputBezierCurveAction, a);

	a = new QAction( tr("TBR_TRIM"), QPixmap( dir+tr("ICON_VIEW_TRIM") ), tr("MNU_TRIM"), 0, this, "trim curve" );
	connect( a, SIGNAL( activated() ) , this, SLOT( onTrimCurve() ) );
    a->setToggleAction(true);
    connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
	myViewActions->insert(MyTrimCurveAction, a);


}

/*********************MAS INSERT*******************************/

void View::onaddComboBox(QToolBar* tbr) 
{
    QString dir = ApplicationWindow::getResourceDir() + QString( "/" );

    QComboBox* ComboBoxSnaps = new QComboBox( FALSE, tbr, "ComboBoxSnaps" );
    ComboBoxSnaps->insertItem( QPixmap( dir+tr("ICON_SNAP_NON")),			SnapNothing );
    ComboBoxSnaps->insertItem( QPixmap( dir+tr("ICON_SNAP_END")),			SnapEnd);
    ComboBoxSnaps->insertItem( QPixmap( dir+tr("ICON_SNAP_MIDDLE")),		SnapMiddle);
    ComboBoxSnaps->insertItem( QPixmap( dir+tr("ICON_SNAP_CENTER")),		SnapCenter);
    ComboBoxSnaps->insertItem( QPixmap( dir+tr("ICON_SNAP_NEAREST")),		SnapNearest);
    ComboBoxSnaps->insertItem( QPixmap( dir+tr("ICON_VIEW_INTERSECTION")),	SnapIntersection);
    ComboBoxSnaps->insertItem( QPixmap( dir+tr("ICON_VIEW_TANGENT")),		SnapTangent );
    ComboBoxSnaps->insertItem( QPixmap( dir+tr("ICON_VIEW_PARALLEL")),		SnapParallel );
    ComboBoxSnaps->insertItem( QPixmap( dir+tr("ICON_VIEW_PERPENDICULAR")),	SnapPerpendicular );
    ComboBoxSnaps->insertItem( QPixmap( dir+tr("ICON_VIEW_ANALYSER")),		SnapAnalyse );
    connect( ComboBoxSnaps, SIGNAL( activated ( int )), this, SLOT( updateSnap(int)));
	mySketcher->SetSnap(SnapNothing);

	PolylineModeCheck = new QCheckBox("Polyline mode",tbr,"set mode");
    connect( PolylineModeCheck, SIGNAL( toggled (bool) ) , this, SLOT( setPolylineMode(bool) ) );

    QComboBox* ComboBoxColor = new QComboBox( FALSE, tbr, "ComboBoxColor" );
    ComboBoxColor->insertItem( tr( "BLACK" ) );				
    ComboBoxColor->insertItem( tr( "BROWN" ) );	
    ComboBoxColor->insertItem( tr( "RED" ) );			
    ComboBoxColor->insertItem( tr( "ORANGE" ) );			
    ComboBoxColor->insertItem( tr( "YELLOW" ) );			
    ComboBoxColor->insertItem( tr( "FORESTGREEN" ) );		
    ComboBoxColor->insertItem( tr( "GREEN" ) );		
    ComboBoxColor->insertItem( tr( "BLUE" ) );	
    ComboBoxColor->insertItem( tr( "DEEPSKYBLUE1" ) );		
    ComboBoxColor->insertItem( tr( "LIGHTSKYBLUE" ) );	
    ComboBoxColor->insertItem( tr( "CYAN" ) );				
    ComboBoxColor->insertItem( tr( "PURPLE" ) );		
    ComboBoxColor->insertItem( tr( "MAGENTA" ) );			
    ComboBoxColor->insertItem( tr( "VIOLET" ) );			
    ComboBoxColor->insertItem( tr( "DEEPPINK" ) );			
    ComboBoxColor->insertItem( tr( "PINK" ) );			
    ComboBoxColor->insertItem( tr( "GRAY" ) );		
    ComboBoxColor->insertItem( tr( "WHITE" ) );
	ComboBoxColor->setCurrentItem ( 4 );
    connect( ComboBoxColor, SIGNAL( activated ( int )), this, SLOT( updateColor(int)));

    QComboBox* ComboBoxType = new QComboBox( FALSE, tbr, "ComboBoxType" );
    ComboBoxType->insertItem( tr( "Main" ) );
    ComboBoxType->insertItem( tr( "Auxiliary" ) );
    connect( ComboBoxType, SIGNAL( activated ( int )), this, SLOT( updateType(int)));

    QComboBox* ComboBoxStyle = new QComboBox( FALSE, tbr, "ComboBoxStyle" );
    ComboBoxStyle->insertItem( tr( "SOLID" ) );
    ComboBoxStyle->insertItem( tr( "DASH" ) );
    ComboBoxStyle->insertItem( tr( "DOT" ) );
    ComboBoxStyle->insertItem( tr( "DOTDASH" ) );
    connect( ComboBoxStyle, SIGNAL( activated ( int )), this, SLOT( updateStyle(int)));

    QComboBox* ComboBoxWidth = new QComboBox( FALSE, tbr, "ComboBoxWidth" );
    ComboBoxWidth->insertItem( tr( "1.0" ) );
    ComboBoxWidth->insertItem( tr( "2.0" ) );
    ComboBoxWidth->insertItem( tr( "3.0" ) );
    ComboBoxWidth->insertItem( tr( "4.0" ) );
    ComboBoxWidth->insertItem( tr( "5.0" ) );
    ComboBoxWidth->insertItem( tr( "6.0" ) );
    ComboBoxWidth->insertItem( tr( "7.0" ) );
    ComboBoxWidth->insertItem( tr( "8.0" ) );
    ComboBoxWidth->insertItem( tr( "9.0" ) );
    connect( ComboBoxWidth, SIGNAL( activated ( int )), this, SLOT( updateWidth(int)));

}


void View::updateSnap(int pos) 
{
	mySketcher->SetSnap((Sketcher_SnapType)pos);
}

void View::updateColor(int pos)
{
	switch(pos)
	{
	case 0:  mySketcher->SetColor(Quantity_NOC_BLACK);
			 break;
	case 1:  mySketcher->SetColor(Quantity_NOC_BROWN);
			 break;
	case 2:  mySketcher->SetColor(Quantity_NOC_RED);
			 break;
	case 3:  mySketcher->SetColor(Quantity_NOC_ORANGE);
			 break;
	case 4:  mySketcher->SetColor(Quantity_NOC_YELLOW);
			 break;
	case 5:  mySketcher->SetColor(Quantity_NOC_FORESTGREEN);
			 break;
	case 6:  mySketcher->SetColor(Quantity_NOC_GREEN);
			 break;
	case 7:  mySketcher->SetColor(Quantity_NOC_BLUE1);
			 break;
	case 8:  mySketcher->SetColor(Quantity_NOC_DEEPSKYBLUE1);
			 break;
	case 9:  mySketcher->SetColor(Quantity_NOC_LIGHTSKYBLUE);
			 break;
	case 10: mySketcher->SetColor(Quantity_NOC_CYAN1);
			 break;
	case 11: mySketcher->SetColor(Quantity_NOC_PURPLE);
			 break;
	case 12: mySketcher->SetColor(Quantity_NOC_MAGENTA1);
			 break;
	case 13: mySketcher->SetColor(Quantity_NOC_VIOLET);
			 break;
	case 14: mySketcher->SetColor(Quantity_NOC_DEEPPINK);
			 break;
	case 15: mySketcher->SetColor(Quantity_NOC_PINK);
			 break;
	case 16: mySketcher->SetColor(Quantity_NOC_GRAY70);
			 break;
	case 17: mySketcher->SetColor(Quantity_NOC_WHITE);
			 break;
	default: break;
	}
} 

void View::updateType(int pos)
{
	mySketcher->SetType((Sketcher_ObjectType)pos);
}

void View::updateStyle(int pos)
{
	mySketcher->SetStyle((Aspect_TypeOfLine)pos);
}

void View::updateWidth(int pos)
{
	mySketcher->SetWidth((double)(pos+1));
}

void View::setPolylineMode(bool mode) 
{
	if(mode)mySketcher->SetPolylineMode(Standard_True);
	else	mySketcher->SetPolylineMode(Standard_False);
}

void View::onErase() 
{
//	myContext->EraseSelected();
//	myContext->UpdateCurrentViewer();
//	myContext->UpdateCurrent();
//  myContext->CloseLocalContext();
  myContext->EraseAll();
//  myView->Redraw ();
// myView->Update ();
}

void View::onDeleteSelected()
{
 mySketcher->DeleteSelectedObject();
}


void View::onProperty()
{
 mySketcher->ViewProperties();
}

void View::onRedrawAll()
{
 mySketcher->RedrawAll();
}


void View::onChangePlane()
{
 gp_Dir dir(2,0,1); 

 gp_Ax3 newgp_Ax3(gp::Origin(),dir);

 mySketcher->SetCoordinateSystem (newgp_Ax3);
}

void View::onGrid()
{
  mySketcher->GetCoordinateSystem();
  Handle(V3d_Viewer) aViewer = myView->Viewer();
  if (GRIDCounter)
  {
	aViewer->ActivateGrid(GRID1);
	GRIDCounter = false;
  }
  else 
  {
	aViewer->DeactivateGrid();
//	aViewer->ActivateGrid(GRID2);
	GRIDCounter = true;
  }

  myView->Update ();
}


void View::onInputPoints() 
{
 mySketcher->ObjectAction (Point_Method);
 myCurrentMode = SketcherAction;
}

void View::onInputLines() 
{
 mySketcher->ObjectAction (Line2P_Method);
 myCurrentMode = SketcherAction;
}

void View::onInputCircles()
{
 mySketcher->ObjectAction (CircleCenterRadius_Method);
 myCurrentMode = SketcherAction;
}

void View::onInputCircles3P()
{
 mySketcher->ObjectAction (Circle3P_Method);
 myCurrentMode = SketcherAction;
}

void View::onInputCircles2PTan()
{
 mySketcher->ObjectAction (Circle2PTan_Method);
 myCurrentMode = SketcherAction;
}

void View::onInputCirclesP2Tan()
{
 mySketcher->ObjectAction (CircleP2Tan_Method);
 myCurrentMode = SketcherAction;
}

void View::onInputCircles3Tan()
{
 mySketcher->ObjectAction (Circle3Tan_Method);
 myCurrentMode = SketcherAction;
}


void View::onInputArc3P()
{
 mySketcher->ObjectAction (Arc3P_Method);
 myCurrentMode = SketcherAction;
}


void View::onInputArcCenter2P()
{
 mySketcher->ObjectAction (ArcCenter2P_Method);
 myCurrentMode = SketcherAction;
}

void View::onInputBezierCurve()
{
 mySketcher->ObjectAction (BezierCurve_Method);
 myCurrentMode = SketcherAction;
}

void View::onTrimCurve()
{
 mySketcher->ObjectAction (Trim_Method);
 myCurrentMode = SketcherAction;
}

/*********************MAS INSERT*******************************/


void View::mousePressEvent( QMouseEvent* e )
{
    if ( e->button() == Qt::LeftButton )
        onLButtonDown( e->state(), e->pos() );
    else if ( e->button() == Qt::MidButton )
        onMButtonDown( e->state(), e->pos() );
    else if ( e->button() == Qt::RightButton )
        onRButtonDown( e->state(), e->pos() );
}

void View::mouseReleaseEvent(QMouseEvent* e)
{
    if ( e->button() == Qt::LeftButton )
        onLButtonUp( e->state(), e->pos() );
    else if ( e->button() == Qt::MidButton )
        onMButtonUp( e->state(), e->pos() );
    else if( e->button() == Qt::RightButton )
        onRButtonUp( e->state(), e->pos() );
}

void View::mouseMoveEvent(QMouseEvent* e)
{
    onMouseMove( e->state(), e->pos() );
}

void View::activateCursor( const CurrentAction3d mode )
{
    switch( mode )
    {
        case CurAction3d_DynamicPanning:
            setCursor( *panCursor );
            break;
        case CurAction3d_DynamicZooming:
            setCursor( *zoomCursor );
            break;
        case CurAction3d_DynamicRotation:
            setCursor( *rotCursor );
            break;
        case CurAction3d_GlobalPanning:
            setCursor( *globPanCursor );
            break;
        case CurAction3d_WindowZooming:
            setCursor( *handCursor );
            break;
        case CurAction3d_Nothing:
        default:
            setCursor( *defCursor );
            break;
    }
}

void View::onLButtonDown( ButtonState nFlags, const QPoint point )
{
    //  save the current mouse coordinate in min
    myXmin = point.x();
    myYmin = point.y();
    myXmax = point.x();
    myYmax = point.y();

    if ( nFlags & CASCADESHORTCUTKEY )
    {
        myCurrentMode = CurAction3d_DynamicZooming;
    }
    else
    {
        switch ( myCurrentMode )
        {
            case CurAction3d_Nothing:
                if ( nFlags & MULTISELECTIONKEY )
	                MultiDragEvent( myXmax, myYmax, -1 );
                else
	                DragEvent( myXmax, myYmax, -1 );
                break;
            case CurAction3d_DynamicZooming:
                break;
            case CurAction3d_WindowZooming:
                break;
            case CurAction3d_DynamicPanning:
                break;
            case CurAction3d_GlobalPanning:
                break;
            case SketcherAction: 
				myView->Convert(myXmin,myYmin, my_v3dX,my_v3dY,my_v3dZ);
				myView->Proj(projVx,projVy,projVz);
				mySketcher->OnMouseInputEvent(my_v3dX,my_v3dY,my_v3dZ,projVx,projVy,projVz);

                break;
            case CurAction3d_DynamicRotation:
                if ( !myDegenerateModeIsOn )
	                myView->SetDegenerateModeOn();
                myView->StartRotation( point.x(), point.y() );
                break;
            default:
	            Standard_Failure::Raise( "incompatible Current Mode" );
                break;
        }
    }
    activateCursor( myCurrentMode );
}

void View::onMButtonDown( ButtonState nFlags, const QPoint point )
{
    if ( nFlags & CASCADESHORTCUTKEY )
        myCurrentMode = CurAction3d_DynamicPanning;
    activateCursor( myCurrentMode );
}

void View::onRButtonDown( ButtonState nFlags, const QPoint point )
{
    if ( nFlags & CASCADESHORTCUTKEY )
    {
        if ( !myDegenerateModeIsOn )
            myView->SetDegenerateModeOn();
        myCurrentMode = CurAction3d_DynamicRotation;
        myView->StartRotation( point.x(), point.y() );
    }
	else if(myCurrentMode == SketcherAction)
		mySketcher->OnCancel();

    else
    {
        Popup( point.x(), point.y() );
    }
    activateCursor( myCurrentMode );
}

void View::onLButtonUp( ButtonState nFlags, const QPoint point )
{
    switch( myCurrentMode )
    {
        case CurAction3d_Nothing:
            if ( point.x() == myXmin && point.y() == myYmin )
            {
	            // no offset between down and up --> selectEvent
	            myXmax = point.x();
	            myYmax = point.y();
	            if ( nFlags & MULTISELECTIONKEY )
	                MultiInputEvent( point.x(), point.y() );
                else
	                InputEvent( point.x(), point.y() );
            }
            else
            {
	            DrawRectangle( myXmin, myYmin, myXmax, myYmax, Standard_False );
	            myXmax = point.x();
	            myYmax = point.y();
	            if ( nFlags & MULTISELECTIONKEY )
	                MultiDragEvent( point.x(), point.y(), 1 );
                else
	                DragEvent( point.x(), point.y(), 1 );
            }
            break;
        case CurAction3d_DynamicZooming:
            myCurrentMode = CurAction3d_Nothing;
            noActiveActions();
            break;
        case CurAction3d_WindowZooming:
            DrawRectangle( myXmin, myYmin, myXmax, myYmax, Standard_False );//,LongDash);
            myXmax = point.x();
            myYmax = point.y();
            if ( (abs( myXmin - myXmax ) > ValZWMin ) ||
                 (abs( myYmin - myYmax ) > ValZWMin ) )
	            myView->WindowFitAll( myXmin, myYmin, myXmax, myYmax );
            myCurrentMode = CurAction3d_Nothing;
            noActiveActions();
            break;
        case CurAction3d_DynamicPanning:
            myCurrentMode = CurAction3d_Nothing;
            noActiveActions();
            break;
        case CurAction3d_GlobalPanning :
            myView->Place( point.x(), point.y(), myCurZoom );
            myCurrentMode = CurAction3d_Nothing;
            noActiveActions();
            break;
        case CurAction3d_DynamicRotation:
            myCurrentMode = CurAction3d_Nothing;
            noActiveActions();
            break;
        case SketcherAction: 
            break;
        default:
	        Standard_Failure::Raise(" incompatible Current Mode ");
            break;
    }
    activateCursor( myCurrentMode );
    ApplicationWindow::getApplication()->onSelectionChanged();
}

void View::onMButtonUp( ButtonState nFlags, const QPoint point )
{
    myCurrentMode = CurAction3d_Nothing;
    activateCursor( myCurrentMode );
}

void View::onRButtonUp( ButtonState nFlags, const QPoint point )
{
    if ( myCurrentMode == CurAction3d_Nothing )
        Popup( point.x(), point.y() );
	else if (myCurrentMode == SketcherAction)
	{
		myCurrentMode = CurAction3d_Nothing;
		noActiveActions();
	}
    else
    {
        QApplication::setOverrideCursor( Qt::waitCursor );
        // reset tyhe good Degenerated mode according to the strored one
        //   --> dynamic rotation may have change it
        if ( !myDegenerateModeIsOn )
        {
            myView->SetDegenerateModeOff();
            myDegenerateModeIsOn = Standard_False;
        }
        else
        {
            myView->SetDegenerateModeOn();
            myDegenerateModeIsOn = Standard_True;
        }
        QApplication::restoreOverrideCursor();
        myCurrentMode = CurAction3d_Nothing;
    }
    activateCursor( myCurrentMode );
}

void View::onMouseMove( ButtonState nFlags, const QPoint point )
{
    if ( nFlags & LeftButton || nFlags & RightButton || nFlags & MidButton )
    {
    switch ( myCurrentMode )
    {
        case CurAction3d_Nothing:
	        myXmax = point.x();
	        myYmax = point.y();
	        DrawRectangle( myXmin, myYmin, myXmax, myYmax, Standard_False );
	        if ( nFlags & MULTISELECTIONKEY )
	            MultiDragEvent( myXmax, myYmax, 0 );
            else
	            DragEvent( myXmax, myYmax, 0 );
	        DrawRectangle( myXmin, myYmin, myXmax, myYmax, Standard_True );
            break;
        case CurAction3d_DynamicZooming:
	        myView->Zoom( myXmax, myYmax, point.x(), point.y() );
            myXmax = point.x();
            myYmax = point.y();
	        break;
        case CurAction3d_WindowZooming:
	        myXmax = point.x();
            myYmax = point.y();
	        DrawRectangle( myXmin, myYmin, myXmax, myYmax, Standard_False );
	        DrawRectangle( myXmin, myYmin, myXmax, myYmax, Standard_True );
	        break;
        case CurAction3d_DynamicPanning:
	        myView->Pan( point.x() - myXmax, myYmax - point.y() );
	        myXmax = point.x();
	        myYmax = point.y();
	        break;
        case CurAction3d_GlobalPanning:
            break;
        case CurAction3d_DynamicRotation:
	        myView->Rotation( point.x(), point.y() );
	        myView->Redraw();
            break;
        case SketcherAction: 
            break;
        default:
	        Standard_Failure::Raise( "incompatible Current Mode" );
            break;
    }
    }
    else
    {
        myXmax = point.x();
        myYmax = point.y();
		if (myCurrentMode == SketcherAction)
		{
			myView->Convert(myXmax,myYmax, my_v3dX,my_v3dY,my_v3dZ);
			myView->Proj(projVx,projVy,projVz);
			mySketcher->OnMouseMoveEvent(my_v3dX,my_v3dY,my_v3dZ,projVx,projVy,projVz);
		}
	    if ( nFlags & MULTISELECTIONKEY )
	        MultiMoveEvent( point.x(), point.y() );
        else
	        MoveEvent( point.x(), point.y() );
    }
}

void View::DragEvent( const int x, const int y, const int TheState )
{
    // TheState == -1  button down
    // TheState ==  0  move
    // TheState ==  1  button up

    static Standard_Integer theButtonDownX = 0;
    static Standard_Integer theButtonDownY = 0;

    if ( TheState == -1 )
    {
        theButtonDownX = x;
        theButtonDownY = y;
    }

    if ( TheState == 0 )
    {
        myContext->Select( theButtonDownX, theButtonDownY, x, y, myView );
        emit selectionChanged();
    }
}

void View::InputEvent( const int x, const int y )
{
    myContext->Select();
    emit selectionChanged();
}

void View::MoveEvent( const int x, const int y )
{
    myContext-> MoveTo( x, y, myView );
}

void View::MultiMoveEvent( const int x, const int y )
{
    myContext->MoveTo( x, y, myView );
}

void View::MultiDragEvent( const int x, const int y, const int TheState )
{
    static Standard_Integer theButtonDownX = 0;
    static Standard_Integer theButtonDownY = 0;

    if ( TheState == -1 )
    {
        theButtonDownX = x;
        theButtonDownY = y;
    }
    if ( TheState == 0 )
    {
        myContext->ShiftSelect( theButtonDownX, theButtonDownY, x, y, myView );
        emit selectionChanged();
    }
}

void View::MultiInputEvent( const int x, const int y )
{
  myContext->ShiftSelect();
  emit selectionChanged();
}

void View::Popup( const int x, const int y )
{
/*    ApplicationWindow* stApp = ApplicationWindow::getApplication();
    QWorkspace* ws = ApplicationWindow::getWorkspace();
    QWidget* w = ws->activeWindow();
    if ( myContext->NbSelected() )
    {
        QList<QAction>* aList = stApp->getToolActions();
        QPopupMenu* myToolMenu = new QPopupMenu( 0 );
        aList->at( ApplicationWindow::ToolWireframeId )->addTo( myToolMenu );
        aList->at( ApplicationWindow::ToolShadingId )->addTo( myToolMenu );
        aList->at( ApplicationWindow::ToolColorId )->addTo( myToolMenu );

        QPopupMenu* myMaterMenu = new QPopupMenu( myToolMenu );

        QList<QAction>* aMeterActions = ApplicationWindow::getApplication()->getMaterialActions();
        for ( QAction* a = aMeterActions->first(); a; a = aMeterActions->next() )
            a->addTo( myMaterMenu );

        QString dir = ApplicationWindow::getResourceDir() + QString( "/" );
        myToolMenu->insertItem(QPixmap( dir+tr("ICON_TOOL_MATER")),tr("MNU_MATER"), myMaterMenu);

        aList->at( ApplicationWindow::ToolTransparencyId )->addTo( myToolMenu );
        aList->at( ApplicationWindow::ToolDeleteId )->addTo( myToolMenu );
        myToolMenu->exec( QCursor::pos() );
        delete myToolMenu;
    }
    else
    {
        QPopupMenu* myBackMenu = new QPopupMenu( 0 );
        QAction* a = new QAction( tr("TBR_CH_BACK"), tr("MNU_CH_BACK"), 0, this, "change background" );
        connect( a,SIGNAL( activated() ), this, SLOT( onBackground() ) );
        a->addTo( myBackMenu );
        myBackMenu->exec( QCursor::pos() );
        delete myBackMenu;
    }
 //   if ( w )
		////////////////////////////////////////ERRRRRRRRRRRRRRROOOOOOOOOOOOOOOOOOOOOOOORRRRRRRRRRRRRRRRRRRRR
		//////////////////////////////////////////////////////
   //     w->setFocus();
	*/
}

void View::DrawRectangle(const int MinX, const int MinY,
			 const int MaxX, const int MaxY, const bool Draw)
{
    QPainter thePainter(this);
    thePainter.setRasterOp(Qt::XorROP);
    thePainter.setPen(Qt::white);

    static Standard_Integer StoredMinX, StoredMaxX, StoredMinY, StoredMaxY;
    static Standard_Boolean m_IsVisible;

    QRect aRect;
    if ( m_IsVisible && !Draw) // move or up  : erase at the old position
    {
        aRect.setRect( StoredMinX, StoredMinY, abs(StoredMaxX-StoredMinX), abs(StoredMaxY-StoredMinY));
        thePainter.drawRect(aRect);
        m_IsVisible = false;
    }
    StoredMinX = (MinX < MaxX) ? MinX: MaxX ;
    StoredMinY = (MinY < MaxY) ? MinY: MaxY ;
    StoredMaxX = (MinX > MaxX) ? MinX: MaxX ;
    StoredMaxY = (MinY > MaxY) ? MinY: MaxY ;

    if (Draw) // move : draw
    {
        aRect.setRect( StoredMinX, StoredMinY, abs(StoredMaxX-StoredMinX), abs(StoredMaxY-StoredMinY));
        thePainter.drawRect(aRect);
        m_IsVisible = true;
    }
}

void View::noActiveActions()
{
    for ( int i = ViewFitAllId; i <= MyTrimCurveAction ; i++ )
    {
        QAction* anAction = myViewActions->at( i );
        if( ( anAction == myViewActions->at( ViewFitAreaId ) ) ||
            ( anAction == myViewActions->at( ViewZoomId ) ) ||
            ( anAction == myViewActions->at( ViewPanId  ) ) ||
            ( anAction == myViewActions->at( ViewGlobalPanId ) ) ||
            ( anAction == myViewActions->at( ViewRotationId  ) ) ||
             ( anAction == myViewActions->at( MyInputPointAction ) ) ||
             ( anAction == myViewActions->at( MyInputLineAction ) ) ||
             ( anAction == myViewActions->at( MyInputCircleAction ) ) ||
             ( anAction == myViewActions->at( MyInputCircle3PAction ) ) ||
             ( anAction == myViewActions->at( MyInputCircle2PTanAction ) ) ||
             ( anAction == myViewActions->at( MyInputCircleP2TanAction ) ) ||
             ( anAction == myViewActions->at( MyInputCircle3TanAction ) ) ||
             ( anAction == myViewActions->at( MyInputArc3PAction ) )  || 
             ( anAction == myViewActions->at( MyInputArcCenter2PAction ) )  || 
             ( anAction == myViewActions->at( MyInputBezierCurveAction ) )  ||
             ( anAction == myViewActions->at( MyTrimCurveAction ) )  )
        {
            setCursor( *defCursor );
            anAction->setToggleAction( true );
            anAction->setOn( false );
        }
    }
}

void View::onBackground()
{
    QColor aColor ;
    Standard_Real R1;
    Standard_Real G1;
    Standard_Real B1;
    myView->BackgroundColor(Quantity_TOC_RGB,R1,G1,B1);
    aColor.setRgb(R1*255,G1*255,B1*255);

    QColor aRetColor = QColorDialog::getColor(aColor);

    if( aRetColor.isValid() )
    {
        R1 = aRetColor.red()/255.;
        G1 = aRetColor.green()/255.;
        B1 = aRetColor.blue()/255.;
        myView->SetBackgroundColor(Quantity_TOC_RGB,R1,G1,B1);
    }
    myView->Redraw();
}

bool View::dump(Standard_CString theFile)
{
  myView->Redraw();
  return myView->Dump(theFile);
}
