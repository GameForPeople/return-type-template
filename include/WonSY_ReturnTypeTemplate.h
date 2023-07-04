/*
	Copyright 2023, Won Seong-Yeon. All Rights Reserved.
		KoreaGameMaker@gmail.com
		github.com/GameForPeople
*/


#pragma once


#ifndef WONSY_RETURN_TYPE_TEMPLATE
#define WONSY_RETURN_TYPE_TEMPLATE


#include <source_location>
#include <functional>


// NODISCARD 옵션 적용
#define WONSY_RETURN_TYPE_TEMPLATE_NODISCARD

// UNHANDLED 된 개체의 소멸 시점의 Task호출 옵션 적용
#define WONSY_RETURN_TYPE_TEMPLATE_RUN_UNHANDLED_TASK


namespace WonSY
{
	/*
		ReturnTypeTemplate< T >
		
		C++ 표준에서의 함수의 반환형은 "단순히 반환되는 값의 타입"으로서의 의미만을 가지는데,
		이러한 반환형에 추가적인 기능을 처리하기 위해, ReturnTypeTemplate< T >를 구현합니다.
	
		현재 ReturnTypeTemplate< T >에 구현된 기능은 다음과 같습니다.
	
		0. 반환 위치 확인 ( 필수 )
			- std::source_location을 바탕으로, 반환된 위치를 편하게 확인할 수 있습니다.
			
			[ 장점 ]
				- F10을 통해 디버깅하는 과정에서, 특정 함수의 반환값이 의도와 다른 경우, 반환값의 반환 위치를 빠르게 확인할 수 있습니다.
				- 그 외의, 라이브 이슈 원인 파악 등의 용이성을 가져올 수 있습니다.
	
			[ 단점 ]
				- 아무래도, 일반 리턴 처리보다, 성능 하락이 있을 수 있습니다.
	
		1. NODISCARD 옵션 적용( 선택 )
			- 반환 값이 DISCARD 되었을 때, 빌드 오류를 발생시킵니다.
	
		2. 소멸 시점의 동작 정의
			- ReturnType이 소멸되는 시점에 동작할 Callback을 정의합니다.
	
	*/
	template< typename ReturnValueType >
	// todo : 컨셉, Type Traits 등을 활용하여, _Type에 대한 기본적인 체크들을 진행합니다.
#ifdef WONSY_RETURN_TYPE_TEMPLATE_NODISCARD
	class [[nodiscard]] ReturnTypeTemplate
#else
	class ReturnTypeTemplate
#endif
	{
	public:
		using Type = ReturnValueType;

	public:
#pragma region [ CTOR, DTOR ]
		ReturnTypeTemplate( const ReturnValueType& returnValue, const std::source_location sourceLocation = std::source_location::current() )
			: m_returnValue   ( returnValue    )
			, m_sourceLocation( sourceLocation )
#ifdef WONSY_RETURN_TYPE_TEMPLATE_RUN_UNHANDLED_TASK
			, m_isHandled     ( false          )
#endif
		{
		}

#ifdef WONSY_RETURN_TYPE_TEMPLATE_RUN_UNHANDLED_TASK
		~ReturnTypeTemplate()
		{
			if ( !m_isHandled && ms_unhandledTask )
			{
				ms_unhandledTask( m_returnValue, m_sourceLocation );
			}
		}

		ReturnTypeTemplate( const ReturnTypeTemplate& rhs )
			: m_returnValue   ( rhs.m_returnValue    )
			, m_sourceLocation( rhs.m_sourceLocation )
			, m_isHandled     ( false )
		{
			rhs.m_isHandled = true;
		}

		ReturnTypeTemplate& operator=( const ReturnTypeTemplate& rhs )
		{
			if ( this != &rhs ) 
			{
				m_returnValue    = rhs.m_returnValue;
				m_sourceLocation = rhs.m_sourceLocation;
				m_isHandled      = false;

				rhs.m_isHandled = true;
			}
			return *this;
		}
#endif
#pragma endregion

	public:
#pragma region [ Operator ]
		operator ReturnValueType() const noexcept
		{
			return m_returnValue;
		}

		operator ReturnValueType()
		{
			return m_returnValue;
		}

		bool operator==( const ReturnTypeTemplate& rhs ) const noexcept
		{
			return operator==( rhs.m_returnValue );
		}

		bool operator==( const ReturnValueType& rhs ) const noexcept
		{
			return m_returnValue == rhs;
		}
#pragma endregion

	public:
#pragma region [ Member Var ]
		inline static std::function< void( const ReturnValueType&, const std::source_location& ) > ms_unhandledTask = nullptr;

	private:
		const ReturnValueType      m_returnValue;
		const std::source_location m_sourceLocation;

#ifdef WONSY_RETURN_TYPE_TEMPLATE_RUN_UNHANDLED_TASK
		mutable bool               m_isHandled;
#endif
#pragma endregion

	public:
		const std::source_location& GetSourceLocation() const { return m_sourceLocation; }

		void SetHandlingTrue() const noexcept
		{
#ifdef WONSY_RETURN_TYPE_TEMPLATE_RUN_UNHANDLED_TASK
			m_isHandled = true;
#endif
		}
	};
}

template< typename Type >
using WsyReturnType = WonSY::ReturnTypeTemplate< Type >;

#define MAKE_NAME( name, line ) _##name##_##line

#define MAKE_NAME_WITH_LINE( name, line ) MAKE_NAME( name, line )

#define MAKE_NOT_DUPLICATED_NAME( name  ) MAKE_NAME_WITH_LINE( name, __LINE__ )

// WsyReturnType을 무시할 필요가 있을 경우 사용됩니다.
#define IGNORE_RESULT( ignore_reason )                                \
[[maybe_unused]] const auto MAKE_NOT_DUPLICATED_NAME( IGNORE_VARIABLE )

#endif
