#pragma once

enum EVPV_TYPE
{
	EVPV_NONE,
	EVPV_BODY,
	EVPV_CROSS,
	EVPV_CONTROL,
	EVPV_INPUT,
	EVPV_OUTPUT,
};

struct EVPV
{
	EVPV_TYPE type;
	unsigned param;

	EVPV(EVPV_TYPE arg = EVPV_NONE,
		 unsigned p = 0) :
		type(arg),
		param(p) {}
};

enum EL_STATE
{
	EL_STATE_TRUE = 1,
	EL_STATE_FALSE = 0
};

class ELEMENT;
struct EL_INPUT
{
	ELEMENT* target;
	unsigned output;

	EL_INPUT(ELEMENT* in_target = 0,
			 unsigned in_output = 0) :
			target(in_target),
			output(in_output) {}
};