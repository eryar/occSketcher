/**
* \file Geom2d_Arc.hxx
* \brief Header file for the class Geom2d_Arc
* \author <a href="mailto:sergmaslov@istel.ru?subject=Geom2d_Arc.hxx">Sergei Maslov</a>
*/
#ifndef _Geom2d_Arc_HeaderFile
#define _Geom2d_Arc_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#include <Standard_DefineHandle.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Ax2d.hxx>
#include <gp_Circ2d.hxx>
#include <Geom2d_Circle.hxx>
#include <ElCLib.hxx>


DEFINE_STANDARD_HANDLE(Geom2d_Arc,Geom2d_Circle)

//Class describing 2d arc geometry
class Geom2d_Arc : public Geom2d_Circle
{

public:
// Type management
DEFINE_STANDARD_RTTI(Geom2d_Arc)

/**
 * \fn Geom2d_Arc(const gp_Circ2d& C)
 * \brief Constructs a Geom2d_Arc
 * \param C const gp_Circ2d&
 */
Standard_EXPORT Geom2d_Arc(const gp_Circ2d& C);

/**
 * \fn Geom2d_Arc(const gp_Ax2d& Ax2d,const Standard_Real R)
 * \brief Constructs a Geom2d_Arc
 * \param Ax2d const gp_Ax2d&
 * \param R const Standard_Real
 */
Standard_EXPORT Geom2d_Arc(const gp_Ax2d& Ax2d,const Standard_Real R);

/**
 * \fn ~Geom2d_Arc()
 * \brief destructor
 */
Standard_EXPORT ~Geom2d_Arc();

/**
 * \fn SetParam(const gp_Pnt2d& start,const gp_Pnt2d& mid,const gp_Pnt2d& end)
 * \brief set parameters
 * \param start const gp_Pnt2d&
 * \param mid const gp_Pnt2d&
 * \param end const gp_Pnt2d&
 */
Standard_EXPORT	void SetParam(const gp_Pnt2d& start,const gp_Pnt2d& mid,const gp_Pnt2d& end);

/**
 * \fn SetFirstParam(const Standard_Real u1)
 * \brief set first parameter
 * \return void
 * \param u1 const Standard_Real
 */ 
Standard_EXPORT	void SetFirstParam(const Standard_Real u1);

/**
 * \fn SetFirstParam(const gp_Pnt2d& p1)
 * \brief set first parameter
 * \return void
 * \param p1 const gp_Pnt2d&
 */ 
Standard_EXPORT	void SetFirstParam(const gp_Pnt2d& p1);

/**
 * \fn SetLastParam(const Standard_Real u2)
 * \brief set last parameter
 * \return void
 * \param u2 const Standard_Real
 */ 
Standard_EXPORT void SetLastParam(const Standard_Real u2);

/**
 * \fn SetLastParam(const gp_Pnt2d& p2)
 * \brief set last parameter
 * \return void
 * \param p2 const gp_Pnt2d&
 */ 
Standard_EXPORT void SetLastParam(const gp_Pnt2d& p2);
				
/**
 * \fn FirstParameter()
 * \brief get first parameter
 * \return Standard_Real const
 */ 
Standard_EXPORT Standard_Real FirstParameter() const;

/**
 * \fn LastParameter()
 * \brief get last parameter
 * \return Standard_Real const
 */ 
Standard_EXPORT Standard_Real LastParameter() const;

/**
 * \fn FirstPnt()
 * \brief get first 2d point
 * \return gp_Pnt2d const 
 */
Standard_EXPORT gp_Pnt2d FirstPnt() const;

/**
 * \fn LastPnt()
 * \brief get last 2d point
 * \return gp_Pnt2d const 
 */
Standard_EXPORT gp_Pnt2d LastPnt() const;

/**
 * \fn MiddlePnt()
 * \brief get middle 2d point
 * \return gp_Pnt2d const 
 */
Standard_EXPORT gp_Pnt2d MiddlePnt() const;

private: 

//methods

/**
 * \fn CheckParam()
 * \brief set correct parameters
 * \return void
 */
Standard_EXPORT void CheckParam();

//members

Standard_Real	myFirstParam;
Standard_Real	myLastParam;

};

#endif
