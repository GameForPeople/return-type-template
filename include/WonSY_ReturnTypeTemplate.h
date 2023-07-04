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


// NODISCARD �ɼ� ����
#define WONSY_RETURN_TYPE_TEMPLATE_NODISCARD

// UNHANDLED �� ��ü�� �Ҹ� ������ Taskȣ�� �ɼ� ����
#define WONSY_RETURN_TYPE_TEMPLATE_RUN_UNHANDLED_TASK


namespace WonSY
{
	/*
		ReturnTypeTemplate< T >
		
		C++ ǥ�ؿ����� �Լ��� ��ȯ���� "�ܼ��� ��ȯ�Ǵ� ���� Ÿ��"���μ��� �ǹ̸��� �����µ�,
		�̷��� ��ȯ���� �߰����� ����� ó���ϱ� ����, ReturnTypeTemplate< T >�� �����մϴ�.
	
		���� ReturnTypeTemplate< T >�� ������ ����� ������ �����ϴ�.
	
		0. ��ȯ ��ġ Ȯ�� ( �ʼ� )
			- std::source_location�� ��������, ��ȯ�� ��ġ�� ���ϰ� Ȯ���� �� �ֽ��ϴ�.
			
			[ ���� ]
				- F10�� ���� ������ϴ� ��������, Ư�� �Լ��� ��ȯ���� �ǵ��� �ٸ� ���, ��ȯ���� ��ȯ ��ġ�� ������ Ȯ���� �� �ֽ��ϴ�.
				- �� ����, ���̺� �̽� ���� �ľ� ���� ���̼��� ������ �� �ֽ��ϴ�.
	
			[ ���� ]
				- �ƹ�����, �Ϲ� ���� ó������, ���� �϶��� ���� �� �ֽ��ϴ�.
	
		1. NODISCARD �ɼ� ����( ���� )
			- ��ȯ ���� DISCARD �Ǿ��� ��, ���� ������ �߻���ŵ�ϴ�.
	
		2. �Ҹ� ������ ���� ����
			- ReturnType�� �Ҹ�Ǵ� ������ ������ Callback�� �����մϴ�.
	
	*/
	template< typename ReturnValueType >
	// todo : ����, Type Traits ���� Ȱ���Ͽ�, _Type�� ���� �⺻���� üũ���� �����մϴ�.
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

// WsyReturnType�� ������ �ʿ䰡 ���� ��� ���˴ϴ�.
#define IGNORE_RESULT( ignore_reason )                                \
[[maybe_unused]] const auto MAKE_NOT_DUPLICATED_NAME( IGNORE_VARIABLE )

#endif
