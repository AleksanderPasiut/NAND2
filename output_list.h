#pragma once

class ELEMENT;

struct OUTPUT
{
	ELEMENT* element;
	unsigned port_id;

	OUTPUT(ELEMENT* in_element = 0, unsigned in_port_id = 0) : 
		element(in_element), port_id(in_port_id) {}
};

class OUTPUT_LIST
{
private:
	OUTPUT* table;
	unsigned amount;

public:
	OUTPUT_LIST();
	~OUTPUT_LIST();

	bool add(ELEMENT*, unsigned);
	bool remove(ELEMENT*);
	bool remove(ELEMENT*, unsigned);
	void clear();
	const OUTPUT* operator[] (unsigned) const;
	unsigned Amount() const { return amount; }
};