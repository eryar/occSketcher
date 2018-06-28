#ifndef MATERIAL_H
#define MATERIAL_H

#include <qdialog.h>
#include <qlist.h>
#include <qpushbutton.h>

#include <AIS_InteractiveContext.hxx>

class DialogMaterial : public QDialog
{
	Q_OBJECT
public:
	DialogMaterial (QWidget * parent=0, const char * name=0, bool modal=true, WFlags f=0 );
	~DialogMaterial();

signals:
	void sendMaterialChanged(int);

public slots:
	void updateButtons(bool isOn);

private:
	QList<QPushButton> myButtons;
};

#endif
