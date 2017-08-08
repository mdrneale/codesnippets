#include "XMLElement.h"
#include "..\helper code\HelperCode.h"

XmlElement::XmlElement() : Value(NULL),	Name(NULL), Parent(NULL)
{

}

XmlElement::~XmlElement()
{
	if ( Value )
	{
		delete []Value;
		Value = NULL;
	}

	if ( Name )
	{
		delete []Name;
		Name=NULL;
	}
	for (unsigned int a = 0; a < Children.size(); ++a)
	{
		if (Children[a])
		{
			delete Children[a];
			Children[a] = NULL;
		}
	}
}

const char					*	XmlElement::GetValue		(							)
{
	return Value;
};

void						XmlElement::SetValue		( const char * NewValue			)
{
	if ( Value )
	{
		delete [] Value; 
	}
	Value = CreateString(NewValue);
};

const char					*	XmlElement::GetName			(							)
{
	return Name;
};

void						XmlElement::SetName			( const char * NewName		)
{
	if ( Name )
	{
		delete [] Name; 
	}
	Name = CreateString(NewName);
};

std::vector<XmlElement*>	*	XmlElement::GetChildren		(							)
{
	return &Children;
};

std::vector<XmlElement*>	*	XmlElement::GetChildren		( const char * ChildrensName		)
{
	std::vector<XmlElement*>	*	kids	=	new std::vector<XmlElement*>;

	for (unsigned int a = 0; a < Children.size(); ++a)
	{
		if ( !strcmp( Children[a]->GetName() , ChildrensName ) )
		{
			kids->push_back(Children[a]);
		}
	}
	return kids;
};

XmlElement				*	XmlElement::GetFirstChild	(							)
{
	if (Children.size())
	{
		return Children[0];
	}
	return NULL;
};

XmlElement				*	XmlElement::GetFirstChild	( const char * ChildsName			)
{
	for (unsigned int a = 0; a < Children.size(); ++a)
	{
		if ( !strcmp( Children[a]->GetName() , ChildsName ) )
		{
			return Children[a];
		}
	}

	return NULL;
};

XmlElement				*	XmlElement::GetLastChild	(							)
{
	if (Children.size())
	{
		return Children[Children.size()-1];
	}
	return NULL;
};

XmlElement				*	XmlElement::GetLastChild	( const char * ChildsName			)
{
	if (Children.size())
	{
		for (unsigned int a = (unsigned int)(Children.size()-1); a > 0; --a)
		{
			if ( !strcmp( Children[a]->GetName() , ChildsName ) )
			{
				return Children[a];
			}
		}
	}
	return NULL;
};


void						XmlElement::AddChild		( XmlElement * child		)
{
	child->Parent	=	this;
	Children.push_back(child);
};

void						XmlElement::AddSibling		( XmlElement * child		)
{
	if (Parent)
	{
		Parent->AddChild( child );
	}
}

XmlElement				*	XmlElement::GetNextSibling			(							)
{
	if (Parent)
	{
		for (unsigned int a = 0; a < Parent->Children.size()-1; ++a)
		{
			if (Parent->Children[a]==this)
			{
				return Parent->Children[a+1];
			}
		}
	}
	return NULL;
}