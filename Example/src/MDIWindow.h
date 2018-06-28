#ifndef MDIWINDOW_H
#define MDIWINDOW_H

#include <qmainwindow.h>

class Document;
class View;

class MDIWindow: public QMainWindow
//class MDIWindow: public QWidget
{
    Q_OBJECT

public:
    MDIWindow( Document* aDocument, QWidget* parent, const char* name, int wflags );
    ~MDIWindow();
	Document* getDocument();
	void fitAll();
    virtual QSize              sizeHint() const;

signals:
    void                       selectionChanged();
    void message(const QString&, int );
	void sendCloseView(MDIWindow* theView);

public slots:
  void closeEvent(QCloseEvent* e);        
  void onWindowActivated ();
  void dump();

protected:
  void createViewActions();

private:
    Document* myDocument;
    View* myView;
};

#endif
