//
// Created by Max on 06/05/2022.
//

#ifndef DAYZ_EXTERNAL_CPP_WORLD_H
#define DAYZ_EXTERNAL_CPP_WORLD_H

#include "memory.h"
#include "offset.h"
#include "math.h"
#include "entity.h"
#include "camera.h"

#include <vector>

/**
 * @brief Represents instance of World object
 */
class world {
private:
    address_t m_address;
    address_t m_near_table_address;
    address_t m_far_table_address;
    address_t m_slow_table_address;
    address_t m_item_table_address;

public:
    world()
        : m_address(0), m_near_table_address(0), m_far_table_address(0), m_slow_table_address(0), m_item_table_address(0), m_camera(0), m_localplayer(0)
    {};

    world(address_t address)
        : m_address(address),
          m_near_table_address(memory::read<address_t>(m_address + O_WORLD_NEAR_TABLE)),
          m_far_table_address(memory::read<address_t>(m_address + O_WORLD_FAR_TABLE)),
          m_slow_table_address(memory::read<address_t>(m_address + O_WORLD_SLOW_TABLE)),
          m_item_table_address(memory::read<address_t>(m_address + O_WORLD_ITEM_TABLE)),
          m_camera(memory::read<address_t>(m_address + O_WORLD_CAMERA)),
          m_localplayer(get_localplayer())
    {};

    camera m_camera;
    entity m_localplayer;
    std::vector<entity> m_near_entities;
    std::vector<entity> m_far_entities;

    /**
     * @return
     */
    std::vector<entity> get_near_entities() const;

    /**
     *
     * @return
     */
    std::vector<entity> get_far_entities() const;

    /**
     *
     * @return
     */
    std::vector<entity> get_slow_entities() const;

    /**
     *
     * @return
     */
    std::vector<entity> get_items() const;

    /*
     * @brief
     */
    entity get_localplayer();

    /**
     * @brief Read(DayZ_x64 + World Offset)
     */
    static address_t get_address();
};

#endif //DAYZ_EXTERNAL_CPP_WORLD_H
