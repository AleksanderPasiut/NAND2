#include "master.h"

void InitProcessing(ELEMENTS_SET& elements_set)
{
	for (unsigned i = 0; i < elements_set.RetAmount(); i++)
		elements_set[i]->computed() = !(elements_set[i]->RetId()) || elements_set[i]->RetSourceFlag();
}

bool NormalProcessing(ELEMENTS_SET& elements_set)
{
	bool ret = false;

	for (unsigned i = 0; i < elements_set.RetAmount(); i++)
		if (!(elements_set[i]->computed()))
		{
			bool flag = elements_set[i]->Proceed(false);
			ret |= flag;
		}

	return ret;
}

void ForcedProcessing(ELEMENTS_SET& elements_set)
{
	for (unsigned i = 0; i < elements_set.RetAmount(); i++)
		if (!(elements_set[i]->computed()))
			elements_set[i]->Proceed(true);
}

bool IsProcessingDone(ELEMENTS_SET& elements_set)
{
	for (unsigned i = 0; i < elements_set.RetAmount(); i++)
		if (!(elements_set[i]->computed()))
			return false;

	return true;
}

void MASTER::Proceed()
{
	InitProcessing(elements_set);

	for (unsigned i = 0; i < elements_set.RetAmount(); i++)
	{
		if (!NormalProcessing(elements_set))
			ForcedProcessing(elements_set);

		if (IsProcessingDone(elements_set))
			break;
	}

	if (!IsProcessingDone(elements_set) && !unstable_system_notification)
	{
		MessageBox(hwnd, "Podany uk³ad jest niestabilny.", "NAND2", MB_OK);
		unstable_system_notification = true;
	}

	Paint();
	return;
}