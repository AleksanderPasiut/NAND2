#include "saveload.h"

#include "master.h"

/* 
Schemat zapisu do pliku:
 - WINDOWPLACEMENT
 - sns.x (float)
 - sns.y (float)
 - sns.scale (float)
 - element_amount (unsigned)

	Pêtla elementów:
		- id (unsigned);
		- pos.x (float)
		- pos.y (float)
		- type_of_element (ELEMENT_TYPE)

		W zale¿noœci od typu elementu:
			* ELEMENT_SOURCE
				- state (EL_STATE)
			* ELEMENT_CLOCK
				- elapse (unsigned)
			* ELEMENT_NAND
				- input_amount (unsigned)
			* ELEMENT_COMMENT
				- text_length (unsigned, rozmiar tekstu wraz z koñcz¹cym zerem)
				- text (wchar_t array, koñczona zerem)

	Pêtla po³aczeñ:
		- id Ÿród³a
		- output Ÿród³a
		- id celu
		- input celu
	|| koñcz¹ce 0 (4 bajty)
*/

// funkcje pomocnicze
template<typename T> inline void SAVELOAD::write(const T obj)
{
	fs.write(reinterpret_cast<const char*>(&obj), sizeof(T));
	return;
}
template<typename T> inline T SAVELOAD::read()
{
	T ret;
	fs.read(reinterpret_cast<char*>(&ret), sizeof(T));
	return ret;
}

wchar_t* SAVELOAD::ReadText()
{
	unsigned text_length = read<unsigned>();
	wchar_t* ret = new wchar_t [text_length];
	fs.read(reinterpret_cast<char*>(ret), text_length*sizeof(wchar_t));
	return ret;
}
void SAVELOAD::WriteOutputList(const OUTPUT_LIST& ptr)
{
	for (unsigned i = 0; i < ptr.retAmount(); i++)
	{
		write<ELEMENT*>(ptr[i]->element);
		write<unsigned>(ptr[i]->input);
	}

	write<unsigned>(0);
	return;
}
void SAVELOAD::WriteText(const wchar_t* text)
{
	if (!text)
	{
		write<unsigned>(1);
		write<wchar_t>(0);
	}
	else
	{
		unsigned size = static_cast<unsigned>(wcslen(text))+1;
		write<unsigned>(size);
		fs.write(reinterpret_cast<const char*>(text), size*sizeof(wchar_t));
	}
	return;
}

