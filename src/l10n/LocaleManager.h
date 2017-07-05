#ifndef BEEEON_LOCALE_MANAGER_H
#define BEEEON_LOCALE_MANAGER_H

#include <string>

#include "l10n/Locale.h"

namespace BeeeOn {

class LocaleManager {
public:
	virtual Locale parse(const std::string &input) = 0;
};

}

#endif
