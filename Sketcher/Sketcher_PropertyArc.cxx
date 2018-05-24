/**
* \file Sketcher_PropertyArc.cxx
* \brief Implementation file for the class Sketcher_PropertyArc
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_PropertyArc.cxx">Sergei Maslov</a>
*/
#include "Sketcher_PropertyArc.hxx"

/**
 * \fn Sketcher_PropertyArc( QWidget* parent,  const char* name, WFlags fl )
 * \brief Constructs a Sketcher_PropertyArc which is a child of 'parent', with the name 'name' and widget flags set to 'f' 
 * \param parent QWidget*
 * \param name const char*
 * \param fl WFlags
 */
Sketcher_PropertyArc::Sketcher_PropertyArc( QWidget* parent,  const char* name, WFlags fl )
    : Sketcher_Property( parent, name, fl )
{
    if ( !name )
	setName( "Property of Arc" );
    setCaption( tr( "ArcProperties" ) );

////////////////////////////////////////////////////////////////////////////////

    TextLabelPoint1->setText( tr( "Center" ) );
    TextLabelRadius = new QLabel( GroupBoxGP, "TextLabelRadius" );
    TextLabelRadius->setText( tr( "Radius" ) );
    LineEditRadius = new QLineEdit( GroupBoxGP, "LineEditRadius" );

    TextLabelStartArc = new QLabel( GroupBoxGP, "TextLabelStartArc" );
    TextLabelStartArc->setText( tr( "Start" ) );
    LineEditStartArc = new QLineEdit( GroupBoxGP, "LineEditStartArc" );
    TextLabelEndArc = new QLabel( GroupBoxGP, "TextLabelEndArc" );
    TextLabelEndArc->setText( tr( "End" ) );
    LineEditEndArc = new QLineEdit( GroupBoxGP, "LineEditEndArc" );


    GroupBoxGPLayout->addWidget( TextLabelRadius, 1, 0 );
    GroupBoxGPLayout->addWidget( LineEditRadius, 1, 1 );
    GroupBoxGPLayout->addWidget( TextLabelStartArc, 2, 0 );
    GroupBoxGPLayout->addWidget( LineEditStartArc, 2, 1 );
    GroupBoxGPLayout->addWidget( TextLabelEndArc, 3, 0 );
    GroupBoxGPLayout->addWidget( LineEditEndArc, 3, 1 );

///////////////////////////////////////////////////////////////////////////////

	Init();

////////////////////////////////////////////////////////////////////////////////////

}

/**
 * \fn ~Sketcher_PropertyArc() 
 * \brief destructor
 */
Sketcher_PropertyArc::~Sketcher_PropertyArc()
{
    // no need to delete child widgets, Qt does it all for us
}


/**
 * \fn SetGeometry()
 * \brief show object geometry in dialog window
 * \return void 
 */
void Sketcher_PropertyArc::SetGeometry()
{
	curGeom2d_Arc = Handle(Geom2d_Arc)::DownCast(mySObject->GetGeometry());

	firstPnt2d = curGeom2d_Arc->Location();
	myRadius = curGeom2d_Arc->Radius();
	myFirstParameter = curGeom2d_Arc->FirstParameter() / (2*Standard_PI);
	myLastParameter = curGeom2d_Arc->LastParameter() / (2*Standard_PI);

	SetCoord(LineEditPoint1,firstPnt2d);
	LineEditRadius->setText(QString::number(myRadius));
	LineEditStartArc->setText(QString::number(myFirstParameter ));
	LineEditEndArc->setText(QString::number(myLastParameter));
}


/**
 * \fn CheckGeometry()
 * \brief check geometry for change
 * \return bool 
 */
bool Sketcher_PropertyArc::CheckGeometry()
{
	if (CheckCoord(LineEditPoint1,tempPnt2d ))
	{
		NumName = LineEditRadius->text();
		tempRadius = NumName.toDouble();
		if(NumName.contains(NumberExpr) == 0 && !NumName.isNull() && tempRadius > 0) 
		{
			NumName = LineEditStartArc->text();
			tempFirstParameter = NumName.toDouble();
			if(NumName.contains(NumberExpr) == 0 && !NumName.isNull()) 
			{
				NumName = LineEditEndArc->text();
				tempLastParameter = NumName.toDouble();
				if(NumName.contains(NumberExpr) == 0 && !NumName.isNull()) 
					return true;
				else LineEditEndArc->selectAll();
			}
			else LineEditStartArc->selectAll();
		}
		else LineEditRadius->selectAll();
	}
	return false;
}

/**
 * \fn GetGeometry()
 * \brief create new object
 * \return bool 
 */
bool Sketcher_PropertyArc::GetGeometry()
{
	if(!firstPnt2d.IsEqual(tempPnt2d,1.0e-6) || myRadius != tempRadius || myFirstParameter != tempFirstParameter || myLastParameter != tempLastParameter )
	{
		gce_MakeCirc2d tempMakeCirc2d(tempPnt2d,tempRadius);
		if (tempMakeCirc2d.Status() == gce_Done) 
		{
			firstPnt2d = tempPnt2d;
			myRadius = tempRadius;

			curGeom2d_Arc->SetCirc2d(tempMakeCirc2d.Value());
			curGeom2d_Arc->SetFirstParam(tempFirstParameter * 2*Standard_PI);
			curGeom2d_Arc->SetLastParam(tempLastParameter * 2*Standard_PI);
			myFirstParameter = curGeom2d_Arc->FirstParameter();
			myLastParameter = curGeom2d_Arc->LastParameter();

			Handle(Geom_Circle)	newGeom_Circle = new Geom_Circle(ElCLib::To3d(myCoordinateSystem.Ax2(),curGeom2d_Arc->Circ2d()));
			Handle(AIS_Circle) newAIS_Circle = new AIS_Circle(newGeom_Circle);
			newAIS_Circle->SetFirstParam(myFirstParameter);
			newAIS_Circle->SetLastParam (myLastParameter);
			myFirstParameter /= (2*Standard_PI);
			myLastParameter /= (2*Standard_PI);

			myContext->Remove(myAIS_Object);
			myAIS_Object = newAIS_Circle;

			LineEditStartArc->setText(QString::number(myFirstParameter));
			LineEditEndArc->setText(QString::number(myLastParameter));
			return true;
		}
	}
	return false;
}

