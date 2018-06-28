#if !defined(CSFDB)
#error CSFDB precompiler directive is mandatory for CasCade 
#endif

#include "Translate.h"

#include "Application.h"

#include <qdir.h>
#include <qlayout.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qvaluelist.h>
#include <qfiledialog.h>
#include <qapplication.h>

#include <AIS_Shape.hxx>
#include <AIS_InteractiveObject.hxx>

#include <FSD_File.hxx>

#include <ShapeSchema.hxx>
#include <Storage_Data.hxx>
#include <Storage_Root.hxx>
#include <Storage_HSeqOfRoot.hxx>
#include <PTopoDS_HShape.hxx>
#include <PTColStd_PersistentTransientMap.hxx>
#include <PTColStd_TransientPersistentMap.hxx>

#include <IGESControl_Reader.hxx>
#include <IGESControl_Writer.hxx>
#include <IGESControl_Controller.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>
#include <STEPControl_StepModelType.hxx>
#include <Interface_Static.hxx>
#include <Interface_TraceFile.hxx>

#include <StlAPI_Writer.hxx>
#include <VrmlAPI_Writer.hxx>

#include <MgtBRep.hxx>
#include <MgtBRepAbs_TriangleMode.hxx>

#include <BRepTools.hxx>
#include <BRep_Tool.hxx>
#include <BRep_Builder.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_HSequenceOfShape.hxx>

#include <Geom_Line.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Surface.hxx>


#include <Standard_ErrorHandler.hxx>
#include <Standard_CString.hxx>

// ---------------------------- TranslateDlg -----------------------------------------

class TranslateDlg : public QFileDialog
{
public:
	TranslateDlg( QWidget* = 0, const char* = 0, bool = true );
	~TranslateDlg();
	int                   getMode() const;
	void                  setMode( const int );
    void                  addMode( const int, const QString& );
    void                  clear();

private:
	QComboBox*            myBox;
    QValueList<int>       myList;
};

TranslateDlg::TranslateDlg( QWidget* parent, const char* name, bool modal )
: QFileDialog( parent, name, modal )
{
    QGroupBox* paramGroup = new QGroupBox( 1, Qt::Vertical, this );
    paramGroup->setFrameStyle( QFrame::NoFrame );
    if ( paramGroup->layout() )
        paramGroup->layout()->setMargin( 0 );
	myBox = new QComboBox( paramGroup );
	addWidgets( 0, paramGroup, 0 );
}

TranslateDlg::~TranslateDlg()
{
}

int TranslateDlg::getMode() const
{
    if ( myBox->currentItem() < 0 || myBox->currentItem() > (int)myList.count() - 1 )
        return -1;
    else
        return *myList.at( myBox->currentItem() );
}

void TranslateDlg::setMode( const int mode )
{
    int idx = myList.findIndex( mode );
    if ( idx >= 0 )
        myBox->setCurrentItem( idx );
}

void TranslateDlg::addMode( const int mode, const QString& name )
{
    myBox->show();
    myBox->insertItem( name );
    myList.append( mode );
    myBox->updateGeometry();
    updateGeometry();
}

void TranslateDlg::clear()
{
    myList.clear();
    myBox->clear();
    myBox->hide();
    myBox->updateGeometry();
    updateGeometry();
}

// ---------------------------- Translate -----------------------------------------

Translate::Translate( QObject* parent, const char* name )
: QObject( parent, name ),
myDlg( 0 )
{
}

Translate::~Translate()
{
    if ( myDlg )
        delete myDlg;
}

QString Translate::info() const
{
    return myInfo;
}

bool Translate::importModel( const int format, const Handle(AIS_InteractiveContext)& ic )
{
    myInfo = QString();
    QString fileName = selectFileName( format, true );
    if ( fileName.isEmpty() )
        return true;

    if ( !QFileInfo( fileName ).exists() )
    {
        myInfo = tr( "INF_TRANSLATE_FILENOTFOUND" ).arg( fileName );
        return false;
    }

    QApplication::setOverrideCursor( Qt::waitCursor );
    Handle(TopTools_HSequenceOfShape) shapes = importModel( format, fileName );
    QApplication::restoreOverrideCursor();

    if ( shapes.IsNull() || !shapes->Length() )
        return false;

    for ( int i = 1; i <= shapes->Length(); i++ )
        ic->Display( new AIS_Shape( shapes->Value( i ) ), false );
    ic->UpdateCurrentViewer();
    return true;
}

