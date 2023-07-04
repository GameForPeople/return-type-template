#pragma once


#include <iostream>
#include <string>


#include "include/WonSY_ReturnTypeTemplate.h"


enum class EReturnType
{
	SUCCESS,

	FAIL,  // ���÷μ� ū �ǹ̴� ���� ���� �ʽ��ϴ�.
	ERROR, // ���÷μ� ū �ǹ̴� ���� ���� �ʽ��ϴ�.
};

using ReturnType = WsyReturnType< EReturnType >;

ReturnType _Func1_A()
{
	if ( false )
		return EReturnType::SUCCESS;

	if ( true )
		return EReturnType::ERROR;
}

ReturnType Func1()
{
	if ( const auto func1AResult = _Func1_A();
		func1AResult != EReturnType::SUCCESS )
		return func1AResult;

	return EReturnType::SUCCESS;
}