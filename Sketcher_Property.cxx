/**
* \file Sketcher_Property.cxx
* \brief Implementation file for the class Sketcher_Property
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_Property.cxx">Sergei Maslov</a>
*/
#include "Sketcher_Property.hxx"

#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#include <qpixmap.h>

/**
 * \fn Sketcher_Property( QWidget* parent,  const char* name, WFlags fl )
 * \brief Constructs a Sketcher_Property which is a child of 'parent', with the name 'name' and widget flags set to 'f' 
 * \param parent QWidget*
 * \param name const char*
 * \param fl WFlags
 */
Sketcher_Property::Sketcher_Property( QWidget* parent,  const char* name)
    : QWidget( parent),
	StartCoord("["),MidCoord(","),EndCoord("]"),
	NumberExpr("[^0-9.-]")
{
    if ( !name )
	setWindowTitle( "Sketcher_Property" );
    resize( 288, 369 ); 
    setMaximumSize( QSize( 400, 400 ) );
    //setCaption( tr( "Properties" ) );

////////////////////////////////////////////////////////////////////////////////
    TextLabelID = new QLabel( "TextLabelID", this );
    TextLabelID->setText( tr( "ID" ) );
    LineEditID = new QLineEdit( "LineEditID", this );
////////////////////////////////////////////////////////////////////////////////

    GroupBoxGP = new QGroupBox( "GroupBoxGP", this );
    GroupBoxGP->setTitle( tr( "Geometry Properties" ) );
    //GroupBoxGP->setColumnLayout(0, Qt::Vertical );
    GroupBoxGP->layout()->setSpacing( 0 );
    GroupBoxGP->layout()->setMargin( 0 );

    TextLabelPoint1 = new QLabel("TextLabelPoint1", GroupBoxGP);
    LineEditPoint1 = new QLineEdit( "LineEditPoint1", GroupBoxGP);

    GroupBoxGPLayout = new QGridLayout( GroupBoxGP);
    GroupBoxGPLayout->setAlignment( Qt::AlignTop );
    GroupBoxGPLayout->setSpacing( 6 );
    GroupBoxGPLayout->setMargin( 11 );
    GroupBoxGPLayout->addWidget( TextLabelPoint1, 0, 0 );
    GroupBoxGPLayout->addWidget( LineEditPoint1, 0, 1 );

///////////////////////////////////////////////////////////////////////////////

    GroupBoxAttributes = new QGroupBox( "GroupBoxAttributes", this );
    GroupBoxAttributes->setTitle( tr( "Attributes" ) );
    //GroupBoxAttributes->setColumnLayout(0, Qt::Vertical );
    GroupBoxAttributes->layout()->setSpacing( 0 );
    GroupBoxAttributes->layout()->setMargin( 0 );

    TextLabelColor = new QLabel( GroupBoxAttributes);
    TextLabelColor->setText( tr( "Color" ) );
    ComboBoxColor = new QComboBox( GroupBoxAttributes);
    ComboBoxColor->addItem( tr( "BLACK" ) );				
    ComboBoxColor->addItem( tr( "BROWN" ) );	
    ComboBoxColor->addItem( tr( "RED" ) );			
    ComboBoxColor->addItem( tr( "ORANGE" ) );			
    ComboBoxColor->addItem( tr( "YELLOW" ) );			
    ComboBoxColor->addItem( tr( "FORESTGREEN" ) );		
    ComboBoxColor->addItem( tr( "GREEN" ) );		
    ComboBoxColor->addItem( tr( "BLUE" ) );	
    ComboBoxColor->addItem( tr( "DEEPSKYBLUE1" ) );		
    ComboBoxColor->addItem( tr( "LIGHTSKYBLUE" ) );	
    ComboBoxColor->addItem( tr( "CYAN" ) );				
    ComboBoxColor->addItem( tr( "PURPLE" ) );		
    ComboBoxColor->addItem( tr( "MAGENTA" ) );			
    ComboBoxColor->addItem( tr( "VIOLET" ) );			
    ComboBoxColor->addItem( tr( "DEEPPINK" ) );			
    ComboBoxColor->addItem( tr( "PINK" ) );			
    ComboBoxColor->addItem( tr( "GRAY" ) );		
    ComboBoxColor->addItem( tr( "WHITE" ) );

    TextLabelType = new QLabel( "TextLabelType", GroupBoxAttributes);
    TextLabelType->setText( tr( "Type" ) );
    ComboBoxType = new QComboBox( GroupBoxAttributes);
    ComboBoxType->addItem( tr( "Main" ) );
    ComboBoxType->addItem( tr( "Auxiliary" ) );

    GroupBoxAttributesLayout = new QGridLayout( GroupBoxAttributes);
    GroupBoxAttributesLayout->setAlignment( Qt::AlignTop );
    GroupBoxAttributesLayout->setSpacing( 6 );
    GroupBoxAttributesLayout->setMargin( 11 );
    GroupBoxAttributesLayout->addWidget( TextLabelColor, 0, 0 );
    GroupBoxAttributesLayout->addWidget( ComboBoxColor, 0, 1 );
    GroupBoxAttributesLayout->addWidget( TextLabelType, 1, 0 );
    GroupBoxAttributesLayout->addWidget( ComboBoxType, 1, 1 );

////////////////////////////////////////////////////////////////////////////////////

    PushButtonOK = new QPushButton( "PushButtonOK", this );
	PushButtonOK->setMinimumSize ( 70, 25 );
    PushButtonOK->setText( tr( "OK" ) );
    PushButtonCancel = new QPushButton( "PushButtonCancel", this );
	PushButtonCancel->setMinimumSize ( 70, 25 );
    PushButtonCancel->setText( tr( "Cancel" ) );
    PushButtonApply = new QPushButton( "PushButtonApply", this );
	PushButtonApply->setMinimumSize ( 70, 25 );
    PushButtonApply->setText( tr( "Apply" ) );

    QSpacerItem* spacer_1 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    QSpacerItem* spacerOK_Cancel = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    QSpacerItem* spacerCancel_Apply = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );


    PropertyLayout = new QGridLayout( this ); 
    PropertyLayout->setSpacing( 6 );
    PropertyLayout->setMargin( 11 );
    PropertyLayout->addWidget( TextLabelID, 0, 0 );
    //PropertyLayout->addMultiCellWidget( LineEditID, 0, 0, 1, 5 );
    PropertyLayout->addItem( spacer_1, 1, 3 );
    //PropertyLayout->addMultiCellWidget( GroupBoxGP, 2, 2, 0, 5 );
    PropertyLayout->addItem( spacer_2, 3, 3 );
    //PropertyLayout->addMultiCellWidget( GroupBoxAttributes, 4, 4, 0, 5 );
    PropertyLayout->addItem( spacer_3, 5, 3 );
    //PropertyLayout->addMultiCellWidget( PushButtonOK, 6, 6, 0, 1 );
    PropertyLayout->addItem( spacerOK_Cancel, 6, 2 );
    PropertyLayout->addWidget( PushButtonCancel, 6, 3 );
    PropertyLayout->addItem( spacerCancel_Apply, 6, 4 );
    PropertyLayout->addWidget( PushButtonApply, 6, 5 );

    connect( PushButtonOK,	SIGNAL( clicked() ), this, SLOT( onOK( ) ) );
    connect( PushButtonCancel,	SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( PushButtonApply,	SIGNAL( clicked() ), this, SLOT( onApply () ) );

	QString fileName( tr("res"));
	fileName=fileName + QString("/") + QString( tr("property.png"));
	QPixmap myIcon( fileName);//, const char * format, int conversion_flags )
	setWindowIcon (myIcon);

	myNameOfColor = Quantity_NOC_YELLOW;
	myObjectStyle = Aspect_TOL_SOLID;
	myWidth = 1.0;
	myPrs3dAspect = new Prs3d_LineAspect(myNameOfColor,myObjectStyle,myWidth);
	myDrawer = new Prs3d_Drawer();
	myDrawer->SetLineAspect(myPrs3dAspect);
}

