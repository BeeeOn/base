#ifndef BEEEON_LOGGABLE_H
#define BEEEON_LOGGABLE_H

#include <typeinfo>
#include <string>

namespace Poco {

class Logger;

}

namespace BeeeOn {

class ClassInfo;

class Loggable {
public:
	Loggable();
	Loggable(const ClassInfo &info);
	Loggable(const std::type_info &info);
	virtual ~Loggable();

	static Poco::Logger &forMethod(const char *name);
	static Poco::Logger &forClass(const ClassInfo &info);
	static Poco::Logger &forClass(const std::type_info &info);

	template <typename T>
	static Poco::Logger &forInstance(const T *i)
	{
		return forClass(typeid(*i));
	}

	static void configureSimple(
			Poco::Logger &logger, const std::string &level);

protected:
	void setupLogger(Poco::Logger *logger = 0) const;

	Poco::Logger &logger() const
	{
		setupLogger();
		return *m_logger;
	}

private:
	const std::string m_name;
	mutable Poco::Logger *m_logger;
};

}

#define BEEEON_CATCH_CHAIN(logger)                                           \
	catch (const Poco::Exception &e) {                                   \
		(logger).log(e, __FILE__, __LINE__);                         \
	}                                                                    \
	catch (const std::exception &e) {                                    \
		(logger).critical(e.what(), __FILE__, __LINE__);             \
	}                                                                    \
	catch (const char *m) {                                              \
		(logger).fatal(m, __FILE__, __LINE__);                       \
	}                                                                    \
	catch (...) {                                                        \
		(logger).fatal("unknown error occured", __FILE__, __LINE__); \
	}

#define BEEEON_CATCH_CHAIN_MESSAGE(logger, message)                          \
	catch (const Poco::Exception &e) {                                   \
		(logger).log(e, __FILE__, __LINE__);                         \
		(logger).error(message, __FILE__, __LINE__);                 \
	}                                                                    \
	catch (const std::exception &e) {                                    \
		(logger).critical(e.what(), __FILE__, __LINE__);             \
		(logger).critical(message, __FILE__, __LINE__);              \
	}                                                                    \
	catch (const char *m) {                                              \
		(logger).fatal(m, __FILE__, __LINE__);                       \
		(logger).fatal(message, __FILE__, __LINE__);                 \
	}                                                                    \
	catch (...) {                                                        \
		(logger).fatal("unknown error occured", __FILE__, __LINE__); \
		(logger).fatal(message, __FILE__, __LINE__);                 \
	}

#define BEEEON_CATCH_CHAIN_RETHROW(logger)                                   \
	catch (const Poco::Exception &e) {                                   \
		(logger).log(e, __FILE__, __LINE__);                         \
		e.rethrow();                                                 \
	}                                                                    \
	catch (const std::exception &e) {                                    \
		(logger).critical(e.what(), __FILE__, __LINE__);             \
		throw Poco::RuntimeException(e.what());                      \
	}                                                                    \
	catch (const char *m) {                                              \
		(logger).fatal(m, __FILE__, __LINE__);                       \
		throw Poco::RuntimeException(m);                             \
	}                                                                    \
	catch (...) {                                                        \
		(logger).fatal("unknown error occured", __FILE__, __LINE__); \
		throw Poco::RuntimeException("unknown error");               \
	}

#define BEEEON_CATCH_CHAIN_ACTION(logger, action)                            \
	catch (const Poco::Exception &e) {                                   \
		(logger).log(e, __FILE__, __LINE__);                         \
		action;                                                      \
	}                                                                    \
	catch (const std::exception &e) {                                    \
		(logger).critical(e.what(), __FILE__, __LINE__);             \
		action;                                                      \
	}                                                                    \
	catch (const char *m) {                                              \
		(logger).fatal(m, __FILE__, __LINE__);                       \
		action;                                                      \
	}                                                                    \
	catch (...) {                                                        \
		(logger).fatal("unknown error occured", __FILE__, __LINE__); \
		action;                                                      \
	}

#define BEEEON_CATCH_CHAIN_ACTION_RETHROW(logger, action)                    \
	catch (const Poco::Exception &e) {                                   \
		(logger).log(e, __FILE__, __LINE__);                         \
		action;                                                      \
		e.rethrow();                                                 \
	}                                                                    \
	catch (const std::exception &e) {                                    \
		(logger).critical(e.what(), __FILE__, __LINE__);             \
		action;                                                      \
		throw Poco::RuntimeException(e.what());                      \
	}                                                                    \
	catch (const char *m) {                                              \
		(logger).fatal(m, __FILE__, __LINE__);                       \
		action;                                                      \
		throw Poco::RuntimeException(m);                             \
	}                                                                    \
	catch (...) {                                                        \
		(logger).fatal("unknown error occured", __FILE__, __LINE__); \
		action;                                                      \
		throw Poco::RuntimeException("unknown error");               \
	}


#endif
