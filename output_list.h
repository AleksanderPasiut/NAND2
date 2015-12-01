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
	bool remove(ELEMENT*);
	bool remove(ELEMENT*, unsigned);
	void clear();

	OUTPUT* operator[] (unsigned arg) const { return arg < amount ? tab+arg : 0; }
	unsigned retAmount() const { return amount; }
};