#include <qdialog.h>
#include <AIS_InteractiveContext.hxx>

class DialogTransparency : public QDialog
{
	Q_OBJECT
public:
	DialogTransparency (QWidget * parent=0, const char * name=0, bool modal=true, WFlags f=0 );
	~DialogTransparency();
signals:
	void sendTransparencyChanged(int value);
};
