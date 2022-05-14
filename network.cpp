//
// Created by Max on 07/05/2022.
//
#include "network.h"

int network::get_player_count() const {
    return memory::read<int>(m_address + O_NETWORK_PLAYERCOUNT);
}
