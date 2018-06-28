#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <qobject.h>

#include <AIS_InteractiveContext.hxx>
#include <TopTools_HSequenceOfShape.hxx>

class TranslateDlg;

class Translate: public QObject
{
	Q_OBJECT

public:
    enum { FormatBREP, FormatIGES, FormatSTEP, FormatCSFDB, FormatVRML, FormatSTL };

	Translate( QObject*, const char* = 0 );
	~Translate();

    bool                              importModel( const int, const Handle(AIS_InteractiveContext)& );
    bool                              exportModel( const int, const Handle(AIS_InteractiveContext)& );

    QString                           info() const;

private:
    Handle(TopTools_HSequenceOfShape) importModel( const int, const QString& );
    bool                              exportModel( const int, const QString&,
                                              const Handle(TopTools_HSequenceOfShape)& );
    QString                           selectFileName( const int, const bool );
    TranslateDlg*                     getDialog( const int, const bool );
    Handle(TopTools_HSequenceOfShape) getShapes( const Handle(AIS_InteractiveContext)& );

	Handle(TopTools_HSequenceOfShape) importBREP( const QString& );
	Handle(TopTools_HSequenceOfShape) importIGES( const QString& );
	Handle(TopTools_HSequenceOfShape) importSTEP( const QString& );
	Handle(TopTools_HSequenceOfShape) importCSFDB( const QString& );

	bool exportBREP( const QString&, const Handle(TopTools_HSequenceOfShape)& );
	bool exportIGES( const QString&, const Handle(TopTools_HSequenceOfShape)& );
	bool exportSTEP( const QString&, const Handle(TopTools_HSequenceOfShape)& );
	bool exportCSFDB( const QString&, const Handle(TopTools_HSequenceOfShape)& );
	bool exportSTL( const QString&, const Handle(TopTools_HSequenceOfShape)& );
	bool exportVRML( const QString&, const Handle(TopTools_HSequenceOfShape)& );

    bool checkFacetedBrep( const Handle(TopTools_HSequenceOfShape)& );

private:
    TranslateDlg*                     myDlg;
    QString                           myInfo;
};

#endif
