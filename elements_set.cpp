#include "elements_set.h"

ELEMENTS_SET::ELEMENTS_SET() :
	set(0),
	amount(0)
{}
ELEMENTS_SET::~ELEMENTS_SET()
{
	for (unsigned i = 0; i < amount; i++)
		delete set[i];
	delete[] set;
}

bool ELEMENTS_SET::add(ELEMENT* element)
{
	ELEMENT** new_set = new ELEMENT* [amount+1];

	if (new_set)
	{
		for (unsigned i = 0; i < amount; i++)
			new_set[i] = set[i];

		new_set[amount++] = element;

		delete[] set;
		set = new_set;
		return true;
	}
	return false;
}
bool ELEMENTS_SET::remove(ELEMENT* element)
{
	unsigned index;
	for (index = 0; index < amount; index++)
		if (set[index] == element)
			break;

	if (index == amount)
		return false;

	ELEMENT** new_set = new ELEMENT* [amount-1];

	if (new_set)
	{
		unsigned i = 0;
		for (; i < index; i++)
			new_set[i] = set[i];
		delete element;
		for (; i < amount; i++)
			new_set[i] = set[i+1];

		amount--;
		delete[] set;
		set = new_set;
		return true;
	}
	return false;
}
bool ELEMENTS_SET::set_on_top(ELEMENT* element)
{
	unsigned index;
	for (index = 0; index < amount; index++)
		if (set[index] == element)
			break;

	if (index == amount)
		return false;
	
	unsigned i;
	for (i = index; i < amount-1; i++)
		set[i] = set[i+1];

	set[i] = element;
	return true;
}
ELEMENT* ELEMENTS_SET::operator[] (unsigned arg)
{
	if (arg >= amount)
		return 0;
	else return set[arg];
}

