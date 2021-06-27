/**
* \file Sketcher_SnapEnd.cxx
* \brief Implementation file for the class Sketcher_SnapEnd
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_SnapEnd.cxx">Sergei Maslov</a>
*/
#include "Sketcher_SnapEnd.hxx"

IMPLEMENT_STANDARD_RTTIEXT(Sketcher_SnapEnd, Sketcher_Snap)

/**
 * \fn Sketcher_SnapEnd() 
 * \brief Constructs a Sketcher_SnapEnd
 */
Sketcher_SnapEnd::Sketcher_SnapEnd()
{
}

/**
 * \fn ~Sketcher_SnapEnd() 
 * \brief Destructor
 */
Sketcher_SnapEnd::~Sketcher_SnapEnd()
{
}

/**
 * \fn SelectEvent() 
 * \brief find point nearest to end line
 * \return void 
 */
void Sketcher_SnapEnd::SelectEvent()
{
	findbestPnt2d = Standard_False;
	minDistance = minimumSnapDistance;

	for( Standard_Integer i=1; i<=data->Length(); i++)
	{
		mySObject = Handle(Sketcher_Object)::DownCast(data->Value(i));
		myGeometryType = mySObject->GetGeometryType();
		switch (myGeometryType)
		{
		case PointSketcherObject:	
				curGeom2d_Point = Handle(Geom2d_CartesianPoint)::DownCast(mySObject->GetGeometry());
				objectPnt2d = curGeom2d_Point->Pnt2d();
				if(count())
				{
					bestPnt2d = objectPnt2d;
					curHilightedObj = mySObject->GetAIS_Object();
				}
				break;
		case LineSketcherObject:	
				curGeom2d_Edge = Handle(Geom2d_Edge)::DownCast(mySObject->GetGeometry());
				objectPnt2d = curGeom2d_Edge->GetStart_Pnt();
				if(count())
				{
					bestPnt2d = objectPnt2d;
					curHilightedObj = mySObject->GetAIS_Object();
				}
				objectPnt2d = curGeom2d_Edge->GetEnd_Pnt();
				if(count())
				{
					bestPnt2d = objectPnt2d;
					curHilightedObj = mySObject->GetAIS_Object();
				}
				break;
		case CircleSketcherObject:	
				break;
		case ArcSketcherObject:
				curGeom2d_Arc = Handle(Geom2d_Arc)::DownCast(mySObject->GetGeometry());
				objectPnt2d = curGeom2d_Arc->FirstPnt();
				if(count())
				{
					bestPnt2d = objectPnt2d;
					curHilightedObj = mySObject->GetAIS_Object();
				}
				objectPnt2d = curGeom2d_Arc->LastPnt();
				if(count())
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
	else findbestPnt2d = Standard_True;
}


/**
 * \fn GetSnapType()
 * \brief get Snap type
 * \return Sketcher_SnapType 
 */
Sketcher_SnapType Sketcher_SnapEnd::GetSnapType()
{
	return SnapEnd;
}
