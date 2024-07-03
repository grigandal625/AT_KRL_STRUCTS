#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>

inline std::string doubleToString(double value) {
    std::ostringstream oss;
    oss << std::setprecision(std::numeric_limits<double>::max_digits10) << value;
    return oss.str();
}

#endif // UTILS_H