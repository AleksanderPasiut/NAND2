#pragma once

#include <fstream>

class MASTER;
class OUTPUT_LIST;

class SAVELOAD
{
private:
	MASTER* Master;
	wchar_t* savepath;
	std::fstream fs;

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
	SAVELOAD(SAVELOAD&) {}
	SAVELOAD& operator= (SAVELOAD&) {}

private:
	template<typename T> inline void write(const T ptr);
	template<typename T> inline T read();

	wchar_t* ReadText();
	void WriteOutputList(const OUTPUT_LIST& ptr);
	void WriteText(const wchar_t* ptr);

public:
	static SAVELOAD* Create(MASTER* Master);
	~SAVELOAD() { delete[] savepath; }

	void Load();
	void Save();
};