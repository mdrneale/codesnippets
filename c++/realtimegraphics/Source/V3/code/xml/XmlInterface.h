#ifndef xmlinterface_h
#define xmlinterface_h
#include "XMLElement.h"

class TiXmlNode;
class TiXmlDocument;

class XmlInterface
{
public:
	XmlInterface		(													){};
	~XmlInterface		(													){};

	void LoadXml		( const char * File , XmlElement* Root		);
	void SaveXml		( const char * File , XmlElement* Root		);

private:
	void FillXml		( TiXmlNode * Node , XmlElement * CurrentElement	);
	void FillTinyXml	( TiXmlNode * Node , XmlElement * CurrentElement	);
};
#endif