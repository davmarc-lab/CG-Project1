#pragma once

#include "../Lib.hpp"

class Action
{
    public:
        void (*execute)();

        Action() {}

        void run() { this->execute(); }
};
