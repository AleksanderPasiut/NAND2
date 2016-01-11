#pragma once

#include <fstream>
#include <Windows.h>

#include "noncopyable.h"

class MASTER;
class OUTPUT_LIST;

class SAVELOAD
{
	NONCOPYABLE(SAVELOAD)

private:
	MASTER* Master;
	wchar_t* savepath;
	std::fstream fs;
	WINDOWPLACEMENT wpl;

	enum ELEMENT_TYPE
	{
		ELEMENT_TYPE_SOURCE = 1,
		ELEMENT_TYPE_CLOCK = 2,
		ELEMENT_TYPE_NAND = 3,
		ELEMENT_TYPE_OUTPUT = 4,
		ELEMENT_TYPE_COMMENT = 5,
	};

private:
	SAVELOAD() { savepath = 0; }

private:
	template<typename T> inline void write(const T ptr);
	template<typename T> inline T read();

	wchar_t* ReadText();
	void WriteOutputList(const OUTPUT_LIST& ptr);
	void WriteText(const wchar_t* ptr);

	void WriteWindowPos();
	void WriteElements();
	void WriteLinkings();
	void ReadWindowPos();
	void ReadElements();
	void ReadLinkings();
	void FinishWindowPosSetting();

public:
	static SAVELOAD* Create(MASTER* Master);
	~SAVELOAD() { delete[] savepath; }

	bool Load();
	void Save();
};