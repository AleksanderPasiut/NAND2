#pragma once

#include "element.h"

class ELEMENTS_SET
{
	NONCOPYABLE(ELEMENTS_SET)

private:
	ELEMENT** set;
	unsigned amount;

public:
	ELEMENTS_SET();
	~ELEMENTS_SET();

public:
	bool add(ELEMENT*);
	bool remove(ELEMENT*);
	bool set_on_top(ELEMENT*);
	void clear();
	ELEMENT* operator[] (unsigned);	// zwraca element wed�ug kolejno�ci w elements_set
	ELEMENT* operator() (unsigned); // zwraca element wed�ug id

	unsigned RetAmount() const { return amount; }
};