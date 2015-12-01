#include "master.h"

void MASTER::Proceed()
{
	// liczba elementów
	unsigned elements_amount = elements_set.RetAmount();

	// utworzenie listy startowej
	OUTPUT_LIST initial_table;
	for (unsigned i = 0; i < elements_amount; i++)
	{
		elements_set[i]->SetComputationFlag();

		if(elements_set[i]->RetSourceFlag())
			initial_table.add(elements_set[i], 0);
	}

	// uruchomienie obliczania
	for (unsigned i = 0; i < initial_table.retAmount(); i++)
		initial_table[i]->element->Proceed(0, elements_amount);

	Paint();
	return;
}