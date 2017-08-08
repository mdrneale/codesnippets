#include "XmlInterface.h"
#include "tinyxml\tinyxml.h"
#include "..\helper code\HelperCode.h"


void XmlInterface::LoadXml(const char * File , XmlElement* Root)
{
	TiXmlDocument Tiny(File);
	if (Tiny.LoadFile())
	{
		TiXmlNode	*	TinyRoot		=	Tiny.RootElement();
		FillXml( TinyRoot , Root );
	}
	else
	{
		Root = NULL;
	}
};

void XmlInterface::SaveXml(const char * File , XmlElement* Root)
{
	TiXmlDocument doc;  
 	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild( decl ); 
	TiXmlElement * root = new TiXmlElement( Root->GetName() );  
	doc.LinkEndChild( root ); 
	FillTinyXml( root , Root->GetFirstChild() );
	doc.SaveFile( File );
};

void XmlInterface::FillXml		( TiXmlNode * Node ,  XmlElement * CurrentElement )
{
	if ( !Node ) return;
	if ( Node->Type() == TiXmlNode::ELEMENT)
	{
		CurrentElement->SetName( Node->Value() );
		TiXmlNode * pChild = Node->FirstChild();
		if ( pChild )
		{
			if ( pChild->Type() == TiXmlNode::ELEMENT )
			{
				XmlElement* newchild	=	new XmlElement();
				CurrentElement->AddChild( newchild );
				FillXml( pChild , newchild );
			}
			else if ( pChild->Type() == TiXmlNode::TEXT )
			{
				CurrentElement->SetValue( pChild->Value() );
			}
		}
		if ( Node = Node->NextSibling() ) 
		{
			XmlElement* newchild	=	new XmlElement();
			CurrentElement->AddSibling( newchild );
			FillXml( Node , newchild );
		}
	}
};

void XmlInterface::FillTinyXml	( TiXmlNode * Node ,  XmlElement * CurrentElement )
{
	if ( !CurrentElement ) return;
	if ( !CurrentElement->GetName() ) return;

	TiXmlElement * NewElement = new TiXmlElement( CurrentElement->GetName() );  
	Node->LinkEndChild( NewElement );

	if ( CurrentElement->GetValue() )
	{
		NewElement->LinkEndChild( new TiXmlText( CurrentElement->GetValue() ) );  
	}

	if ( CurrentElement->GetFirstChild() )
	{
		FillTinyXml( NewElement , CurrentElement->GetFirstChild() );
	}

	if ( CurrentElement->GetNextSibling() )
	{
		FillTinyXml( Node , CurrentElement->GetNextSibling() );
	}
};