#include "master.h"

void MASTER::Proceed()
{
	unsigned elements_amount = elements_set.RetAmount();

	for (unsigned i = 0; i < elements_amount; i++)
	{
		for (unsigned j = 0; j < elements_amount; j++)
			elements_set[j]->SetComputationFlag();

		for (unsigned j = 0; j < elements_amount; j++)
			elements_set[j]->RecursiveStateCompute();
	}

	Paint();
	return;
}