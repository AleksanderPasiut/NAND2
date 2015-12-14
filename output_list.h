#pragma once

class ELEMENT;

struct OUTPUT
{
	ELEMENT* element;
	unsigned input;

	OUTPUT() : element(0), input(0) {}
	OUTPUT(ELEMENT* in_element, unsigned in_input) : element(in_element), input(in_input) {}
};

class OUTPUT_LIST
{
private:
	OUTPUT* tab;
	unsigned amount;

public:
	OUTPUT_LIST();
	~OUTPUT_LIST();

	bool add(ELEMENT*, unsigned);
	bool add_if_new(ELEMENT*, unsigned);
	bool remove_first_element();
	bool remove(ELEMENT* target);
	bool remove(ELEMENT* target, unsigned id);
	void clear();

	OUTPUT* operator[] (unsigned arg) const { return arg < amount ? tab+arg : 0; }
	unsigned retAmount() const { return amount; }
};