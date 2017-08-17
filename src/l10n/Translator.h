#ifndef BEEEON_TRANSLATOR_H
#define BEEEON_TRANSLATOR_H

#include <iostream>
#include <vector>
#include <string>

#include <Poco/Dynamic/Var.h>

namespace BeeeOn {

class Locale;

class Translator {
public:
	virtual ~Translator();

	/**
	 * Format the message identified by given key.
	 * The method calls formatImpl internally.
	 */
	template <typename... Rest>
	std::string format(
		const std::string &key,
		const Rest & ... rest)
	{
		std::vector<Poco::Dynamic::Var> tmp;
		formatCollect(tmp, rest...);
		return formatImpl(key, tmp);
	}

protected:
	void prepareArg(std::vector<Poco::Dynamic::Var> &tmp, const char *first)
	{
		tmp.push_back(std::string(first));
	}

	template <typename Type>
	void prepareArg(std::vector<Poco::Dynamic::Var> &tmp, const Type &value)
	{
		tmp.push_back(value);
	}

	void formatCollect(std::vector<Poco::Dynamic::Var> &)
	{
	}

	template <typename First, typename... Rest>
	void formatCollect(std::vector<Poco::Dynamic::Var> &tmp,
		const First &first,
		const Rest & ... rest)
	{
		prepareArg(tmp, first);
		formatCollect(tmp, rest...);
	}

	/**
	 * Implements the actual key lookup and formatting.
	 */
	virtual std::string formatImpl(
		const std::string &key,
		const std::vector<Poco::Dynamic::Var> &args) = 0;

};

class TranslatorFactory {
public:
	virtual ~TranslatorFactory();

	/**
	 * Create an instance of Translator for the given locale
	 * and package name.
	 */
	virtual Translator *create(
			const Locale &locale,
			const std::string &name = "") = 0;
};

}

#endif
