//
// Created by Max on 07/05/2022.
//

#ifndef DAYZ_EXTERNAL_CPP_NETWORK_H
#define DAYZ_EXTERNAL_CPP_NETWORK_H

#include "memory.h"
#include "offset.h"
#include "utility.h"

class network {
private:
    address_t m_address;
    address_t m_scoreboard_address;
public:
    network() : m_address(0), m_scoreboard_address(0), m_server_name("") {};
    network(address_t address)
    : m_address(address),
        m_scoreboard_address(memory::read<address_t>(m_address + O_NETWORK_SCOREBOARD)),
        m_server_name(":)")
    {};

    std::string m_server_name;

    /**
     * @brief
     */
    int get_player_count() const;
};

#endif //DAYZ_EXTERNAL_CPP_NETWORK_H
