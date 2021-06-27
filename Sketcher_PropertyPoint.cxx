/**
* \file Sketcher_PropertyPoint.cxx
* \brief Implementation file for the class Sketcher_PropertyPoint
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_PropertyPoint.cxx">Sergei Maslov</a>
*/
#include "Sketcher_PropertyPoint.hxx"

/**
 * \fn Sketcher_PropertyPoint( QWidget* parent,  const char* name, WFlags fl )
 * \brief Constructs a Sketcher_PropertyPoint which is a child of 'parent', with the name 'name' and widget flags set to 'f' 
 * \param parent QWidget*
 * \param name const char*
 * \param fl WFlags
 */
Sketcher_PropertyPoint::Sketcher_PropertyPoint( QWidget* parent,  const char* name)
    : Sketcher_Property( parent, name)
{
    if ( !name )
	setWindowTitle( "Property Points" );
    //setCaption( tr( "PointsProperties" ) );

    TextLabelPoint1->setText( tr( "Point " ) );
	isPointWindow = true;
}

/**
 * \fn ~Sketcher_PropertyPoint() 
 * \brief destructor
 */
Sketcher_PropertyPoint::~Sketcher_PropertyPoint()
{
    // no need to delete child widgets, Qt does it all for us
}


/**
 * \fn SetGeometry()
 * \brief show object geometry in dialog window
 * \return void 
 */
void Sketcher_PropertyPoint::SetGeometry()
{
	curGeom2d_Point = Handle(Geom2d_CartesianPoint)::DownCast(mySObject->GetGeometry());
	firstPnt2d = curGeom2d_Point->Pnt2d();

	SetCoord(LineEditPoint1,firstPnt2d);
}


/**
 * \fn CheckGeometry()
 * \brief check geometry for change
 * \return bool 
 */
bool Sketcher_PropertyPoint::CheckGeometry()
{
	return CheckCoord(LineEditPoint1,tempPnt2d);
}

/**
 * \fn GetGeometry()
 * \brief create new object
 * \return bool 
 */
bool Sketcher_PropertyPoint::GetGeometry()
{
	if(!firstPnt2d.IsEqual(tempPnt2d,1.0e-6))
	{
		firstPnt2d = tempPnt2d;

		curGeom2d_Point->SetPnt2d(firstPnt2d);
		Handle (Geom_CartesianPoint) newGeom_Point = new Geom_CartesianPoint(ElCLib::To3d(myCoordinateSystem.Ax2(),firstPnt2d));
		Handle(AIS_Point) newAIS_Point = new AIS_Point(newGeom_Point);

		myContext->Remove(myAIS_Object, true);
		myAIS_Object = newAIS_Point;
		return true;
	}
	return false;
}

