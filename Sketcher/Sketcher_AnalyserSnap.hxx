/**
* \file Sketcher_AnalyserSnap.hxx
* \brief Header file for the class Sketcher_AnalyserSnap
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_AnalyserSnap.hxx">Sergei Maslov</a>
*/
#ifndef SKETCHER_ANALYSERSNAP_H
#define SKETCHER_ANALYSERSNAP_H

#include <Standard_Macro.hxx>
#include <MMgt_TShared.hxx>
#include <Standard_DefineHandle.hxx>

#include "Sketcher_Object.hxx"
#include "Sketcher_Snap.hxx"

#include "Sketcher_SnapEnd.hxx"
#include "Sketcher_SnapMiddle.hxx"
#include "Sketcher_SnapCenter.hxx"
#include "Sketcher_SnapNearest.hxx"
#include "Sketcher_SnapIntersection.hxx"
#include "Sketcher_SnapLineParallel.hxx"
#include "Sketcher_SnapLinePerpendicular.hxx"
#include "Sketcher_SnapTangent.hxx"

#include <TColStd_HSequenceOfTransient.hxx>
#include <AIS_InteractiveContext.hxx>

#include <TColgp_SequenceOfPnt2d.hxx>
#include <TColStd_SequenceOfReal.hxx>
#include <TColStd_SequenceOfInteger.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Ax3.hxx>


DEFINE_STANDARD_HANDLE(Sketcher_AnalyserSnap,MMgt_TShared)

