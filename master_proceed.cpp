#pragma once

#include "master.h"

void MASTER::Proceed()
{
	unsigned elements_amount = elements_set.RetAmount();
	bool* done_tab = new bool [elements_amount];

	if (!done_tab)
		return;

	for (unsigned i = 0; i < elements_amount; i++)
	{	done_tab[i] = false;
		elements_set[i]->ResetState();	}

	bool state_changed = true;

	unsigned i;
	for (i = 0; i <= elements_amount && state_changed; i++)
	{
		state_changed = false;

		for (unsigned j = 0; j < elements_amount; j++)
			if (!done_tab[j] && 
				elements_set[j]->ComputeState())
			{	done_tab[j] = true;
				state_changed = true;	}
	}

	delete[] done_tab;
	
	Paint();
	return;
}