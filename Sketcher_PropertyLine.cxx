/**
* \file Sketcher_PropertyLine.cxx
* \brief Implementation file for the class Sketcher_PropertyLine
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_PropertyLine.cxx">Sergei Maslov</a>
*/
#include "Sketcher_PropertyLine.hxx"

/**
 * \fn Sketcher_PropertyLine( QWidget* parent,  const char* name, WFlags fl )
 * \brief Constructs a Sketcher_PropertyLine which is a child of 'parent', with the name 'name' and widget flags set to 'f' 
 * \param parent QWidget*
 * \param name const char*
 * \param fl WFlags
 */
Sketcher_PropertyLine::Sketcher_PropertyLine( QWidget* parent,  const char* name)
    : Sketcher_Property( parent, name)
{
    if ( !name )
    setWindowTitle( "Property of Line" );
    //setCaption( tr( "LinesProperties" ) );

////////////////////////////////////////////////////////////////////////////////

    TextLabelPoint1->setText( tr( "Point 1" ) );
    TextLabelPoint2 = new QLabel( "TextLabelPoint2", GroupBoxGP);
    TextLabelPoint2->setText( tr( "Point 2" ) );
    LineEditPoint2 = new QLineEdit( "LineEditPoint2", GroupBoxGP);

    GroupBoxGPLayout->addWidget( TextLabelPoint2, 1, 0 );
    GroupBoxGPLayout->addWidget( LineEditPoint2, 1, 1 );

///////////////////////////////////////////////////////////////////////////////
	Init();

    TextLabelLength = new QLabel("TextLabelLength", GroupBoxAttributes);
    TextLabelLength->setText( tr( "Length" ) );
    LineEditLength = new QLineEdit("LineEditLength", GroupBoxAttributes);
    LineEditLength->setEnabled( FALSE );

    GroupBoxAttributesLayout->addWidget( TextLabelLength, 4, 0 );
    GroupBoxAttributesLayout->addWidget( LineEditLength, 4, 1 );

////////////////////////////////////////////////////////////////////////////////////

}

/**
 * \fn ~Sketcher_PropertyLine() 
 * \brief destructor
 */
Sketcher_PropertyLine::~Sketcher_PropertyLine()
{
    // no need to delete child widgets, Qt does it all for us
}

/**
 * \fn SetGeometry()
 * \brief show object geometry in dialog window
 * \return void 
 */
void Sketcher_PropertyLine::SetGeometry()
{
	curGeom2d_Edge = Handle(Geom2d_Edge)::DownCast(mySObject->GetGeometry());

	firstPnt2d = curGeom2d_Edge->GetStart_Pnt();
	secondPnt2d = curGeom2d_Edge->GetEnd_Pnt();

	SetCoord(LineEditPoint1,firstPnt2d);
	SetCoord(LineEditPoint2,secondPnt2d);
	SetLineLength();
}

/**
 * \fn CheckGeometry()
 * \brief check geometry for change
 * \return bool 
 */
bool Sketcher_PropertyLine::CheckGeometry()
{
	if (CheckCoord(LineEditPoint1,tempPnt2d ))
		return CheckCoord(LineEditPoint2,temp2Pnt2d);
	else return false;
}

/**
 * \fn GetGeometry()
 * \brief create new object
 * \return bool 
 */
bool Sketcher_PropertyLine::GetGeometry()
{
	if(!firstPnt2d.IsEqual(tempPnt2d,1.0e-6) || !secondPnt2d.IsEqual(temp2Pnt2d,1.0e-6))
	{
		if (curGeom2d_Edge->SetPoints(tempPnt2d,temp2Pnt2d))
		{
			firstPnt2d = tempPnt2d;
			secondPnt2d = temp2Pnt2d;

			Handle (Geom_CartesianPoint) Point1 = new Geom_CartesianPoint(ElCLib::To3d(myCoordinateSystem.Ax2(),firstPnt2d));
			Handle (Geom_CartesianPoint) Point2 = new Geom_CartesianPoint(ElCLib::To3d(myCoordinateSystem.Ax2(),secondPnt2d));
			Handle(AIS_Line) newAIS_Line = new AIS_Line(Point1,Point2);

			myContext->Remove(myAIS_Object, true);
			myAIS_Object = newAIS_Line;

			SetLineLength();
			return true;
		}
	}
	return false;
}

/**
 * \fn SetLineLength()
 * \brief show line length in dialog window
 * \return void 
 */
void Sketcher_PropertyLine::SetLineLength() 
{
	double x = sqrt(pow((firstPnt2d.X()-secondPnt2d.X()),2) + pow((firstPnt2d.Y()-secondPnt2d.Y()),2));
	LineEditLength->setText(QString::number(x));
}

