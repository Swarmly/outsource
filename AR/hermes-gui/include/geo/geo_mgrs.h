#pragma once
#include "gtu-core_global.h"

namespace gtu::geo {

/**
 * Geo point in MGRS.
 */
class GTUCORESHARED_EXPORT MGRS
{
public:
    MGRS(double lat, double lon, long precision = MAX_PRECISION);

    bool is_empty() const;
    bool is_valid() const;

    std::string const &coord() const;
private:
    static const long MAX_PRECISION = 5;

    std::string mgrs_coord_;
};

}