/**
 * \fn ~Sketcher_Property() 
 * \brief destructor
 */
Sketcher_Property::~Sketcher_Property()
{
    // no need to delete child widgets, Qt does it all for us
}

/**
 * \fn Init()
 * \brief Constructs a supplementary elements 
 * \return void 
 */
void Sketcher_Property::Init()
{
	isPointWindow = false;
    TextLabelStyle = new QLabel( "TextLabelStyle", GroupBoxAttributes);
    TextLabelStyle->setText( tr( "Style" ) );
    ComboBoxStyle = new QComboBox(GroupBoxAttributes);
    ComboBoxStyle->addItem( tr( "SOLID" ) );
    ComboBoxStyle->addItem( tr( "DASH" ) );
    ComboBoxStyle->addItem( tr( "DOT" ) );
    ComboBoxStyle->addItem( tr( "DOTDASH" ) );

    TextLabelWidth = new QLabel("TextLabelWidth", GroupBoxAttributes);
    TextLabelWidth->setText( tr( "Width" ) );
    ComboBoxWidth = new QComboBox(GroupBoxAttributes);
    ComboBoxWidth->addItem( tr( "1.0" ) );
    ComboBoxWidth->addItem( tr( "2.0" ) );
    ComboBoxWidth->addItem( tr( "3.0" ) );
    ComboBoxWidth->addItem( tr( "4.0" ) );
    ComboBoxWidth->addItem( tr( "5.0" ) );
    ComboBoxWidth->addItem( tr( "6.0" ) );
    ComboBoxWidth->addItem( tr( "7.0" ) );
    ComboBoxWidth->addItem( tr( "8.0" ) );
    ComboBoxWidth->addItem( tr( "9.0" ) );

    GroupBoxAttributesLayout->addWidget( TextLabelStyle, 2, 0 );
    GroupBoxAttributesLayout->addWidget( ComboBoxStyle, 2, 1 );
    GroupBoxAttributesLayout->addWidget( TextLabelWidth, 3, 0 );
    GroupBoxAttributesLayout->addWidget( ComboBoxWidth, 3, 1 );
}


