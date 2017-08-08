#ifndef xmlelement_h
#define xmlelement_h
#include <vector>
//microsoft are gimps
//  |
// \|/
//  '
#define GetFirstChild	GetFirstChild	
#define GetNextSibling	GetNextSibling

class XmlElement
{
public:
	 XmlElement();
	~XmlElement();

	const char					*	GetValue		(								);
	void							SetValue		( const char * NewValue			);

	const char					*	GetName			(								);
	void							SetName			( const char * NewName			);

	std::vector<XmlElement*>	*	GetChildren		(								);
	std::vector<XmlElement*>	*	GetChildren		( const char * ChildrensName	);

	XmlElement					*	GetFirstChild	(								);
	XmlElement					*	GetFirstChild	( const char * ChildsName		);

	XmlElement					*	GetNextSibling	(								); 

	XmlElement					*	GetLastChild	(								);
	XmlElement					*	GetLastChild	( const char * ChildsName		);

	void							AddChild		( XmlElement * child			);
	void							AddSibling		( XmlElement * child			);

	friend class XmlInterface;


private:
	char						*	Value;
	char						*	Name;
	XmlElement					*	Parent;				
	std::vector<XmlElement*>		Children;
};

#endif