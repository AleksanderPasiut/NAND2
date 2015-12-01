#include "master.h"

void MASTER::Proceed()
{
	// liczba elementów
	unsigned elements_amount = elements_set.RetAmount();

	// utworzenie listy startowej
	OUTPUT_LIST compute_list;

	for (unsigned i = 0; i < elements_amount; i++)
		if(elements_set[i]->RetSourceFlag())
			compute_list.add(elements_set[i], 0);

	// uruchomienie obliczania
	for (unsigned i = 0; i < 2*elements_amount && compute_list.retAmount(); i++)
		compute_list[0]->element->Proceed(compute_list);

	Paint();
	return;
}