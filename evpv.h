#pragma once

enum EVPV_TYPE
{
	EVPV_NONE,
	EVPV_BODY,
	EVPV_CROSS,
	EVPV_CONTROL,
	EVPV_INPUT,
	EVPV_OUTPUT
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
	EL_STATE_NO_INPUT,
	EL_STATE_UNKNOWN,
	EL_STATE_TRUE,
	EL_STATE_FALSE
};