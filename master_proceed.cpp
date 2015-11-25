#include "master.h"

void MASTER::Proceed()
{
	unsigned elements_amount = elements_set.RetAmount();

	for (unsigned i = 0; i < elements_amount; i++)
		elements_set[i]->ResetState();

	bool change = true;

	unsigned i;
	for (i = 0; i <= elements_amount && change; i++)
	{
		change = false;

		for (unsigned j = 0; j < elements_amount; j++)
				if (elements_set[j]->ComputeState())
					change = true;
	}

	if (i == elements_amount+1)
	{
		if (algorithm_stable)
			MessageBox(hwnd, "Podany uk³ad nie jest stabilny.", "B³¹d", MB_OK);

		algorithm_stable = false;
	}
	else algorithm_stable = true;

	Paint();
	return;
}