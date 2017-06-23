#include <iostream>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>

#include <Poco/Environment.h>
#include <Poco/Logger.h>
#include <Poco/AutoPtr.h>
#include <Poco/File.h>
#include <Poco/Util/IniFileConfiguration.h>
#include <Poco/Util/MapConfiguration.h>
#include <Poco/Util/LoggingConfigurator.h>

#include "cppunit/TapTestProducer.h"

using namespace std;
using namespace CppUnit;
using namespace Poco;
using namespace Poco::Util;

void setupLogger(const std::string &path)
{
	File file(path);
	LoggingConfigurator logConfig;

	if (!file.exists()) {
		AutoPtr<MapConfiguration> config(new MapConfiguration);

		logConfig.configure(config);
		cerr << "failed to locate logging configuration file "
			<< file.path() << endl;
		cerr << "try: ./test-suite <path-to>/logging.ini" << endl;
	}
	else {
		AutoPtr<IniFileConfiguration> config(
				new IniFileConfiguration(file.path()));
		logConfig.configure(config);
	}
}

static int runWithTapOutput(Test *suite)
{
	TestResult controller;
	BeeeOn::TapTestProducer tapProducer;
	controller.addListener(&tapProducer);

	TestRunner runner;
	runner.addTest(suite);
	runner.run(controller);

	return tapProducer.wasSuccessful()? 0 : 1;
}

static int runStandard(Test *suite, const string &format)
{
	TestRunner runner;
	TestResult controller;
	TestResultCollector collector;

	runner.addTest(suite);
	controller.addListener(&collector);
	runner.run(controller);

	if (format == "human") {
		CompilerOutputter outputter(&collector, cout);
		outputter.write();
	}
	else {
		cerr << "unsupported output format: " << format << endl;
		return -1;
	}

	return collector.wasSuccessful()? 0 : 1;
}

int main(int argc, char **argv)
{
	setupLogger(argc > 1? argv[1] : "logging.ini");

	Test *suite = TestFactoryRegistry::getRegistry().makeTest();

	const string &format = Environment::get("TEST_OUTPUT_FORMAT", "tap");
	if (format == "tap")
		return runWithTapOutput(suite);

	return runStandard(suite, format);
}
