#ifndef APPLICATION_H
#define APPLICATION_H

#include "Document.h"

#include <qmainwindow.h>
#include <qaction.h>
#include <qtoolbar.h>
#include <qmenu.h>
#include <qworkspace.h>
#include <qlist.h>

class ApplicationWindow: public QMainWindow
{
    Q_OBJECT

public:
	enum { FileNewId, FileCloseId, FileQuitId, ViewToolId, ViewStatusId, HelpAboutId };
    enum { ToolWireframeId, ToolShadingId, ToolColorId, ToolMaterialId, ToolTransparencyId, ToolDeleteId };
	enum { FileImportBREPId, FileExportBREPId, FileImportCSFDBId, FileExportCSFDBId,
	       FileImportIGESId, FileExportIGESId, FileImportSTEPId, FileExportSTEPId,
	       FileExportSTLId, FileExportVRMLId };
	
    ApplicationWindow();
    ~ApplicationWindow();

	static QWorkspace*              getWorkspace();
	static ApplicationWindow*       getApplication();
	static QString                  getResourceDir();

	void                            updateFileActions();
    QList<QAction>*                 getToolActions();
    QList<QAction>*                 getMaterialActions();

public slots:
    Document*                       onNewDoc();
    void                            onCloseWindow();
	void                            onCloseDocument( Document* theDoc );
	void                            onViewToolBar();
	void                            onViewStatusBar();
	void                            onAbout();
	void                            onImport();
	void                            onExport();
	void                            onExportImage();
    void                            onToolAction();
    void                            onSelectionChanged();
	void                            onCreateNewView();
    void                            onSetMaterial( int );
    void                            onWindowActivated ( QWidget * w );
	void                            windowsMenuAboutToShow();
    void                            windowsMenuActivated( int id );

protected:
    virtual void                    resizeEvent( QResizeEvent* );

private:
	void                            createStandardOperations();
	void                            createCasCadeOperations();
	void                            createTranslatePopups();
	void                            createWindowPopup();
    bool                            translate( const int, const bool );
    int                             translationFormat( const QAction* );

private:
    int                             myNbDocuments;
	bool                            myIsDocuments;

	QList<QAction*>                  myStdActions;
    QList<QAction>                  myToolActions;
    QList<QAction>                  myMaterialActions;
	QList<QAction>                  myCasCadeTranslateActions;
    QList<Document>                 myDocuments;

	QToolBar*                       myStdToolBar;
	QToolBar*                       myCasCadeBar;
	QMenu*                     myFilePopup;
	QMenu*                     myWindowPopup;
	QMenu*                     myImportPopup;
	QMenu*                     myExportPopup;
};

#endif
