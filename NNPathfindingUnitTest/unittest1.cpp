#include "stdafx.h"
#include "CppUnitTest.h"

#include "../TownieTry3/NNHandler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NNPathfindingUnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(ThisTestWillPass)
		{
			Assert::AreEqual(0, 0);
		}

		TEST_METHOD(ThisTestWillFail)
		{
			Assert::AreEqual(0, 1);
		}

	};
}