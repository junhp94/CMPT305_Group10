#ifndef FUNCTIONAL_UNIT_H
#define FUNCTIONAL_UNIT_H

class FunctionalUnit {
public:
    FunctionalUnit();
    bool isAvailable() const;
    void setBusy();
    void setFree();

private:
    bool available;
};

#endif