void SAVELOAD::WriteWindowPos()
{
	wpl.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(Master->hwnd, &wpl);
	write<WINDOWPLACEMENT>(wpl);
	write<float>(Master->sns.x);
	write<float>(Master->sns.y);
	write<float>(Master->sns.scale);
	return;
}
void SAVELOAD::WriteElements()
{
	write<unsigned>(Master->elements_set.RetAmount());
	for (unsigned i = 0; i < Master->elements_set.RetAmount(); i++)
	{
		write<unsigned>(Master->elements_set[i]->RetId());
		D2D1_POINT_2F pos = Master->elements_set[i]->RetPos();
		write<float>(pos.x);
		write<float>(pos.y);

		if (ELEMENT_SOURCE* ptr = dynamic_cast<ELEMENT_SOURCE*>(Master->elements_set[i]))
		{
			write<ELEMENT_TYPE>(ELEMENT_TYPE_SOURCE);
			write<EL_STATE>(ptr->RetState());
		}
		else if (ELEMENT_CLOCK* ptr = dynamic_cast<ELEMENT_CLOCK*>(Master->elements_set[i]))
		{
			write<ELEMENT_TYPE>(ELEMENT_TYPE_CLOCK);
			write<unsigned>(ptr->RetElapse());
		}
		else if (ELEMENT_NAND* ptr = dynamic_cast<ELEMENT_NAND*>(Master->elements_set[i]))
		{
			write<ELEMENT_TYPE>(ELEMENT_TYPE_NAND);
			write<unsigned>(ptr->RetInputAmount());
		}
		else if (ELEMENT_OUTPUT* ptr = dynamic_cast<ELEMENT_OUTPUT*>(Master->elements_set[i]))
		{
			write<ELEMENT_TYPE>(ELEMENT_TYPE_OUTPUT);
		}
		else if (ELEMENT_COMMENT* ptr = dynamic_cast<ELEMENT_COMMENT*>(Master->elements_set[i]))
		{
			write<ELEMENT_TYPE>(ELEMENT_TYPE_COMMENT);
			WriteText(ptr->RetText());
		}
	}
	return;
}
void SAVELOAD::WriteLinkings()
{
	for (unsigned i = 0; i < Master->elements_set.RetAmount(); i++)
	{
		if (ELEMENT_SOURCE* ptr = dynamic_cast<ELEMENT_SOURCE*>(Master->elements_set[i]))
		{
			const OUTPUT_LIST& optlst = ptr->RetOutputList();
			for (unsigned j = 0; j < optlst.retAmount(); j++)
			{
				write<unsigned>(ptr->RetId());
				write<unsigned>(0);
				write<unsigned>(optlst[j]->element->RetId());
				write<unsigned>(optlst[j]->input);
			}
		}
		else if (ELEMENT_CLOCK* ptr = dynamic_cast<ELEMENT_CLOCK*>(Master->elements_set[i]))
		{
			const OUTPUT_LIST& optlst = ptr->RetOutputList();
			for (unsigned j = 0; j < optlst.retAmount(); j++)
			{
				write<unsigned>(ptr->RetId());
				write<unsigned>(0);
				write<unsigned>(optlst[j]->element->RetId());
				write<unsigned>(optlst[j]->input);
			}
		}
		else if (ELEMENT_NAND* ptr = dynamic_cast<ELEMENT_NAND*>(Master->elements_set[i]))
		{
			const OUTPUT_LIST& optlst = ptr->RetOutputList();
			for (unsigned j = 0; j < optlst.retAmount(); j++)
			{
				write<unsigned>(ptr->RetId());
				write<unsigned>(0);
				write<unsigned>(optlst[j]->element->RetId());
				write<unsigned>(optlst[j]->input);
			}
		}
	}
	write<unsigned>(0);
	return;
}
void SAVELOAD::ReadWindowPos()
{
	wpl = read<WINDOWPLACEMENT>();
	SetWindowPlacement(Master->hwnd, &wpl);
	Master->sns.x = read<float>();
	Master->sns.y = read<float>();
	Master->sns.scale = read<float>();
	Master->sns.RefreshMatrix();
	return;
}
void SAVELOAD::ReadElements()
{
	unsigned elements_amount = read<unsigned>();
	for(unsigned i = 0; i < elements_amount; i++)
	{
		unsigned id = read<unsigned>();
		float pos_x = read<float>();
		float pos_y = read<float>();
		ELEMENT_TYPE type = read<ELEMENT_TYPE>();

		ELEMENT* added_element = 0;
		switch(type)
		{
		case ELEMENT_TYPE_SOURCE:
			{
				EL_STATE state = read<EL_STATE>();
				ELEMENT_SOURCE* element = ELEMENT_SOURCE::Create(Master->target,
																 Master->brush_set,
																 pos_x,
																 pos_y,
																 id);
				element->SetState(state);
				added_element = reinterpret_cast<ELEMENT*>(element);
				break;
			}
		case ELEMENT_TYPE_CLOCK:
			{
				unsigned elapse = read<unsigned>();
				ELEMENT_CLOCK* element = ELEMENT_CLOCK::Create(Master->target,
															   Master->brush_set,
															   pos_x,
															   pos_y,
															   id,
															   elapse,
															   Master);
				added_element = reinterpret_cast<ELEMENT*>(element);
				break;
			}
		case ELEMENT_TYPE_NAND:
			{
				unsigned input_amount = read<unsigned>();
				ELEMENT_NAND* element = ELEMENT_NAND::Create(Master->target,
															 Master->brush_set,
															 pos_x,
															 pos_y,
															 id,
															 input_amount);
				added_element = reinterpret_cast<ELEMENT*>(element);
				break;
			}
		case ELEMENT_TYPE_OUTPUT:
			{
				ELEMENT_OUTPUT* element = ELEMENT_OUTPUT::Create(Master->target,
																 Master->brush_set,
																 pos_x,
																 pos_y,
																 id);
				added_element = reinterpret_cast<ELEMENT*>(element);
				break;
			}
		case ELEMENT_TYPE_COMMENT:
			{
				wchar_t* text = ReadText();
				ELEMENT_COMMENT* element = ELEMENT_COMMENT::Create(Master->target,
																   Master->brush_set,
																   pos_x,
																   pos_y,
																   text,
																   Master->dwfactory);
				added_element = reinterpret_cast<ELEMENT*>(element);
				delete[] text;
				break;
			}
		}

		Master->elements_set.add(added_element);
	}
	return;
}
void SAVELOAD::ReadLinkings()
{
	for(;;)
	{
		unsigned source_id = read<unsigned>();		if (!source_id || !fs) break;
		unsigned source_output = read<unsigned>();
		unsigned dest_id = read<unsigned>();
		unsigned dest_input = read<unsigned>();

		ELEMENT* element_source = Master->elements_set(source_id);
		ELEMENT* element_dest = Master->elements_set(dest_id);

		if (element_source && element_dest && element_source != element_dest)
		{
			element_source->AddOutput(source_output, element_dest, dest_input);
			element_dest->SetInput(dest_input, element_source, source_output);
		}
	}

	Master->Proceed(0);
	return;
}
void SAVELOAD::FinishWindowPosSetting()
{
	Master->target->Resize(MASTER::RetHwndClientSize(Master->hwnd));
	Master->sns.RefreshMatrix();
	ShowWindow(Master->hwnd, wpl.showCmd);
	return;
}

// g³ówne funkcje
SAVELOAD* SAVELOAD::Create(MASTER* ptr)
{
	SAVELOAD* ret = new SAVELOAD;

	if (!ret)
		return 0;

	ret->Master = ptr;

	// wyodrêbnienie œcie¿ki do aplikacji
	wchar_t* command_line = GetCommandLineW();
	int savepath_length = 1;
	while(command_line[savepath_length] != L'"')
		savepath_length++;
	while(command_line[savepath_length] != L'\\')
		savepath_length--;
	savepath_length += 7;
	ret->savepath = new wchar_t [savepath_length];

	if (!ret->savepath)
	{	delete ret;
		return 0;	}

	memset(ret->savepath, 0, savepath_length*sizeof(wchar_t));
	memcpy(ret->savepath, command_line+1, savepath_length*sizeof(wchar_t));
	memcpy(ret->savepath+(savepath_length-7), L"config", 7*sizeof(wchar_t));
	return ret;
}
bool SAVELOAD::Load() 
{
	fs.open(savepath, std::fstream::in | std::fstream::binary);

	if (!fs)
		return false;

	ReadWindowPos();
	ReadElements();
	ReadLinkings();
	FinishWindowPosSetting();

	fs.close();
	return true;
}
void SAVELOAD::Save()
{
	fs.open(savepath, std::fstream::out | std::fstream::binary);

	if (!fs)
		return;

	WriteWindowPos();
	WriteElements();
	WriteLinkings();

	fs.close();
	return;
}