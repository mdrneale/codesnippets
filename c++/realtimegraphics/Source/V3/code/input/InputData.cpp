#include "InputData.h"
#include <list>
#include "..\xml\tinyxml\tinyxml.h"
#include <dinput.h>

inputdata::inputdata(){}

inputdata::~inputdata(){}

inputdevices inputdata::loadfromxml(const char *const file, std::vector<action> &tofill)
{
	TiXmlDocument doc(file);
	if (doc.LoadFile())
	{
		TiXmlNode* root  = doc.RootElement();
		TiXmlNode* child = NULL;
		std::string inputtype = root->FirstChild("inputtype")->ToElement()->GetText();
		inputdevices contype=e_nodevice;
		if (inputtype == "keyboard")
		{
			contype = e_keyboard;
		}
		else if (inputtype == "mouse")
		{
			contype = e_mouse;
		}
		else if (inputtype == "xboxcontroller")
		{
			contype = e_xboxcontroller;
		}
		else if (inputtype == "ps3controller")
		{
			contype = e_ps3controller;
		}
		else if (inputtype == "ps3guitar")
		{
			contype = e_ps3guitar;
		}
		else if (inputtype == "ps3drums")
		{
			contype = e_ps3drums;
		}
		else
		{
			contype = e_anydevice;
		}

		for (TiXmlNode* current=root->FirstChild("action"); current!=NULL; current=current->NextSibling())
		{		
			action temp;
			temp.actionfor = contype;
			if (current->FirstChild("key")->ToElement()->Attribute("ascii"))
			{
				temp.isascii=true;
			}
			else
			{
				temp.isascii=false;
			}
			temp.message=current->FirstChild("message")->ToElement()->GetText();
			if (temp.isascii)
			{
				if ( contype == e_keyboard || contype == e_anydevice)
				{
					temp.keyname2=AsciiToDIK(*(current->FirstChild("key")->ToElement()->GetText()));
				}
			}
			else
			{
				temp.keyname=current->FirstChild("key")->ToElement()->GetText();
			}
			tofill.push_back(temp);
		}
		return contype;
	}
	return e_nodevice;
}

char inputdata::AsciiToDIK(char ascii)
{
	char DIK=ascii;
	switch (ascii)
	{
	case 'a': 
	case 'A':
		DIK = DIK_A;
		break;

	case 'b': 
	case 'B':
		DIK = DIK_B;
		break;

	case 'c': 
	case 'C':
		DIK = DIK_C;
		break;

	case 'd': 
	case 'D':
		DIK = DIK_D;
		break;

	case 'e': 
	case 'E':
		DIK = DIK_E;
		break;

	case 'f': 
	case 'F':
		DIK = DIK_F;
		break;

	case 'g': 
	case 'G':
		DIK = DIK_G;
		break;

	case 'h': 
	case 'H':
		DIK = DIK_H;
		break;

	case 'i': 
	case 'I':
		DIK = DIK_I;
		break;

	case 'j': 
	case 'J':
		DIK = DIK_J;
		break;

	case 'k': 
	case 'K':
		DIK = DIK_K;
		break;

	case 'l': 
	case 'L':
		DIK = DIK_L;
		break;

	case 'm': 
	case 'M':
		DIK = DIK_M;
		break;

	case 'n': 
	case 'N':
		DIK = DIK_N;
		break;

	case 'o': 
	case 'O':
		DIK = DIK_O;
		break;

	case 'p': 
	case 'P':
		DIK = DIK_P;
		break;

	case 'q': 
	case 'Q':
		DIK = DIK_Q;
		break;

	case 'r': 
	case 'R':
		DIK = DIK_R;
		break;

	case 's': 
	case 'S':
		DIK = DIK_S;
		break;

	case 't': 
	case 'T':
		DIK = DIK_T;
		break;

	case 'u': 
	case 'U':
		DIK = DIK_U;
		break;

	case 'v': 
	case 'V':
		DIK = DIK_V;
		break;

	case 'w': 
	case 'W':
		DIK = DIK_W;
		break;

	case 'x': 
	case 'X':
		DIK = DIK_X;
		break;

	case 'y': 
	case 'Y':
		DIK = DIK_Y;
		break;

	case 'z': 
	case 'Z':
		DIK = DIK_Z;
		break;
	}
	return DIK;
}