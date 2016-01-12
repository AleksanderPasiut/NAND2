#include "master.h"

void InitProcessing(ELEMENT* starting_element, ELEMENTS_SET& elements_set, OUTPUT_LIST* current_list)
{
	if (starting_element)
		current_list->add(starting_element, 0);
	else
	{
		for (unsigned i = 0; i < elements_set.RetAmount(); i++)
			if (elements_set[i]->RetSourceFlag())
				current_list->add(elements_set[i], 0);
	}
}

void SwitchLists(OUTPUT_LIST*& current_list, OUTPUT_LIST*& next_list)
{
	OUTPUT_LIST* tmp = current_list;
	current_list = next_list;
	next_list = tmp;
	next_list->clear();
}

void MASTER::Proceed(ELEMENT* starting_element)
{
	// listy u¿ywane podczas obliczeñ
	OUTPUT_LIST list1;
	OUTPUT_LIST list2;
	OUTPUT_LIST* current_list = &list1;
	OUTPUT_LIST* next_list = &list2;

	// utworzenie listy startowej
	InitProcessing(starting_element, elements_set, current_list);

	// g³ówna pêtla
	unsigned i;
	for (i = 0; i < elements_set.RetAmount() * 2; i++)
	{
		bool state_changed = false;

		// właściwe działanie elementów 
		// (dla NANDÓW jest to albo odczytanie stanów wejściowych i propagacja 
		// albo ustalenie stanu i wysłanie sygnału do nowych elementów (czyli dodanie ich do next_list)
		// dla pozostałych elementów jest to po prostu ustalenie stanu
		for (unsigned j = 0; j < current_list->retAmount(); j++)
			(*current_list)[j]->element->Proceed(next_list, (*current_list)[j]->input);

		// zamiana wskaźników
		SwitchLists(current_list, next_list);

		// je¿eli stan żadnego elementu nie został zmieniony, to czas zakończyć obliczenia
		//if (!state_changed)
		//	break;
	}


	//if (i && i == elements_set.RetAmount() * 2 && !unstable_system_notification)
	//{
	//	MessageBox(hwnd, "Podany układ jest niestabilny.", "NAND2", MB_OK);
	//	unstable_system_notification = true;
	//}
	Paint();
	return;
}