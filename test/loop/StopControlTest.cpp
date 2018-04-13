#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>

#include "cppunit/BetterAssert.h"
#include "loop/StopControl.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class StopControlTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(StopControlTest);
	CPPUNIT_TEST(testShouldStop);
	CPPUNIT_TEST_SUITE_END();
public:
	void testShouldStop();
};

CPPUNIT_TEST_SUITE_REGISTRATION(StopControlTest);

void StopControlTest::testShouldStop()
{
	StopControl control;

	CPPUNIT_ASSERT(!control.shouldStop());

	control.requestStop();
	CPPUNIT_ASSERT(control.shouldStop());

	control.clear();
	CPPUNIT_ASSERT(!control.shouldStop());

	control.requestWakeup();
	CPPUNIT_ASSERT(!control.shouldStop());

	control.requestStop();
	CPPUNIT_ASSERT(control.shouldStop());
}

}
