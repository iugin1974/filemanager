// operation.h
#pragma once

class Operation {
  public:
    virtual void execute() = 0;
    virtual ~Operation() = default;
};
