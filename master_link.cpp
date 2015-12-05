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
					// podwójne klikniêcie na wejœciu: kasowanie po³¹czenia
					if (element == linking.element && linking.id == evpv.param)
					{	element->SetInput(linking.id, 0, 0);
						linking = LINKING();	}
					break;
				}
			case EVPV_OUTPUT:
				{
					// klikniêcie na wyjœciu, potem na wejœciu: utworzenie po³¹czenia
					element->SetInput(evpv.param, linking.element, linking.id);
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
					// klikniêcie na wejœciu, potem na wyjœciu: utworzenie po³¹czenia
					linking.element->SetInput(linking.id, element, evpv.param);
					linking = LINKING();
					break;
				}
			case EVPV_OUTPUT:
				{
					// klikniêcie na wyjœciu, potem na wyjœciu: brak akcji
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

	if (linking.element && linking.element->RetOutputPoint(pt, linking.id))
		target->DrawEllipse(D2D1::Ellipse(pt, ELEMENT::PORT_MARK_RADIUS, ELEMENT::PORT_MARK_RADIUS), brush_set->Red(), 2.0f);

	for (unsigned i = 0; i < elements_set.RetAmount(); i++)
		for (unsigned j = 0; elements_set[i]->RetInputPoint(pt, j); j++)
			if (linking.element != elements_set[i])
				target->DrawEllipse(D2D1::Ellipse(pt, ELEMENT::PORT_MARK_RADIUS, ELEMENT::PORT_MARK_RADIUS), brush_set->Green(), 2.0f);
	return;
}

void MASTER::PaintOutputs()
{
	D2D1_POINT_2F pt;

	if (linking.element && linking.element->RetInputPoint(pt, linking.id))
		target->DrawEllipse(D2D1::Ellipse(pt, ELEMENT::PORT_MARK_RADIUS, ELEMENT::PORT_MARK_RADIUS), brush_set->Red(), 2.0f);

	for (unsigned i = 0; i < elements_set.RetAmount(); i++)
		for (unsigned j = 0; elements_set[i]->RetOutputPoint(pt, j); j++)
			if (linking.element != elements_set[i])
				target->DrawEllipse(D2D1::Ellipse(pt, ELEMENT::PORT_MARK_RADIUS, ELEMENT::PORT_MARK_RADIUS), brush_set->Green(), 2.0f);
	return;
}

void MASTER::RemoveElement(ELEMENT* element)
{
	for (unsigned i = 0; i < elements_set.RetAmount(); i++)
		elements_set[i]->RemoveLinkage(element);

	elements_set.remove(element);
	return;
}