Handle(TopTools_HSequenceOfShape) Translate::importModel( const int format, const QString& file )
{
    Handle(TopTools_HSequenceOfShape) shapes;
    try {
        switch ( format )
        {
        case FormatBREP:
            shapes = importBREP( file );
            break;
        case FormatIGES:
            shapes = importIGES( file );
            break;
        case FormatSTEP:
            shapes = importSTEP( file );
            break;
        case FormatCSFDB:
            shapes = importCSFDB( file );
            break;
        }
    } catch ( Standard_Failure ) {
        shapes.Nullify();
    }
    return shapes;
}

bool Translate::exportModel( const int format, const Handle(AIS_InteractiveContext)& ic )
{
    myInfo = QString();
    QString fileName = selectFileName( format, false );
    if ( fileName.isEmpty() )
        return true;

    Handle(TopTools_HSequenceOfShape) shapes = getShapes( ic );
    if ( shapes.IsNull() || !shapes->Length() )
        return false;

    QApplication::setOverrideCursor( Qt::waitCursor );
    bool stat = exportModel( format, fileName, shapes );
    QApplication::restoreOverrideCursor();

    return stat;
}

bool Translate::exportModel( const int format, const QString& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{
    bool status;
    try {
        switch ( format )
        {
        case FormatBREP:
            status = exportBREP( file, shapes );
            break;
        case FormatIGES:
            status = exportIGES( file, shapes );
            break;
        case FormatSTEP:
            status = exportSTEP( file, shapes );
            break;
        case FormatCSFDB:
            status = exportCSFDB( file, shapes );
            break;
        case FormatSTL:
            status = exportSTL( file, shapes );
            break;
        case FormatVRML:
            status = exportVRML( file, shapes );
            break;
        }
    } catch ( Standard_Failure ) {
        status = false;
    }
    return status;
}

Handle(TopTools_HSequenceOfShape) Translate::getShapes( const Handle(AIS_InteractiveContext)& ic )
{
    Handle(TopTools_HSequenceOfShape) aSequence;
    Handle(AIS_InteractiveObject) picked;
    for ( ic->InitCurrent(); ic->MoreCurrent(); ic->NextCurrent() )
    {
        Handle(AIS_InteractiveObject) obj = ic->Current();
        if ( obj->IsKind( STANDARD_TYPE( AIS_Shape ) ) )
        {
            TopoDS_Shape shape = Handle(AIS_Shape)::DownCast(obj)->Shape();
            if ( aSequence.IsNull() )
                aSequence = new TopTools_HSequenceOfShape();
            aSequence->Append( shape );
        }
    }
    return aSequence;
}

/*!
    Selects a file from standard dialog acoording to selection 'filter'
*/
QString Translate::selectFileName( const int format, const bool import )
{
    TranslateDlg* theDlg = getDialog( format, import );

    int ret = theDlg->exec();
    
    qApp->processEvents();

    QString file;
    if ( ret != QDialog::Accepted )
        return file;

    file = theDlg->selectedFile();
    if ( !QFileInfo( file ).extension().length() )
    {
        QString selFilter = theDlg->selectedFilter();
        int idx = selFilter.find( "(*." );
        if ( idx != -1 )
        {
            QString tail = selFilter.mid( idx + 3 );
            idx = tail.find( " " );
            if ( idx == -1 )
                idx = tail.find( ")" );
            QString ext = tail.left( idx );
            if ( ext.length() )
                file += QString( "." ) + ext;
        }
    }

    return file;
}

TranslateDlg* Translate::getDialog( const int format, const bool import )
{
    if ( !myDlg )
        myDlg = new TranslateDlg( 0, 0, true );

    if ( format < 0 )
        return myDlg;

    QString formatFilter = tr( QString().sprintf( "INF_FILTER_FORMAT_%d", format ) );
    QString allFilter = tr( "INF_FILTER_FORMAT_ALL" );

    QStringList filters;
    filters.append( formatFilter );

    if ( import )
        filters.append( allFilter );

    myDlg->setFilters( filters );

	if ( import )
    {
	    myDlg->setCaption( tr( "INF_APP_IMPORT" ) );
        myDlg->setMode( QFileDialog::ExistingFile );
    }
	else
    {
		myDlg->setCaption( tr( "INF_APP_EXPORT" ) );
        myDlg->setMode( QFileDialog::AnyFile );
    }

    myDlg->setSelection( "" );
    myDlg->clear();
    if ( !import )
    {
        switch ( format )
        {
            case FormatSTEP:
                myDlg->addMode( STEPControl_ManifoldSolidBrep,      tr( "INF_BREP_MOIFOLD" ) );
	            myDlg->addMode( STEPControl_FacetedBrep,            tr( "INF_BREP_FACETED" ) );
	            myDlg->addMode( STEPControl_ShellBasedSurfaceModel, tr( "INF_BREP_SHELL" ) );
	            myDlg->addMode( STEPControl_GeometricCurveSet,      tr( "INF_BREP_CURVE" ) );
                break;
            case FormatCSFDB:
                myDlg->addMode( MgtBRepAbs_WithTriangle,    tr( "INF_TRIANGLES_YES" ) );
	            myDlg->addMode( MgtBRepAbs_WithoutTriangle, tr( "INF_TRIANGLES_NO" ) );
                break;
        }
    }

    return myDlg;
}

// ----------------------------- Import functionality -----------------------------

Handle(TopTools_HSequenceOfShape) Translate::importBREP( const QString& file )
{
	Handle(TopTools_HSequenceOfShape) aSequence;
    TopoDS_Shape aShape;
	BRep_Builder aBuilder;

	Standard_Boolean result = BRepTools::Read( aShape, (Standard_CString)file.latin1(), aBuilder );
	if ( result )
    {
	    aSequence = new TopTools_HSequenceOfShape();
		aSequence->Append( aShape );
    }
    return aSequence;
}

Handle(TopTools_HSequenceOfShape) Translate::importIGES( const QString& file )
{
    Handle(TopTools_HSequenceOfShape) aSequence;
    IGESControl_Reader Reader;
    int status = Reader.ReadFile( (Standard_CString)file.latin1() );

    if ( status == IFSelect_RetDone )
    {
        aSequence = new TopTools_HSequenceOfShape();
        Reader.TransferRoots();
        TopoDS_Shape aShape = Reader.OneShape();
        aSequence->Append( aShape );
    }
	return aSequence;
}

Handle(TopTools_HSequenceOfShape) Translate::importSTEP( const QString& file )
{
	Handle(TopTools_HSequenceOfShape) aSequence;

	STEPControl_Reader aReader;
	IFSelect_ReturnStatus status = aReader.ReadFile( (Standard_CString)file.latin1() );
	if ( status == IFSelect_RetDone )
    {
	    Interface_TraceFile::SetDefault();
	    bool failsonly = false;
	    aReader.PrintCheckLoad( failsonly, IFSelect_ItemsByEntity );

	    int nbr = aReader.NbRootsForTransfer();
	    aReader.PrintCheckTransfer( failsonly, IFSelect_ItemsByEntity );
	    for ( Standard_Integer n = 1; n <= nbr; n++ )
	    {
	        bool ok = aReader.TransferRoot( n );
	        int nbs = aReader.NbShapes();
	        if ( nbs > 0 )
            {
	            aSequence = new TopTools_HSequenceOfShape();
	            for ( int i = 1; i <= nbs; i++ )
                {
		            TopoDS_Shape shape = aReader.Shape( i );
		            aSequence->Append( shape );
	            }
            }
        }
    }
	return aSequence;
}

Handle(TopTools_HSequenceOfShape) Translate::importCSFDB( const QString& file )
{
	Handle(TopTools_HSequenceOfShape) aSequence;

    // Check file type
    if ( FSD_File::IsGoodFileType( (Standard_CString)file.latin1() ) != Storage_VSOk )
	    return aSequence;

    static FSD_File fileDriver;
    TCollection_AsciiString aName( (Standard_CString)file.latin1() );
    if ( fileDriver.Open( aName, Storage_VSRead ) != Storage_VSOk )
        return aSequence;

    Handle(ShapeSchema) schema = new ShapeSchema();
    Handle(Storage_Data) data  = schema->Read( fileDriver );
    if ( data->ErrorStatus() != Storage_VSOk )
        return aSequence;

    fileDriver.Close();

    aSequence = new TopTools_HSequenceOfShape();
    Handle(Storage_HSeqOfRoot) roots = data->Roots();
    for ( int i = 1; i <= roots->Length() ; i++ )
    {
        Handle(Storage_Root) r = roots->Value( i );
        Handle(Standard_Persistent) p = r->Object();
        Handle(PTopoDS_HShape) aPShape = Handle(PTopoDS_HShape)::DownCast(p);
        if ( !aPShape.IsNull() )
        {
	        PTColStd_PersistentTransientMap aMap;
	        TopoDS_Shape aTShape;
            MgtBRep::Translate( aPShape, aMap, aTShape, MgtBRepAbs_WithTriangle );
            aSequence->Append( aTShape );
        }
    }

    return aSequence;
}

// ----------------------------- Export functionality -----------------------------

bool Translate::exportBREP( const QString& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{
    if ( shapes.IsNull() || shapes->IsEmpty() )
        return false;

    TopoDS_Shape shape = shapes->Value( 1 );
    return BRepTools::Write( shape, (Standard_CString)file.latin1() ); 
}

bool Translate::exportIGES( const QString& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{
    if ( shapes.IsNull() || shapes->IsEmpty() )
        return false;

    IGESControl_Controller::Init();
	IGESControl_Writer writer( Interface_Static::CVal( "XSTEP.iges.unit" ),
                               Interface_Static::IVal( "XSTEP.iges.writebrep.mode" ) );
 
	for ( int i = 1; i <= shapes->Length(); i++ )
		writer.AddShape ( shapes->Value( i ) );
	writer.ComputeModel();
	return writer.Write( (Standard_CString)file.latin1() );
}

bool Translate::exportSTEP( const QString& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{
    if ( shapes.IsNull() || shapes->IsEmpty() )
        return false;

    TranslateDlg* theDlg = getDialog( -1, false );
    STEPControl_StepModelType type = (STEPControl_StepModelType)theDlg->getMode();
    if ( type < 0 )
        return false;
    
    IFSelect_ReturnStatus status;

    if ( type == STEPControl_FacetedBrep && !checkFacetedBrep( shapes ) )
    {
        myInfo = tr( "INF_FACET_ERROR" );
        return false;
    }

    STEPControl_Writer writer;
	for ( int i = 1; i <= shapes->Length(); i++ )
    {
		status = writer.Transfer( shapes->Value( i ), type );
        if ( status != IFSelect_RetDone )
            return false;
    }

    status = writer.Write( (Standard_CString)file.latin1() );

    switch ( status )
    {
    case IFSelect_RetError:
        myInfo = tr( "INF_DATA_ERROR" );
        break;
    case IFSelect_RetFail:
        myInfo = tr( "INF_WRITING_ERROR" );
        break;
    case IFSelect_RetVoid:
        myInfo = tr( "INF_NOTHING_ERROR" );
        break;
    }
    return status == IFSelect_RetDone;
}

bool Translate::exportCSFDB( const QString& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{
    if ( shapes.IsNull() || shapes->IsEmpty() )
        return false;

    TranslateDlg* theDlg = getDialog( -1, false );
    MgtBRepAbs_TriangleMode type = (MgtBRepAbs_TriangleMode)theDlg->getMode();
    if ( type < 0 )
        return false;

    static FSD_File fileDriver;

    Handle(ShapeSchema) schema = new ShapeSchema();
    Handle(Storage_Data) data  = new Storage_Data();
    data->ClearErrorStatus();

    data->SetApplicationName( TCollection_ExtendedString( "Sample Import / Export" ) );
    data->SetApplicationVersion( "1" );
    data->SetDataType( TCollection_ExtendedString( "Shapes" ) );
    data->AddToUserInfo( "Storing a persistent set of shapes in a flat file" );
    data->AddToComments( TCollection_ExtendedString( "Application is based on CasCade 5.0 Professional" ) );

    if ( fileDriver.Open( (Standard_CString)file.latin1(), Storage_VSWrite ) != Storage_VSOk )
    {
        myInfo = tr( "INF_TRANSLATE_ERROR_CANTSAVEFILE" ).arg( file );
        return false;
    }

    PTColStd_TransientPersistentMap aMap;
	for ( int i = 1; i <= shapes->Length(); i++ )
	{
		TopoDS_Shape shape = shapes->Value( i );
		if ( shape.IsNull() )
		{
			myInfo = tr( "INF_TRANSLATE_ERROR_INVALIDSHAPE" );
			return false;
        }

        Handle(PTopoDS_HShape) pshape = MgtBRep::Translate( shape, aMap, type );
		TCollection_AsciiString objName = TCollection_AsciiString( "Object_" ) + TCollection_AsciiString( i );
		data->AddRoot( objName, pshape );
	}

    schema->Write( fileDriver, data );
    fileDriver.Close();

    if ( data->ErrorStatus() != Storage_VSOk )
    {
        myInfo = tr( "INF_TRANSLATE_ERROR_CANTSAVEDATA" );
        return false;
    } 
    return true;
}

bool Translate::exportSTL( const QString& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{
    if ( shapes.IsNull() || shapes->IsEmpty() )
        return false;

	TopoDS_Compound res;
	BRep_Builder builder;
	builder.MakeCompound( res );

	for ( int i = 1; i <= shapes->Length(); i++ )
	{
		TopoDS_Shape shape = shapes->Value( i );
		if ( shape.IsNull() ) 
		{
			myInfo = tr( "INF_TRANSLATE_ERROR_INVALIDSHAPE" );
			return false;
        }
		builder.Add( res, shape );
	}

	StlAPI_Writer writer;
	writer.Write( res, (Standard_CString)file.latin1() );

    return true;
}

bool Translate::exportVRML( const QString& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{
    if ( shapes.IsNull() || shapes->IsEmpty() )
        return false;

	TopoDS_Compound res;
	BRep_Builder builder;
	builder.MakeCompound( res );

	for ( int i = 1; i <= shapes->Length(); i++ )
	{
		TopoDS_Shape shape = shapes->Value( i );
		if ( shape.IsNull() )
		{
			myInfo = tr( "INF_TRANSLATE_ERROR_INVALIDSHAPE" );
			return false;
        }
		builder.Add( res, shape );
	}

	VrmlAPI_Writer writer;
	writer.Write( res, (Standard_CString)file.latin1() );

    return true;
}

bool Translate::checkFacetedBrep( const Handle(TopTools_HSequenceOfShape)& shapes )
{
	bool err = false;
	for ( int i = 1; i <= shapes->Length(); i++ )
	{
	    TopoDS_Shape shape = shapes->Value( i );
        for ( TopExp_Explorer fexp( shape, TopAbs_FACE ); fexp.More() && !err; fexp.Next() )
		{
		    Handle(Geom_Surface) surface = BRep_Tool::Surface( TopoDS::Face( fexp.Current() ) );
		    if ( !surface->IsKind( STANDARD_TYPE( Geom_Plane ) ) )
		        err = true;
		}
        for ( TopExp_Explorer eexp( shape, TopAbs_EDGE ); eexp.More() && !err; eexp.Next() )
		{
		    Standard_Real fd, ld;
		    Handle(Geom_Curve) curve = BRep_Tool::Curve( TopoDS::Edge( eexp.Current() ), fd, ld );
		    if ( !curve->IsKind( STANDARD_TYPE( Geom_Line ) ) )
		        err = true;
		}
	}
	return !err;
}
