#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\ZariDB/utils/String.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZariDBTests
{		
	TEST_CLASS(StringTest)
	{
	public:
		
		TEST_METHOD(StringTestToString)
		{
			utils::String str(L"soska");
			Assert::IsTrue(lstrcmpW(L"soska", str.ToString()), L"Ti ohuyel");
		}

	};
}