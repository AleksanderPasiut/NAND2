#pragma once

#include "element.h"

class ELEMENTS_SET
{
private:
	ELEMENT** set;
	unsigned amount;

private:
	ELEMENTS_SET(const ELEMENTS_SET&) {}
	ELEMENTS_SET& operator= (const ELEMENTS_SET&) {}

public:
	ELEMENTS_SET();
	~ELEMENTS_SET();

public:
	bool add(ELEMENT*);
	bool remove(ELEMENT*);
	bool set_on_top(ELEMENT*);
	ELEMENT* operator[] (unsigned);	// zwraca element wed³ug kolejnoœci w elements_set
	ELEMENT* operator() (unsigned); // zwraca element wed³ug id

	unsigned RetAmount() const { return amount; }
};