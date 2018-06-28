#include "Material.h"

#include <qpushbutton.h>
#include <qlayout.h>
#include <qsignalmapper.h>

#include <Graphic3d_NameOfMaterial.hxx>

DialogMaterial::DialogMaterial(QWidget * parent,
			       const char* name, bool modal, WFlags f )
: QDialog(parent, name, modal, f)
{
	QPushButton* b;
    QVBoxLayout* vbl = new QVBoxLayout(this,8);
    vbl->setAutoAdd(TRUE);

	QSignalMapper *sm = new QSignalMapper(this);
	connect(sm,SIGNAL(mapped(int)),this,SIGNAL(sendMaterialChanged(int)));

	b = new QPushButton(tr("BTN_PLASTER"),this);
    sm->setMapping(b,(int) Graphic3d_NOM_PLASTER);
    connect(b,SIGNAL(clicked()),sm,SLOT(map()));	
	b->setToggleButton (true);
	connect(b,SIGNAL(toggled(bool)),this,SLOT(updateButtons(bool)));
	myButtons.append(b);

	b = new QPushButton(tr("BTN_BRASS"),this);
    sm->setMapping(b,(int) Graphic3d_NOM_BRASS);
    connect(b,SIGNAL(clicked()),sm,SLOT(map()));	
	b->setToggleButton (true);
	connect(b,SIGNAL(toggled(bool)),this,SLOT(updateButtons(bool)));
	myButtons.append(b);

	b = new QPushButton(tr("BTN_BRONZE"),this);
    sm->setMapping(b,(int) Graphic3d_NOM_BRONZE);
    connect(b,SIGNAL(clicked()),sm,SLOT(map()));	
	b->setToggleButton (true);
	connect(b,SIGNAL(toggled(bool)),this,SLOT(updateButtons(bool)));
	myButtons.append(b);

	b = new QPushButton(tr("BTN_COPPER"),this);
    sm->setMapping(b,(int) Graphic3d_NOM_COPPER);
    connect(b,SIGNAL(clicked()),sm,SLOT(map()));	
	b->setToggleButton (true);
	connect(b,SIGNAL(toggled(bool)),this,SLOT(updateButtons(bool)));
	myButtons.append(b);

	b = new QPushButton(tr("BTN_GOLD"),this);
    sm->setMapping(b,(int) Graphic3d_NOM_GOLD);
    connect(b,SIGNAL(clicked()),sm,SLOT(map()));	
	b->setToggleButton (true);
	connect(b,SIGNAL(toggled(bool)),this,SLOT(updateButtons(bool)));
	myButtons.append(b);

	b = new QPushButton(tr("BTN_PEWTER"),this);
    sm->setMapping(b,(int) Graphic3d_NOM_PEWTER);
    connect(b,SIGNAL(clicked()),sm,SLOT(map()));	
	b->setToggleButton (true);
	connect(b,SIGNAL(toggled(bool)),this,SLOT(updateButtons(bool)));
	myButtons.append(b);

	b = new QPushButton(tr("BTN_PLASTIC"),this);
    sm->setMapping(b,(int) Graphic3d_NOM_PLASTIC);
    connect(b,SIGNAL(clicked()),sm,SLOT(map()));	
	b->setToggleButton (true);
	connect(b,SIGNAL(toggled(bool)),this,SLOT(updateButtons(bool)));
	myButtons.append(b);

	b = new QPushButton(tr("BTN_SILVER"),this);
    sm->setMapping(b,(int) Graphic3d_NOM_SILVER);
    connect(b,SIGNAL(clicked()),sm,SLOT(map()));	
	b->setToggleButton (true);
	connect(b,SIGNAL(toggled(bool)),this,SLOT(updateButtons(bool)));
	myButtons.append(b);
}

DialogMaterial::~DialogMaterial()
{
}

void DialogMaterial::updateButtons(bool isOn)
{
	if(!isOn)
		return;
	
	QPushButton*sentBy = (QPushButton*) sender();

	for(QPushButton* b = myButtons.first(); b ; b = myButtons.next())
		if(b != sentBy) {
			b->setEnabled(true);
			b->setOn(false);
		} else {
			b->setEnabled(false);
		}
}
