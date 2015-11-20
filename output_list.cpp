#include "output_list.h"

OUTPUT_LIST::OUTPUT_LIST() : table(0), amount(0) {}
OUTPUT_LIST::~OUTPUT_LIST() { delete[] table; }

bool OUTPUT_LIST::add(ELEMENT* element, unsigned id = 0)
{
	if (OUTPUT* new_table = new OUTPUT[amount+1])
	{
		for (unsigned i = 0; i < amount; i++)
			new_table[i] = table[i];

		new_table[amount++] = OUTPUT(element, id);
		delete[] table;
		table = new_table;
		return true;
	}
	return false;
}
bool OUTPUT_LIST::remove(ELEMENT* element)
{
	unsigned new_amount = amount;
	unsigned i;
	for (i = 0; i < amount; i++)
		if (table[i].element == element)
		{	table[i].element = 0;
			new_amount--;	}

	if (i == amount)
		return false;

	if (OUTPUT* new_table = new OUTPUT[new_amount])
	{
		unsigned j = 0;
		for (i = 0; i < amount; i++)
			if (table[i].element)
				new_table[j++] = table[i];

		delete[] table;
		table = new_table;
		amount = new_amount;
		return true;
	}
	return false;
}
bool OUTPUT_LIST::remove(ELEMENT* element, unsigned port_id)
{
	unsigned index;
	for (index = 0; index < amount; index++)
		if (table[index].element == element &&
			table[index].port_id == port_id)
			break;

	if (index == amount)
		return false;

	if (OUTPUT* new_table = new OUTPUT[amount-1])
	{
		unsigned i = 0;
		for (; i < index; i++)
			new_table[i] = table[i];
		for (; i < amount; i++)
			new_table[i] = table[i-1];

		delete[] table;
		table = new_table;
		amount--;
		return true;
	}
	return false;
}
void OUTPUT_LIST::clear()
{
	delete[] table;
	amount = 0;
	return;
}
const OUTPUT* OUTPUT_LIST::operator[] (unsigned arg) const
{
	if (arg >= amount)
		return 0;

	return table+arg;
}