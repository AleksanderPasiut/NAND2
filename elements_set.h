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
	ELEMENT* operator[] (unsigned);
};