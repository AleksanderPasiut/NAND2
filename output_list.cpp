#include "output_list.h"

OUTPUT_LIST::OUTPUT_LIST() : tab(0), amount(0) {}

OUTPUT_LIST::~OUTPUT_LIST()
{
	clear();
}

bool OUTPUT_LIST::add(ELEMENT* element, unsigned input)
{
	OUTPUT* new_tab = new OUTPUT[amount+1];

	if (!new_tab)
		return false;

	for (unsigned i = 0; i < amount; i++)
		new_tab[i] = tab[i];

	new_tab[amount] = OUTPUT(element, input);
	amount++;
	delete[] tab;
	tab = new_tab;
	return true;
}

bool OUTPUT_LIST::remove(ELEMENT* element)
{
	unsigned del_amount = 0;
	for (unsigned i = 0; i < amount; i++)
		if (tab[i].element == element)
			del_amount++;

	if (!del_amount)
		return false;

	OUTPUT* new_tab = new OUTPUT[amount-del_amount];

	if (!new_tab)
		return false;

	unsigned j = 0;
	for (unsigned i = 0; i < amount; i++)
		if (tab[i].element != element)
			new_tab[j++] = tab[i];

	amount -= del_amount;
	delete[] tab;
	tab = new_tab;
	return true;
}

bool OUTPUT_LIST::remove(ELEMENT* element, unsigned input)
{
	unsigned del_amount = 0;
	for (unsigned i = 0; i < amount; i++)
		if (tab[i].element == element && tab[i].input == input)
			del_amount++;

	if (!del_amount)
		return false;

	OUTPUT* new_tab = new OUTPUT[amount-del_amount];

	if (!new_tab)
		return false;

	unsigned j = 0;
	for (unsigned i = 0; i < amount; i++)
		if (tab[i].element != element || tab[i].input != input)
			new_tab[j++] = tab[i];

	amount -= del_amount;
	delete[] tab;
	tab = new_tab;
	return true;
}

void OUTPUT_LIST::clear()
{
	delete[] tab;
	tab = 0;
	amount = 0;
	return;
}