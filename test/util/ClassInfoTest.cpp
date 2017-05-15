#include <cppunit/extensions/HelperMacros.h>

#include "cppunit/BetterAssert.h"
#include "util/ClassInfo.h"

using namespace std;

namespace BeeeOn {

class ClassInfoTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(ClassInfoTest);
	CPPUNIT_TEST(testEmptyConstructor);
	CPPUNIT_TEST(testConstruct);
	CPPUNIT_TEST(testId);
	CPPUNIT_TEST_SUITE_END();

public:
	void testEmptyConstructor();
	void testConstruct();
	void testId();
};

CPPUNIT_TEST_SUITE_REGISTRATION(ClassInfoTest);

class TestObject0 {
};

class TestObject1 {
};

void ClassInfoTest::testEmptyConstructor()
{
	ClassInfo info0 = ClassInfo::forClass<TestObject0>();
	ClassInfo info1 = ClassInfo::forClass<TestObject0>();
	ClassInfo info2 = ClassInfo::forClass<TestObject1>();

	CPPUNIT_ASSERT(info0 == info1);
	CPPUNIT_ASSERT(info0 != info2);
	CPPUNIT_ASSERT(info1 != info2);
}

void ClassInfoTest::testConstruct()
{
	TestObject0 o0;
	TestObject1 o1;

	ClassInfo info0(typeid(o0));
	ClassInfo info1(typeid(o1));
	ClassInfo info2(typeid(TestObject0));
	ClassInfo info3(typeid(TestObject1));
	ClassInfo info4(o0);
	ClassInfo info5(o1);
	ClassInfo info6 = ClassInfo::forPointer(&o0);
	ClassInfo info7 = ClassInfo::forPointer(&o1);

	CPPUNIT_ASSERT(info0 == info2);
	CPPUNIT_ASSERT(info2 == info4);
	CPPUNIT_ASSERT(info4 == info6);

	CPPUNIT_ASSERT(info1 == info3);
	CPPUNIT_ASSERT(info3 == info5);
	CPPUNIT_ASSERT(info5 == info7);

	CPPUNIT_ASSERT(info0 != info1);
	CPPUNIT_ASSERT(info2 != info3);
	CPPUNIT_ASSERT(info4 != info5);
	CPPUNIT_ASSERT(info6 != info7);
}

void ClassInfoTest::testId()
{
	TestObject0 o;
	ClassInfo info0(typeid(o));
	ClassInfo info1(typeid(TestObject0));
	ClassInfo info2(o);
	ClassInfo info3 = ClassInfo::forPointer(&o);

	CPPUNIT_ASSERT_EQUAL("N6BeeeOn11TestObject0E", info0.id());
	CPPUNIT_ASSERT_EQUAL("N6BeeeOn11TestObject0E", info1.id());
	CPPUNIT_ASSERT_EQUAL("N6BeeeOn11TestObject0E", info2.id());
	CPPUNIT_ASSERT_EQUAL("N6BeeeOn11TestObject0E", info3.id());
}

}
