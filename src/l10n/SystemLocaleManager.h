#ifndef BEEEON_SYSTEM_LOCALE_MANAGER_H
#define BEEEON_SYSTEM_LOCALE_MANAGER_H

#include "l10n/LocaleManager.h"

namespace BeeeOn {

class SystemLocaleManager : public LocaleManager {
public:
	Locale parse(const std::string &input) override;
};

}

#endif
