#pragma once

#include "gtu-core_global.h"

// https://github.com/andrewstephens75/EnumMapping/blob/master/src/enum_mapping.h

#include <array>
#include <exception>
#include <string>
#include <algorithm>

namespace gtu {

template <class T>
struct NameValuePair {
    using value_type = T;
    const T           value;
    const char* const name;
};

template <class Mapping, class V>
std::string get_name_for_value(Mapping a, V value) {
    auto pos = std::find_if(
        std::begin(a), std::end(a), [&value](const typename Mapping::value_type& t) { return (t.value == value); });
    if (pos != std::end(a)) {
        return pos->name;
    }

    LOG_ERROR_EXT << "Unknown name for enum value: " << value;
    return "";
}

template <class Mapping>
typename Mapping::value_type::value_type get_value_for_name(Mapping a, const std::string& name) {
    auto pos = std::find_if(
        std::begin(a), std::end(a), [&name](const typename Mapping::value_type& t) { return (t.name == name); });
    if (pos != std::end(a)) {
        return pos->value;
    }

    LOG_ERROR_EXT << "Unknown value: " << QString::fromStdString(name);
    return typename Mapping::value_type::value_type();
}
}  // namespace msp
