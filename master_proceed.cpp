#include "master.h"

void MASTER::Proceed()
{
	// liczba elementów
	unsigned elements_amount = elements_set.RetAmount();

	// utworzenie listy startowej oraz reset niepod³¹czonych elementów
	OUTPUT_LIST compute_list;

	for (unsigned i = 0; i < elements_amount; i++)
	{
		if(elements_set[i]->RetSourceFlag())
			compute_list.add(elements_set[i], 0);

		elements_set[i]->Reset();
	}

	// uruchomienie obliczania
	for (unsigned i = 0; i < 1000 && compute_list.retAmount(); i++)
		compute_list[0]->element->Proceed(compute_list);

	Paint();
	return;
}