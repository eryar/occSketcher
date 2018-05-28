/**
* \file Sketcher_QtGUI.cxx
* \brief Implementation file for the class Sketcher_QtGUI
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_QtGUI.cxx">Sergei Maslov</a>
*/
#include "Sketcher_QtGUI.hxx"

/**
 * \fn Sketcher_QtGUI( QWidget* parent)
 * \brief Constructs a Sketcher_QtGUI
 * \param parent QWidget*
 */
Sketcher_QtGUI::Sketcher_QtGUI(QWidget* /*parent*/)
{
	/*prop_arc = new Sketcher_PropertyArc( parent, "Arc", true);
	prop_circle = new Sketcher_PropertyCircle( parent, "Circle", true);
	prop_line = new Sketcher_PropertyLine( parent, "Lines", true);
	prop_point = new Sketcher_PropertyPoint( parent, "Points", true);*/
}

/**
 * \fn ~Sketcher_GUI() 
 * \brief destructor
 */
Sketcher_QtGUI::~Sketcher_QtGUI()
{
    // no need to delete child widgets, Qt does it all for us
}

/**
 * \fn SetContext(Handle(AIS_InteractiveContext)& theContext)
 * \brief pass context to property classes
 * \return void 
 * \param theContext Handle(AIS_InteractiveContext)&
 */
void Sketcher_QtGUI::SetContext (Handle(AIS_InteractiveContext)& /*theContext*/)
{
	/*prop_arc->SetContext( theContext);
	prop_circle->SetContext( theContext);
	prop_line->SetContext( theContext);
	prop_point->SetContext( theContext);*/
}

/**
 * \fn SetAx3(const gp_Ax3& theAx3)
 * \brief pass coordinate system to property classes
 * \return void 
 * \param theAx3 const gp_Ax3&
 */
void Sketcher_QtGUI::SetAx3 (const gp_Ax3& /*theAx3*/)
{
	/*prop_arc->SetAx3( theAx3);
	prop_circle->SetAx3( theAx3);
	prop_line->SetAx3( theAx3);
	prop_point->SetAx3( theAx3);*/
}

/**
 * \fn SetSketcher_Object(Handle(Sketcher_Object)& CurObject)
 * \brief determine property to show, pass object to this property class 
 * \return void 
 * \param CurObject Handle(Sketcher_Object)&
 */
void Sketcher_QtGUI::SetSketcher_Object(Handle(Sketcher_Object)& /*CurObject*/)
{
	/*if(!prop_arc->isHidden())	
		prop_arc->close();
	if(!prop_circle->isHidden())
		prop_circle->close();
	if(!prop_line->isHidden())	
		prop_line->close();
	if(!prop_point->isHidden()) 
		prop_point->close();

	switch (CurObject->GetGeometryType())
	{
	case ArcSketcherObject: 	prop_arc->SetObject(CurObject);
								break;
	case CircleSketcherObject:	prop_circle->SetObject(CurObject);
								break;
	case LineSketcherObject:	prop_line->SetObject(CurObject);
								break;
	case PointSketcherObject:	prop_point->SetObject(CurObject);
								break;
	default:break;
	}*/
}

