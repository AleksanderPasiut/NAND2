#include "master.h"

void MASTER::Proceed()
{
	// reset wewnêtrznych wartoœci input wszystkich elementów
	// oraz ustawienie false dla niepod³¹czonych bramek
	for (unsigned i = 0; i < elements_set.RetAmount(); i++)
		elements_set[i]->Reset();

	// listy u¿ywane podczas obliczeñ
	OUTPUT_LIST list1;
	OUTPUT_LIST list2;
	OUTPUT_LIST* current_list = &list1;
	OUTPUT_LIST* next_list = &list2;

	// utworzenie listy startowej
	for (unsigned i = 0; i < elements_set.RetAmount(); i++)
	{
		if (elements_set[i]->RetSourceFlag())
			current_list->add(elements_set[i], 0);
	}

	// g³ówna pêtla
	unsigned i;
	for (i = 0; i < elements_set.RetAmount()*2; i++)
	{
		bool state_changed = false;

		// aktualizacja stanu przez elementy z current_list
		for (unsigned j = 0; j < current_list->retAmount(); j++)
			state_changed |= (*current_list)[j]->element->UpdateState((*current_list)[j]->input);

		// dopisanie nastêpnych elementów i przekazanie im stanu
		for (unsigned j = 0; j < current_list->retAmount(); j++)
			(*current_list)[j]->element->Proceed(next_list, (*current_list)[j]->input);

		// zamiana wskaŸników
		OUTPUT_LIST* tmp = current_list;
		current_list = next_list;
		next_list = tmp;
		next_list->clear();

		// je¿eli stan ¿adnego elementu nie zosta³ zmieniony, to czas zakoñczyæ obliczenia
		if (!state_changed)
			break;
	}


	if (i && i == elements_set.RetAmount()*2 && !unstable_system_notification)
	{
		MessageBox(hwnd, "Podany uk³ad jest niestabilny.", "NAND2", MB_OK);
		unstable_system_notification = true;
	}
	Paint();
	return;
}