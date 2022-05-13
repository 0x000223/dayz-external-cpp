//
// Created by Max on 06/05/2022.
//

#ifndef DAYZ_EXTERNAL_CPP_UTILITY_H
#define DAYZ_EXTERNAL_CPP_UTILITY_H

#include "memory.h"
#include "offset.h"

#include <string>

namespace memory {

    /**
     *
     * @param address
     * @return
     */
    std::string read_arma_string(address_t address);
};

#endif //DAYZ_EXTERNAL_CPP_UTILITY_H