/**
 * \fn SetContext(Handle(AIS_InteractiveContext)& theContext)
 * \brief set context
 * \return void 
 * \param theContext Handle(AIS_InteractiveContext)&
 */
void Sketcher_Property::SetContext (Handle(AIS_InteractiveContext)& theContext)
{
	myContext = theContext;
}

/**
 * \fn SetAx3(const gp_Ax3& theAx3)
 * \brief set coordinate system
 * \return void 
 * \param theAx3 const gp_Ax3&
 */
void Sketcher_Property::SetAx3 (const gp_Ax3& theAx3)
{
	myCoordinateSystem = theAx3;
}


/**
 * \fn SetObject(Handle(Sketcher_Object)& CurObject)
 * \brief set Sketcher_Object, show dialog window
 * \return void 
 * \param CurObject Handle(Sketcher_Object)&
 */
void Sketcher_Property::SetObject(Handle(Sketcher_Object)& CurObject)
{
	mySObject = CurObject;
	myAIS_Object = mySObject->GetAIS_Object();
	myID = mySObject->GetObjectName();
	myNameOfColor = mySObject->GetColor();
	myObjectType = mySObject->GetType();
	if (!isPointWindow)
	{
		myObjectStyle = mySObject->GetStyle();
		myWidth = mySObject->GetWidth();
		SetObjectStyle();
		SetWidth();
	}

	SetID();
	SetColor();
	SetObjectType();

	SetGeometry();
	show();
}


/**
 * \fn onOK()
 * \brief clicking on "OK" button handler
 * \return void 
 */
void Sketcher_Property::onOK()
{
	if(CheckGeometry())
	{
		if (GetGeometry()) 
			 GetAttributies();
		else CheckAttributies();
		GetName();

		mySObject->SetAIS_Object(myAIS_Object);
		myContext->Display(myAIS_Object, true);

		close();
	}
}

/**
 * \fn onApply()
 * \brief clicking on "Apply" button handler
 * \return void 
 */
void Sketcher_Property::onApply()
{
	if(CheckGeometry())
	{
		if (GetGeometry()) 
		{
			GetAttributies();
			mySObject->SetAIS_Object(myAIS_Object);
			myContext->Display(myAIS_Object, true);
		}
		else CheckAttributies();
		GetName();

	}
}

/**
 * \fn SetID()
 * \brief set object name to LineEditID
 * \return void 
 */
void Sketcher_Property::SetID ()
{
	LineEditID->setText(QString((QChar*)myID.ToExtString(), myID.Length ()));
}

/**
 * \fn GetID()
 * \brief get text from LineEditID
 * \return TCollection_ExtendedString 
 */
TCollection_ExtendedString Sketcher_Property::GetID ()
{
	return TCollection_ExtendedString((char *)LineEditID->text().toLocal8Bit ().data() );
}

/**
 * \fn SetCoord(QLineEdit* le,const gp_Pnt2d& p)
 * \brief set point coordinate to le
 * \return void 
 * \param le QLineEdit*
 * \param p const gp_Pnt2d&
 */
