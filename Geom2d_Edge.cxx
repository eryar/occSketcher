/**
* \file Geom2d_Edge.cxx
* \brief Implementation file for the class Geom2d_Edge
* \author <a href="mailto:sergmaslov@istel.ru?subject=Geom2d_Edge.cxx">Sergei Maslov</a>
*/
#include "Geom2d_Edge.hxx"

IMPLEMENT_STANDARD_RTTIEXT(Geom2d_Edge, Geom2d_Line)

/**
 * \fn Geom2d_Edge()
 * \brief Constructs a Geom2d_Edge
 */
Geom2d_Edge::Geom2d_Edge()
:Geom2d_Line   (gp::Origin2d(),gp::DX2d())
{
}

/**
 * \fn ~Geom2d_Edge()
 * \brief destructor
 */
Geom2d_Edge::~Geom2d_Edge()
{
}

/**
 * \fn SetPoints(const gp_Pnt2d& p1,const gp_Pnt2d& p2)
 * \brief set edge  by 2 points
 * \return Standard_Boolean 
 * \param p1 const gp_Pnt2d&
 * \param p2 const gp_Pnt2d&
 */
Standard_Boolean Geom2d_Edge::SetPoints(const gp_Pnt2d& p1,const gp_Pnt2d& p2)
{
	if (!p1.IsEqual(p2,0))
	{
		SetDirection(gp_Dir2d(gp_Vec2d(p1,p2)));
		SetLocation(p1);

		StartPnt = p1;
		EndPnt = p2;

		return Standard_True;
	}
	else return Standard_False;
}


/**
 * \fn GetStart_Pnt()
 * \brief get start 2d point
 * \return gp_Pnt2d const 
 */
gp_Pnt2d Geom2d_Edge::GetStart_Pnt() const
{
	return StartPnt;
}

/**
 * \fn GetEnd_Pnt()
 * \brief get end 2d point
 * \return gp_Pnt2d const 
 */
gp_Pnt2d Geom2d_Edge::GetEnd_Pnt() const
{
	return EndPnt;
}

/**
 * \fn MiddlePnt()
 * \brief get middle 2d point
 * \return gp_Pnt2d const 
 */
gp_Pnt2d Geom2d_Edge::MiddlePnt() const
{
	return StartPnt.Scaled(EndPnt,0.5);
}

/**
 * \fn StartParameter()
 * \brief get parameter of start 2d point
 * \return Standard_Real const
 */
Standard_Real Geom2d_Edge::StartParameter() const
{
	return ElCLib::Parameter(Lin2d(),StartPnt);
}

/**
 * \fn EndParameter()
 * \brief get parameter of end 2d point
 * \return Standard_Real const
 */
Standard_Real Geom2d_Edge::EndParameter() const
{
	return ElCLib::Parameter(Lin2d(),EndPnt);
}

