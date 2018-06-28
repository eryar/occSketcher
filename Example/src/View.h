#ifndef VIEW_H
#define VIEW_H

#include "Sketcher.hxx"
#include "Sketcher_ObjectTypeOfMethod.hxx"
#include "Sketcher_ObjectGeometryType.hxx"


#include <qwidget.h>
#include <qaction.h>
#include <qlist.h>

#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>


#include <Geom_CartesianPoint.hxx>
#include <AIS_PlaneTrihedron.hxx>
#include <AIS_Axis.hxx>
#include <AIS_Line.hxx>
#include <AIS_Point.hxx>
#include <AIS_Circle.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Line.hxx>
#include <Geom_Plane.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>

#define GRID1 Aspect_GT_Rectangular, Aspect_GDM_Lines
#define GRID2 Aspect_GT_Circular, Aspect_GDM_Points

#include <qtoolbar.h>
#include <qcheckbox.h>
#include <qcombobox.h>


class TopoDS_Shape;

class View: public QWidget
{
    Q_OBJECT
    enum CurrentAction3d { CurAction3d_Nothing, CurAction3d_DynamicZooming,
			               CurAction3d_WindowZooming, CurAction3d_DynamicPanning,
						   CurAction3d_GlobalPanning, CurAction3d_DynamicRotation,
/*********************MAS INSERT*******************************/
						   SketcherAction};

public:
    enum ViewAction { ViewFitAllId, ViewFitAreaId, ViewZoomId, ViewPanId, ViewGlobalPanId,
                      ViewFrontId, ViewBackId, ViewTopId, ViewBottomId, ViewLeftId, ViewRightId,
                      ViewAxoId, ViewRotationId, ViewResetId, ViewHlrOffId, ViewHlrOnId,
/*********************MAS INSERT*******************************/
					  MyEraseActionId,MyDeleteActionId,MyPropertyActionId,
					  MyRedrawActionId,MyChangePlaneAction,MyGridActionId,
					  MyInputPointAction,MyInputLineAction,MyInputCircleAction,
					  MyInputCircle3PAction,MyInputCircle2PTanAction,MyInputCircleP2TanAction,MyInputCircle3TanAction,
					  MyInputArc3PAction,MyInputArcCenter2PAction,
					  MyInputBezierCurveAction,MyTrimCurveAction};


    View( Handle(AIS_InteractiveContext) theContext,
          QWidget* parent, const char* name, int wflags );
    ~View();

    void                          init();
    bool                          dump( Standard_CString theFile );
    QList<QAction>*               getViewActions();
    void                          noActiveActions();
    bool                          isShadingMode();

    static QString                GetMessages( int type,TopAbs_ShapeEnum aSubShapeType,
                                               TopAbs_ShapeEnum aShapeType );
    static QString                GetShapeType( TopAbs_ShapeEnum aShapeType );

    Standard_EXPORT static void   OnButtonuseraction( int ExerciceSTEP,
						                              Handle(AIS_InteractiveContext)& );
    Standard_EXPORT static void   DoSelection( int Id,
                                               Handle(AIS_InteractiveContext)& );
    Standard_EXPORT static void   OnSetSelectionMode( Handle(AIS_InteractiveContext)&,
                                                      Standard_Integer&,
						                              TopAbs_ShapeEnum& SelectionMode,
						                              Standard_Boolean& );
signals:
    void                          selectionChanged();

public slots:
    void                          fitAll();
    void                          fitArea();
    void                          zoom();
    void                          pan();
    void                          globalPan();
    void                          front();
    void                          back();
    void                          top();
    void                          bottom();
    void                          left();
    void                          right();
    void                          axo();
    void                          rotation();
    void                          reset();
    void                          hlrOn();
    void                          hlrOff();
    void                          updateToggled( bool );
    void                          onBackground();
/*********************MAS INSERT*******************************/
	void						  onaddComboBox(QToolBar* tbr);
	void						  updateSnap(int pos);
	void						  updateColor(int pos);
	void						  updateType(int pos);
	void						  updateStyle(int pos);
	void						  updateWidth(int pos);
	void						  setPolylineMode(bool );

    void						  onErase();
    void						  onDeleteSelected();
    void						  onProperty();
    void						  onRedrawAll();
    void						  onChangePlane();
    void						  onGrid();
	void						  onInputPoints();
	void						  onInputLines();
	void						  onInputCircles();
	void						  onInputCircles3P();
	void						  onInputCircles2PTan();
	void						  onInputCirclesP2Tan();
	void						  onInputCircles3Tan();
	void						  onInputArc3P();
	void						  onInputArcCenter2P();
	void						  onInputBezierCurve();
	void						  onTrimCurve();

protected:
    virtual void                  paintEvent( QPaintEvent* );
    virtual void                  resizeEvent( QResizeEvent* );
    virtual void                  mousePressEvent( QMouseEvent* );
    virtual void                  mouseReleaseEvent(QMouseEvent* );
    virtual void                  mouseMoveEvent( QMouseEvent* );

private:
    void                          initCursors();
    void                          initViewActions();
    void                          onLButtonDown( ButtonState nFlags, const QPoint point );
    void                          onMButtonDown( ButtonState nFlags, const QPoint point );
    void                          onRButtonDown( ButtonState nFlags, const QPoint point );
    void                          onLButtonUp( ButtonState nFlags, const QPoint point );
    void                          onMButtonUp( ButtonState nFlags, const QPoint point );
    void                          onRButtonUp( ButtonState nFlags, const QPoint point );
    void                          onMouseMove( ButtonState nFlags, const QPoint point );
    void                          DragEvent( const int x, const int y, const int TheState );
    void                          InputEvent( const int x, const int y );
    void                          MoveEvent( const int x, const int y );
    void                          MultiMoveEvent( const int x, const int y );
    void                          MultiDragEvent( const int x, const int y, const int TheState );
    void                          MultiInputEvent( const int x, const int y );
    void                          Popup( const int x, const int y );
    void                          DrawRectangle( const int MinX, const int MinY,
                                                 const int MaxX, const int MaxY, const bool Draw );
    void                          activateCursor( const CurrentAction3d );


private:
    bool                            myFirst;
    Handle(V3d_View)                myView;
    Handle(AIS_InteractiveContext)  myContext;
    CurrentAction3d                 myCurrentMode;
    Standard_Integer                myXmin;
    Standard_Integer                myYmin;
    Standard_Integer                myXmax;
    Standard_Integer                myYmax;
    Quantity_Factor                 myCurZoom;
    Standard_Boolean                myDegenerateModeIsOn;
    QList<QAction>*                 myViewActions;
/*********************MAS INSERT*******************************/
	QCheckBox*						PolylineModeCheck;
	bool							GRIDCounter;
    Sketcher*						mySketcher;
	V3d_Coordinate					my_v3dX,my_v3dY,my_v3dZ;
	Quantity_Parameter				projVx,projVy,projVz;
};

#endif
