/**
* \file Sketcher_SnapNearest.cxx
* \brief Implementation file for the class Sketcher_SnapNearest
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_SnapNearest.cxx">Sergei Maslov</a>
*/
#include "Sketcher_SnapNearest.hxx"

/**
 * \fn Sketcher_SnapNearest() 
 * \brief Constructs a Sketcher_SnapNearest
 */
Sketcher_SnapNearest::Sketcher_SnapNearest()
{
}


/**
 * \fn ~Sketcher_SnapNearest() 
 * \brief Destructor
 */
Sketcher_SnapNearest::~Sketcher_SnapNearest()
{
}

/**
 * \fn SelectEvent() 
 * \brief find nearest point on curve
 * \return void 
 */
void Sketcher_SnapNearest::SelectEvent()
{
  findbestPnt2d = Standard_False;
  minDistance = minimumSnapDistance;

  for( Standard_Integer i=1; i<=data->Length(); i++)
  {
	mySObject = Handle(Sketcher_Object)::DownCast(data->Value(i));
	myGeometryType = mySObject->GetGeometryType();
	switch (myGeometryType)
	{
	case PointSketcherObject:	break;
	case LineSketcherObject:
	case CircleSketcherObject:	
	case ArcSketcherObject: 	curGeom2d_Curve = Handle(Geom2d_Curve)::DownCast(mySObject->GetGeometry());
								ProjectOnCurve.Init(curPnt2d,curGeom2d_Curve);
								if(countProject())
								{
									bestPnt2d = objectPnt2d;
									curHilightedObj = mySObject->GetAIS_Object();
								}
								break;
	default:break;
	}
  }

  if(minDistance == minimumSnapDistance) 
		 bestPnt2d = curPnt2d;
  else   findbestPnt2d = Standard_True;
}


/**
 * \fn GetSnapType()
 * \brief get Snap type
 * \return Sketcher_SnapType 
 */
Sketcher_SnapType Sketcher_SnapNearest::GetSnapType()
{
	return SnapNearest;
}


IMPLEMENT_STANDARD_HANDLE(Sketcher_SnapNearest,Sketcher_Snap)
IMPLEMENT_STANDARD_RTTI(Sketcher_SnapNearest)

IMPLEMENT_STANDARD_TYPE(Sketcher_SnapNearest)
IMPLEMENT_STANDARD_SUPERTYPE(Sketcher_Snap)
IMPLEMENT_STANDARD_SUPERTYPE(MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE(Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY()
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Sketcher_Snap)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_END()
IMPLEMENT_STANDARD_TYPE_END(Sketcher_SnapNearest)
