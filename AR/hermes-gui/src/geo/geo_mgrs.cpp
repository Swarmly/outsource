#include "geo/geo_mgrs.h"
#include "geo/mgrs/mgrs.h"
#include <algorithm>
#include <cmath>
#include <cstring>

namespace gtu::geo {

MGRS::MGRS(double lat, double lon, long precision) {
    mgrs_coord_.resize(128);
    std::fill(mgrs_coord_.begin(), mgrs_coord_.end(), '\0');
    Convert_Geodetic_To_MGRS(
        lat * (M_PI/180),
        lon * (M_PI/180), precision,
        mgrs_coord_.data()
    );
    mgrs_coord_.resize(std::strlen(mgrs_coord_.data()));
}

bool MGRS::is_empty() const {
    return this->mgrs_coord_.empty();
}

bool MGRS::is_valid() const {
    // TODO - what constitutes a valid MGRS?
    return !is_empty();
}

std::string const& MGRS::coord() const {
    return this->mgrs_coord_;
}

}
