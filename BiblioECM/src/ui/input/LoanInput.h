#pragma once

#include "../../core/ids/UserId.h"
#include "../../core/ids/ItemId.h"



struct LoanInput {
    UserId user;
    ItemId item;
};

ECM::Expected<LoanInput> lerUserEItem();