void Sketcher_Property::SetCoord(QLineEdit* le,const gp_Pnt2d& p)
{
	NumName = StartCoord;
	NumName+= QString::number(p.X());
	NumName+= MidCoord;
	NumName+= QString::number(p.Y());
	NumName+= EndCoord;
	le->setText(NumName);
}

/**
 * \fn CheckCoord(QLineEdit* le,const gp_Pnt2d& p)
 * \brief get point coordinate from le
 * \return bool 
 * \param le QLineEdit*
 * \param p const gp_Pnt2d&
 */
bool Sketcher_Property::CheckCoord(QLineEdit* le, gp_Pnt2d& p)
{
	int pos = 0;
	QString Xstring,Ystring;
	NumName = le->text();
	if(NumName.left(1) == StartCoord && NumName.right(1) == EndCoord)
	{
		NumName = NumName.mid(1,NumName.length()-2);
		pos = NumName.indexOf ( MidCoord );
		if (pos > 0)
		{
			Xstring = NumName.left(pos);
			if(Xstring.contains(NumberExpr) == 0 && !Xstring.isNull()) 
			{
				Ystring = NumName.right(NumName.length()-pos-1);
				if(Ystring.contains(NumberExpr) == 0 && !Ystring.isNull()) 
				{
					p.SetCoord(Xstring.toDouble(),Ystring.toDouble());
					return true;
				}
			}
		}
	}
	le->selectAll();
    return false;
}

/**
 * \fn CheckAttributies()
 * \brief check attributes for change
 * \return void 
 */
void Sketcher_Property::CheckAttributies()
{
	if (myNameOfColor != GetColor() || myObjectStyle != GetObjectStyle() || myWidth != GetWidth())
	{
		GetAttributies();
		myContext->Redisplay(myAIS_Object, true);
	}
}

/**
 * \fn GetAttributies()
 * \brief set attributes to object
 * \return void 
 */
void Sketcher_Property::GetAttributies()
{
	myNameOfColor = GetColor();
	mySObject->SetColor(myNameOfColor);
	if (isPointWindow) myAIS_Object->SetColor(myNameOfColor);
	else
	{
		myObjectStyle = GetObjectStyle();
		myWidth = GetWidth();

		mySObject->SetStyle(myObjectStyle);
		mySObject->SetWidth(myWidth);

		myPrs3dAspect->SetColor(myNameOfColor);
		myPrs3dAspect->SetTypeOfLine(myObjectStyle);
		myPrs3dAspect->SetWidth(myWidth);

		myDrawer->SetLineAspect(myPrs3dAspect);
		myAIS_Object->SetAttributes(myDrawer);
	}
}

/**
 * \fn GetName()
 * \brief check name and type of object for change
 * \return void 
 */
void Sketcher_Property::GetName()
{
	tempID = GetID();
	if (!myID.IsEqual(tempID))
	{
		myID = tempID;
		mySObject->SetObjectName(myID);
	}

	if (myObjectType != GetObjectType())
	{
		myObjectType = GetObjectType();
		mySObject->SetType(myObjectType);
	}

}

/**
 * \fn SetColor()
 * \brief show object color in dialog window
 * \return void 
 */
void Sketcher_Property::SetColor()
{
	switch(myNameOfColor)
	{
	case Quantity_NOC_BLACK:		ComboBoxColor->setCurrentIndex ( 0 );
									break;
	case Quantity_NOC_BROWN:		ComboBoxColor->setCurrentIndex ( 1 );
									break;
	case Quantity_NOC_RED:			ComboBoxColor->setCurrentIndex ( 2 );
									break;
	case Quantity_NOC_ORANGE:		ComboBoxColor->setCurrentIndex ( 3 );
									break;
	case Quantity_NOC_YELLOW:		ComboBoxColor->setCurrentIndex ( 4 );
									break;
	case Quantity_NOC_FORESTGREEN:	ComboBoxColor->setCurrentIndex ( 5 );
									break;
	case Quantity_NOC_GREEN:		ComboBoxColor->setCurrentIndex ( 6 );
									break;
	case Quantity_NOC_BLUE1:		ComboBoxColor->setCurrentIndex ( 7 );
									break;
	case Quantity_NOC_DEEPSKYBLUE1:	ComboBoxColor->setCurrentIndex ( 8 );
									break;
	case Quantity_NOC_LIGHTSKYBLUE:	ComboBoxColor->setCurrentIndex ( 9 );
									break;
	case Quantity_NOC_CYAN1:		ComboBoxColor->setCurrentIndex ( 10 );
									break;
	case Quantity_NOC_PURPLE:		ComboBoxColor->setCurrentIndex ( 11 );
									break;
	case Quantity_NOC_MAGENTA1:		ComboBoxColor->setCurrentIndex ( 12 );
									break;
	case Quantity_NOC_VIOLET:		ComboBoxColor->setCurrentIndex ( 13 );
									break;
	case Quantity_NOC_DEEPPINK:		ComboBoxColor->setCurrentIndex ( 14 );
									break;
	case Quantity_NOC_PINK:			ComboBoxColor->setCurrentIndex ( 15 );
									break;
	case Quantity_NOC_GRAY70:		ComboBoxColor->setCurrentIndex ( 16 );
									break;
	case Quantity_NOC_WHITE:		ComboBoxColor->setCurrentIndex ( 17 );
									break;
	default: ComboBoxColor->setCurrentIndex ( 4 );
			 break;
	}
}  

