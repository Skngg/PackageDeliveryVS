#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PackageDelivery/init.cpp"
#include "../PackageDelivery/point.cpp"
#include "../PackageDelivery/package.cpp"
#include "../PackageDelivery/init.h"
#include "../PackageDelivery/point.h"
#include "../PackageDelivery/package.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		std::vector<Point> testPoints;

		
		int pid = 0;
		int src = 0;
		int dst = 4;
		int dst1 = 2;
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: Your test code here
			
			initFromFile(testPoints);
			Assert::AreEqual(pid, testPoints.at(0).getID());
			
			

			//
		}
		TEST_METHOD(T2)
		{
			initFromFile(testPoints);
			Assert::AreEqual(src, testPoints.at(0).getAvailablePackages().at(0).getSource());
		}
		TEST_METHOD(T3)
		{
			initFromFile(testPoints);
			Assert::AreEqual(dst, testPoints.at(0).getAvailablePackages().at(0).getDestination());
		}
		TEST_METHOD(T4)
		{
			initFromFile(testPoints);
			int pid1 = 1;
			Assert::AreEqual(pid1, testPoints.at(1).getID());
			//Assert::AreEqual(dst1, testPoints.at(1).getAvailablePackages().at(0).getDestination());
		}
		

	};
}