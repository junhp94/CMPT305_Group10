#include "functional_unit.h"

FunctionalUnit::FunctionalUnit() : available(true) {}

bool FunctionalUnit::isAvailable() const {
    return available;
}

void FunctionalUnit::setBusy() {
    available = false;
}

void FunctionalUnit::setFree() {
    available = true;
}