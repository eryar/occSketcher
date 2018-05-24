#ifndef Sketcher_ObjectTypeOfMethod_HeaderFile
#define Sketcher_ObjectTypeOfMethod_HeaderFile

enum Sketcher_ObjectTypeOfMethod 
{ 
 Nothing_Method,
 Point_Method,
 Line2P_Method,
 CircleCenterRadius_Method,
 Circle3P_Method,
 Circle2PTan_Method,
 CircleP2Tan_Method,
 Circle3Tan_Method,
 Arc3P_Method,
 ArcCenter2P_Method,
 BezierCurve_Method,
 Trim_Method
};

enum TangentType 
{ 
 NothingTangent, 
 Line_FirstPnt,
 Line_SecondPnt,
 Circle_CenterPnt 
};

#endif
