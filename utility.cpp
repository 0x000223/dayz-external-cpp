//
// Created by Max on 06/05/2022.
//
#include "utility.h"

std::string memory::read_arma_string(const address_t address) {
    if(!address) {
        return std::string{};
    }
    const auto deref_address = memory::read<address_t>(address);
    int length = memory::read<int>(deref_address + O_STRING_SIZE);
    if (!deref_address || !length || length == 0xcccccccc) { return std::string{}; };
    return std::string {
            reinterpret_cast<char*>(memory::read_bytes(deref_address + O_STRING_START, length))
    };
}