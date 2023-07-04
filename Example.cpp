/*
	Copyright 2023, Won Seong-Yeon. All Rights Reserved.
		KoreaGameMaker@gmail.com
		github.com/GameForPeople
*/


#include "Example.h"


#define DEBUG_BREAK_POINT() [[maybe_unused]] const auto MAKE_NOT_DUPLICATED_NAME( DEBUG_BREAK_POINT ) = 0;


int main()
{
	/*
		ReturnTypeTemplate< T >
		
		아래는 예제 코드입니다.

		예제 코드를 실행하기 전에, "DEBUG_BREAK_POINT" 라인 좌측에 중단점을 걸어주세요.
		이후, [ 디버그 ] - [ 디버깅 시작 ]으로 실행해주세요.
	*/

	// ReturnTypeTemplate< T >의 1번째 기능은, 반환 위치 확인입니다.
	{
		DEBUG_BREAK_POINT(); // 이 라인에서 중단점이 걸렸을 때, F5를 눌러주세요.
		
		const auto Func1Result = Func1(); 

		DEBUG_BREAK_POINT(); // 이 라인에서 중단점이 걸렸을 때, Func1Result의 커서를 갖다대어 값을 확인해주세요.
		// ex ) Func1Result = {m_returnValue=ERROR (2) m_sourceLocation="D:\\Github\\WONSY_NEXT\\return-type-template\\Example.h""_Func1_A": (27, 3) m_isHandled=false }
		// 현재 Func1Result가 어디서 왔는지( m_sourceLocation )을 확인할 수 있습니다. 
		// 이를 바탕으로 디버깅 중 용이성이 증가합니다.

		// 추가적으로 중단점을 걸며 디버깅을 못하는 라이브 환경에서, 리턴된 위치를 출력할수 있습니다.
		if ( Func1Result != EReturnType::SUCCESS )
		{
			// 음~ 뭐 출력했다고 가정합니다.
			Func1Result.GetSourceLocation();
		}
	}

	// ReturnTypeTemplate< T >의 2번째 기능( WONSY_RETURN_TYPE_TEMPLATE_NODISCARD 옵션 활성화 시 )은, 반환값을 처리하지 않을 경우, 빌드 오류를 발생시킵니다.
	{
		// 아래 코드의 주석을 해제할 경우, 빌드 에러가 발생합니다.
		//Func1();
	}

	// ReturnTypeTemplate< T >의 3번째 기능( WONSY_RETURN_TYPE_TEMPLATE_RUN_UNHANDLED_TASK 옵션 활성화 시 )은, 핸들링(임의로 처리)되지 않고 소멸되는 반환값에 대하여, 동작을 정의할 수 있습니다.
	{
		// EReturnType의 unhandled Task를 정의합니다.
		WsyReturnType< EReturnType >::ms_unhandledTask = 
			[]( const EReturnType& returnType, const std::source_location& sourceLocation ) 
			{ 
				// "Func1ResultUnHandled"가 소멸되는 시점에, ms_unhandledTask가 동작합니다.
				DEBUG_BREAK_POINT();

				if ( returnType == EReturnType::SUCCESS ) [[unlikely]]
					return;

				std::cout 
					<< "[WARN] unhandled return type!" 
					<< [ & ]()
						{
							// 으악
							std::string retString;
							retString += "[ Value : ";
							retString += returnType == EReturnType::FAIL ? "FAIL" : "ERROR" ; // 으악
							retString += " , SL : ";
							retString += sourceLocation.file_name();
							retString += '(';
							retString += std::to_string( sourceLocation.line() );
							retString += ")]";
							return retString; 
						}() 
					<< std::endl;
			};

		// 처리되지않은 타입이 소멸될 경우, 설정한 Task( ms_unhandledTask ) 가 호출됩니다.
		{
			[[maybe_unused]] const auto Func1ResultUnHandled = Func1();
		}

		// 처리하는 경우에는 다음과 같이 사용합니다.
		{
			const auto Func1ResultHandled = Func1(); 

			/* Something Else... */

			Func1ResultHandled.SetHandlingTrue();
		}
	}
	
	// 감사합니다.
}

