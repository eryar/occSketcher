#include "Transparency.h"

#include <qlayout.h>
#include <qspinbox.h>

DialogTransparency::DialogTransparency( QWidget* parent, const char* name, bool modal, WFlags f )
: QDialog( parent, name, modal, f )
{
    QHBoxLayout* base = new QHBoxLayout( this, 3, 3 );
	QSpinBox* aSpin = new QSpinBox( 0, 10, 1, this );
    base->addWidget( aSpin );
	connect( aSpin, SIGNAL( valueChanged( int ) ), this, SIGNAL( sendTransparencyChanged( int ) ) );
}

DialogTransparency::~DialogTransparency()
{
}