//Snap Manager class 
class Sketcher_AnalyserSnap : public MMgt_TShared 
{
public:
// Type management
DEFINE_STANDARD_RTTI(Sketcher_AnalyserSnap)

/**
 * \fn Sketcher_AnalyserSnap(Handle(AIS_InteractiveContext)& theContext,Handle(TColStd_HSequenceOfTransient)& thedata,gp_Ax3 &theAx3) 
 * \brief Constructs a Sketcher_AnalyserSnap
 * \param theContext Handle(AIS_InteractiveContext)&
 * \param thedata Handle(TColStd_HSequenceOfTransient)&
 * \param theAx3 gp_Ax3&
 */
Standard_EXPORT	Sketcher_AnalyserSnap(Handle(AIS_InteractiveContext)& theContext, Handle(TColStd_HSequenceOfTransient)& thedata,gp_Ax3 & theAx3);

/**
 * \fn ~Sketcher_AnalyserSnap()
 * \brief destructor
 */
Standard_EXPORT	~Sketcher_AnalyserSnap();

/**
 * \fn SetContext(Handle(AIS_InteractiveContext)& theContext)
 * \brief set context
 * \return void 
 * \param theContext Handle(AIS_InteractiveContext)&
 */
Standard_EXPORT	void SetContext (Handle(AIS_InteractiveContext)& theContext);

/**
 * \fn SetData(Handle(TColStd_HSequenceOfTransient)& thedata)
 * \brief set list of objects
 * \return void 
 * \param thedata TColStd_HSequenceOfTransient)&
 */
Standard_EXPORT	void SetData (Handle(TColStd_HSequenceOfTransient)& thedata); 

/**
 * \fn SetAx3(const gp_Ax3& theAx3)
 * \brief set coordinate system
 * \return void 
 * \param theAx3 const gp_Ax3&
 */
Standard_EXPORT	void SetAx3(const gp_Ax3 &theAx3);

/**
 * \fn SetMinDistance(const Standard_Real& aPrecise)
 * \brief set precise for snap
 * \return void 
 * \param aPrecise const Standard_Real&
 */
Standard_EXPORT	void SetMinDistance(const Standard_Real& aPrecise);

/**
 * \fn SetSnapType(Sketcher_SnapType theSnap)
 * \brief set current snap type
 * \return void 
 * \param theSnap Sketcher_SnapType
 */
Standard_EXPORT	void SetSnapType (Sketcher_SnapType theSnap);

/**
 * \fn GetSnapType()
 * \brief get current snap type
 * \return Sketcher_SnapType 
 */
Standard_EXPORT	Sketcher_SnapType GetSnapType();		

/**
 * \fn MouseInput(const gp_Pnt2d& thePnt2d)
 * \brief input event handler
 * \return gp_Pnt2d 
 * \param thePnt2d const gp_Pnt2d&
 */
Standard_EXPORT	gp_Pnt2d MouseInput(const gp_Pnt2d& thePnt2d);

/**
 * \fn MouseMove(const gp_Pnt2d& thePnt2d)
 * \brief mouse move handler
 * \return gp_Pnt2d 
 * \param thePnt2d const gp_Pnt2d&
 */
Standard_EXPORT	gp_Pnt2d MouseMove(const gp_Pnt2d& thePnt2d);

/**
 * \fn Cancel()
 * \brief cancel event handler
 * \return void 
 */
Standard_EXPORT	void Cancel();

/**
 * \fn MouseInputException(const gp_Pnt2d& p1, const gp_Pnt2d& thePnt2d, TangentType CType, Standard_Boolean TangentOnly)
 * \brief input event exception handler
 * \return gp_Pnt2d
 * \param p1 const gp_Pnt2d&
 * \param thePnt2d const gp_Pnt2d&
 * \param CType TangentType
 * \param TangentOnly Standard_Boolean
 */
Standard_EXPORT	gp_Pnt2d MouseInputException(const gp_Pnt2d& p1, const gp_Pnt2d& thePnt2d, TangentType CType, Standard_Boolean TangentOnly);

/**
 * \fn MouseMoveException(const gp_Pnt2d& p1, const gp_Pnt2d& thePnt2d, TangentType CType, Standard_Boolean TangentOnly)
 * \brief mouse move exception handler
 * \return gp_Pnt2d
 * \param p1 const gp_Pnt2d&
 * \param thePnt2d const gp_Pnt2d&
 * \param CType TangentType
 * \param TangentOnly Standard_Boolean
 */
Standard_EXPORT	gp_Pnt2d MouseMoveException(const gp_Pnt2d& p1, const gp_Pnt2d& thePnt2d, TangentType CType, Standard_Boolean TangentOnly);

private:
//methods

/**
 * \fn SnapAnalyserEvent()
 * \brief search best point among all possible snaps
 * \return void 
*/
Standard_EXPORT	void SnapAnalyserEvent();

/**
 * \fn AddPoints(const Standard_Real& factor)
 * \brief add point to sequence
 * \return void 
 * \param factor const Standard_Real&
*/
Standard_EXPORT	void AddPoints(const Standard_Real& factor);

/**
 * \fn addSnap(Handle(Sketcher_Snap) theSnap)
 * \brief add theSnap to snap list
 * \return void
 * \param theSnap andle(Sketcher_Snap)
 */
Standard_EXPORT	void addSnap(Handle(Sketcher_Snap) theSnap);

/**
 * \fn SelectCurSnap()
 * \brief get snap from snap list
 * \return void
 */
Standard_EXPORT void SelectCurSnap ();

//members
Handle(AIS_InteractiveContext)			myContext;
Handle(TColStd_HSequenceOfTransient)	data;
Handle(TColStd_HSequenceOfTransient) 	mySnaps;
Handle(Sketcher_Snap)					CurSnap;
Sketcher_SnapType						myCurrentSnap;
gp_Ax3									curCoordinateSystem;

gp_Pnt2d								myPnt2d;
gp_Pnt2d								storedPnt2d;
TangentType								storedTangentType;
Standard_Boolean						isTangent;
Standard_Boolean						isLine;

TColgp_SequenceOfPnt2d					mySeqOfPnt2d;
TColStd_SequenceOfReal 					mySeqOfDistance;
TColStd_SequenceOfReal 					mySeqOfFactor;
TColStd_SequenceOfInteger				mySnapType;
Sketcher_SnapType						mySnapAnType;
Sketcher_SnapType						curSnapAnType;

Standard_Real							minimumSnapDistance;
Standard_Real							bestDist;
Standard_Real							curDist;
};

#endif
