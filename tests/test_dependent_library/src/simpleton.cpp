#include <stdexcept>

#include "simpleton.hpp"

void function(const h5gt::Object& obj) {
    if (!obj.isValid()) {
        throw std::exception();
    }
}
