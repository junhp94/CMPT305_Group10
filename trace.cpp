#include "trace.h"

bool Trace::isValid() {
    if (type == Type::INVALID) {
        return false;
    }
    return true;
}