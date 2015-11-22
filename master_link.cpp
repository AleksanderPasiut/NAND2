#include "master.h"

void MASTER::Link(ELEMENT* element, EVPV evpv)
{
	linking.mark_all_inputs = false;
	linking.mark_all_outputs = false;

	switch(evpv.type)
	{
	case EVPV_INPUT:
		{
			switch(linking.type)
			{
			case EVPV_NONE:
				{
					linking.element = element;
					linking.type = EVPV_INPUT;
					linking.id = evpv.param;
					linking.mark_all_outputs = true;
					break;
				}
			case EVPV_INPUT:
				{
					if (element == linking.element && linking.id == evpv.param)
					{	element->SetInput(0, 0, linking.id);
						linking = LINKING();	}
					break;
				}
			case EVPV_OUTPUT:
				{
					element->SetInput(linking.element, linking.id, evpv.param);
					linking = LINKING();
					break;
				}
			}
			break;
		}
	case EVPV_OUTPUT:
		{
			switch(linking.type)
			{
			case EVPV_NONE:
				{
					linking.element = element;
					linking.type = EVPV_OUTPUT;
					linking.id = evpv.param;
					linking.mark_all_inputs = true;
					break;
				}
			case EVPV_INPUT:
				{
					linking.element->SetInput(element, evpv.param, linking.id);
					linking = LINKING();
					break;
				}
			case EVPV_OUTPUT:
				{
					linking = LINKING();
					break;
				}
			}
			break;
		}
	}
	return;
}

void MASTER::PaintInputs()
{
	D2D1_POINT_2F pt;
	if (linking.element->RetOutputPoint(pt, linking.id))
		target->DrawEllipse(D2D1::Ellipse(pt, 10.0f, 10.0f), brush_set->Red(), 2.0f);

	for (unsigned i = 0; i < elements_set.RetAmount(); i++)
		for (unsigned j = 0; elements_set[i]->RetInputPoint(pt, j); j++)
			target->DrawEllipse(D2D1::Ellipse(pt, 10.0f, 10.0f), brush_set->Green(), 2.0f);
	return;
}

void MASTER::PaintOutputs()
{
	D2D1_POINT_2F pt;
	if (linking.element->RetInputPoint(pt, linking.id))
		target->DrawEllipse(D2D1::Ellipse(pt, 10.0f, 10.0f), brush_set->Red(), 2.0f);

	for (unsigned i = 0; i < elements_set.RetAmount(); i++)
		for (unsigned j = 0; elements_set[i]->RetOutputPoint(pt, j); j++)
			target->DrawEllipse(D2D1::Ellipse(pt, 10.0f, 10.0f), brush_set->Green(), 2.0f);
	return;
}