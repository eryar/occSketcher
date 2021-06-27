/**
* \file Sketcher_PropertyCircle.cxx
* \brief Implementation file for the class Sketcher_PropertyCircle
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_PropertyCircle.cxx">Sergei Maslov</a>
*/
#include "Sketcher_PropertyCircle.hxx"

/**
 * \fn Sketcher_PropertyCircle( QWidget* parent,  const char* name, WFlags fl )
 * \brief Constructs a Sketcher_PropertyCircle which is a child of 'parent', with the name 'name' and widget flags set to 'f' 
 * \param parent QWidget*
 * \param name const char*
 * \param fl WFlags
 */
Sketcher_PropertyCircle::Sketcher_PropertyCircle( QWidget* parent,  const char* name)
    : Sketcher_Property( parent, name)
{
    if ( !name )
	setWindowTitle( "Property of Circle" );
    //setCaption( tr( "CircleProperties" ) );

////////////////////////////////////////////////////////////////////////////////

    TextLabelPoint1->setText( tr( "Center" ) );
    TextLabelRadius = new QLabel( "TextLabelRadius", GroupBoxGP);
    TextLabelRadius->setText( tr( "Radius" ) );
    LineEditRadius = new QLineEdit("LineEditRadius", GroupBoxGP);

    GroupBoxGPLayout->addWidget( TextLabelRadius, 1, 0 );
    GroupBoxGPLayout->addWidget( LineEditRadius, 1, 1 );

///////////////////////////////////////////////////////////////////////////////

	Init();

////////////////////////////////////////////////////////////////////////////////////

}

/**
 * \fn ~Sketcher_PropertyCircle() 
 * \brief destructor
 */
Sketcher_PropertyCircle::~Sketcher_PropertyCircle()
{
    // no need to delete child widgets, Qt does it all for us
}


/**
 * \fn SetGeometry()
 * \brief show object geometry in dialog window
 * \return void 
 */
void Sketcher_PropertyCircle::SetGeometry()
{
	curGeom2d_Circle = Handle(Geom2d_Circle)::DownCast(mySObject->GetGeometry());

	firstPnt2d = curGeom2d_Circle->Location();
	myRadius = curGeom2d_Circle->Radius();

	SetCoord(LineEditPoint1,firstPnt2d);
	LineEditRadius->setText(QString::number(myRadius));
}


/**
 * \fn CheckGeometry()
 * \brief check geometry for change
 * \return bool 
 */
bool Sketcher_PropertyCircle::CheckGeometry()
{
	if (CheckCoord(LineEditPoint1,tempPnt2d ))
	{
		NumName = LineEditRadius->text();
		tempRadius = NumName.toDouble();
		if(NumName.contains(NumberExpr) == 0 && !NumName.isNull() && tempRadius > 0) 
			return true;
		else LineEditRadius->selectAll();
	}
	return false;
}

/**
 * \fn GetGeometry()
 * \brief create new object
 * \return bool 
 */
bool Sketcher_PropertyCircle::GetGeometry()
{
	if(!firstPnt2d.IsEqual(tempPnt2d,1.0e-6) || myRadius !=tempRadius)
	{
		firstPnt2d = tempPnt2d;
		myRadius = tempRadius;

		curGeom2d_Circle->SetLocation(firstPnt2d);
		curGeom2d_Circle->SetRadius(myRadius);
		Handle(Geom_Circle)	newGeom_Circle = new Geom_Circle(myCoordinateSystem.Ax2(),myRadius);
		newGeom_Circle->SetLocation(ElCLib::To3d(myCoordinateSystem.Ax2(),firstPnt2d));
		Handle(AIS_Circle) newAIS_Circle = new AIS_Circle(newGeom_Circle);

		myContext->Remove(myAIS_Object, true);
		myAIS_Object = newAIS_Circle;
		return true;
	}
	return false;
}
