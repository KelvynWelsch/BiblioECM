#pragma once

#include <string>
#include "../../core/errors/errors.h"

class Statement {
public:
    virtual ~Statement() = default;

    virtual void bind(int index, const std::string& value) = 0;
    virtual void bind(int index, int value) = 0;
    virtual ECM::Result step() = 0;
    virtual bool hasRow() = 0;
    virtual int columnInt(int col) = 0;
    virtual std::string columnText(int col) = 0;

    virtual void reset() = 0;
};