/**
 * \fn GetColor()
 * \brief get object color from dialog window
 * \return Quantity_NameOfColor 
 */
Quantity_NameOfColor Sketcher_Property::GetColor()
{
	switch(ComboBoxColor->currentIndex ())
	{
	case 0:  return Quantity_NOC_BLACK;
			 break;
	case 1:  return Quantity_NOC_BROWN;
			 break;
	case 2:  return Quantity_NOC_RED;
			 break;
	case 3:  return Quantity_NOC_ORANGE;
			 break;
	case 4:  return Quantity_NOC_YELLOW;
			 break;
	case 5:  return Quantity_NOC_FORESTGREEN;
			 break;
	case 6:  return Quantity_NOC_GREEN;
			 break;
	case 7:  return Quantity_NOC_BLUE1;
			 break;
	case 8:  return Quantity_NOC_DEEPSKYBLUE1;
			 break;
	case 9:  return Quantity_NOC_LIGHTSKYBLUE;
			 break;
	case 10: return Quantity_NOC_CYAN1;
			 break;
	case 11: return Quantity_NOC_PURPLE;
			 break;
	case 12: return Quantity_NOC_MAGENTA1;
			 break;
	case 13: return Quantity_NOC_VIOLET;
			 break;
	case 14: return Quantity_NOC_DEEPPINK;
			 break;
	case 15: return Quantity_NOC_PINK;
			 break;
	case 16: return Quantity_NOC_GRAY70;
			 break;
	case 17: return Quantity_NOC_WHITE;
			 break;
	default: return Quantity_NOC_YELLOW;
			 break;
	}
}  

/**
 * \fn SetObjectType()
 * \brief show object type in dialog window
 * \return void 
 */
void Sketcher_Property::SetObjectType()
{
	ComboBoxType->setCurrentIndex ((int) myObjectType );
}

/**
 * \fn GetObjectType()
 * \brief get object type from dialog window
 * \return Sketcher_ObjectType 
 */
Sketcher_ObjectType Sketcher_Property::GetObjectType()
{
	return (Sketcher_ObjectType)ComboBoxType->currentIndex ();
}  

/**
 * \fn SetObjectStyle()
 * \brief show object line style in dialog window
 * \return void 
 */
void Sketcher_Property::SetObjectStyle()
{
	ComboBoxStyle->setCurrentIndex ((int) myObjectStyle );
}  

/**
 * \fn GetObjectStyle()
 * \brief get object line style from dialog window
 * \return Aspect_TypeOfLine 
 */
Aspect_TypeOfLine Sketcher_Property::GetObjectStyle()
{
	if (isPointWindow) return Aspect_TOL_SOLID;
	else  return (Aspect_TypeOfLine)ComboBoxStyle->currentIndex ();
}  

/**
 * \fn SetWidth()
 * \brief show object line width in dialog window
 * \return void 
 */
void Sketcher_Property::SetWidth()
{
	ComboBoxWidth->setCurrentIndex (((int)myWidth) -1);
}

/**
 * \fn GetWidth()
 * \brief get object line width from dialog window
 * \return Standard_Real 
 */
Standard_Real Sketcher_Property::GetWidth()
{
	if (isPointWindow) return 1.0;
	else
	return (double)(ComboBoxWidth->currentIndex () +1);
}
