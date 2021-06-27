/**
* \file Geom2d_Edge.hxx
* \brief Header file for the class Geom2d_Edge
* \author <a href="mailto:sergmaslov@istel.ru?subject=Geom2d_Edge.hxx">Sergei Maslov</a>
*/
#ifndef _Geom2d_Edge_HeaderFile
#define _Geom2d_Edge_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Dir2d.hxx>
#include <Geom2d_Line.hxx>
#include <ElCLib.hxx>

class gp_Pnt2d2d;

DEFINE_STANDARD_HANDLE(Geom2d_Edge,Geom2d_Line)

//Class describing 2d edge geometry
class Geom2d_Edge : public Geom2d_Line
{

public:
// Type management
DEFINE_STANDARD_RTTIEXT(Geom2d_Edge, Geom2d_Line)

/**
 * \fn Geom2d_Edge()
 * \brief Constructs a Geom2d_Edge
 */
Standard_EXPORT Geom2d_Edge();

/**
 * \fn ~Geom2d_Edge()
 * \brief destructor
 */
Standard_EXPORT ~Geom2d_Edge();

/**
 * \fn SetPoints(const gp_Pnt2d& p1,const gp_Pnt2d& p2)
 * \brief set edge  by 2 points
 * \return Standard_Boolean 
 * \param p1 const gp_Pnt2d&
 * \param p2 const gp_Pnt2d&
 */
Standard_EXPORT Standard_Boolean SetPoints(const gp_Pnt2d& p1,const gp_Pnt2d& p2);

/**
 * \fn GetStart_Pnt()
 * \brief get start 2d point
 * \return gp_Pnt2d const 
 */
Standard_EXPORT gp_Pnt2d GetStart_Pnt() const;

/**
 * \fn GetEnd_Pnt()
 * \brief get end 2d point
 * \return gp_Pnt2d const 
 */
Standard_EXPORT gp_Pnt2d GetEnd_Pnt() const;

/**
 * \fn MiddlePnt()
 * \brief get middle 2d point
 * \return gp_Pnt2d const 
 */
Standard_EXPORT gp_Pnt2d MiddlePnt() const;

/**
 * \fn StartParameter()
 * \brief get parameter of start 2d point
 * \return Standard_Real const
 */
Standard_EXPORT Standard_Real StartParameter() const;

/**
 * \fn EndParameter()
 * \brief get parameter of end 2d point
 * \return Standard_Real const
 */
Standard_EXPORT Standard_Real EndParameter() const;

private: 

gp_Pnt2d	StartPnt;
gp_Pnt2d	EndPnt;

};


#endif
