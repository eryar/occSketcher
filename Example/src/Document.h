#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "MDIWindow.h"

#include <qobject.h>
#include <qlist.h>

#include <AIS_InteractiveContext.hxx>
#include <V3d_Viewer.hxx>

class ApplicationWindow;

class Document : public QObject
{
	Q_OBJECT

public:
	Document( const int, ApplicationWindow*, const char* = 0 );
	~Document();

	ApplicationWindow*             getApplication();
	Handle(AIS_InteractiveContext) getContext();
	void                           removeView( MDIWindow* );
    void                           removeViews();
	int                            countOfWindow();
	void                           fitAll();

signals:
    void                           selectionChanged();
	void                           sendCloseDocument( Document* );

public slots:
	void                           onCloseView( MDIWindow* );
	void                           onCreateNewView();
	void                           onWireframe();
	void                           onShading();
	void                           onColor();
	void                           onMaterial();
    void                           onMaterial( int );
	void                           onTransparency();
	void                           onTransparency( int );
	void                           onDelete();

private:
    Handle(V3d_Viewer)             Viewer( const Standard_CString aDisplay,
			                               const Standard_ExtString aName,
			                               const Standard_CString aDomain,
			                               const Standard_Real ViewSize,
			                               const V3d_TypeOfOrientation ViewProj,
			                               const Standard_Boolean ComputedMode,
			                               const Standard_Boolean aDefaultComputedMode );

private:
	ApplicationWindow*             myApp;
	QList<MDIWindow>               myViews;
	Handle(V3d_Viewer)             myViewer;
	Handle(AIS_InteractiveContext) myContext;
	int                            myIndex;
    int                            myNbViews;
};

#endif
