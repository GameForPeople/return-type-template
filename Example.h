#pragma once


#include <iostream>
#include <string>


#include "include/WonSY_ReturnTypeTemplate.h"


enum class EReturnType
{
	SUCCESS,

	FAIL,  // 예시로서 큰 의미는 갖고 있지 않습니다.
	ERROR, // 예시로서 큰 의미는 갖고 있지 않습니다.
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