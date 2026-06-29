#ifndef LICENSEGENERATOR_H
#define LICENSEGENERATOR_H

#include <QString>
#include <cstdint>

namespace LicenseGenerator {

QString generateLicenseKey(uint64_t timestamp);

}

#endif
