//
// Created by Max on 06/05/2022.
//

#include "world.h"
#include "entity.h"

std::vector<entity> world::get_near_entities() const {
    const auto count = memory::read<uint32_t>(m_address + O_WORLD_NEAR_TABLE + 0x8);
    if(!count) { return std::vector<entity>{0}; };

    std::vector<entity> near_entities;
    for(auto index = 0; index < count; index++) {
        auto entity_address = memory::read<address_t>(m_near_table_address + index * 0x8);
        near_entities.emplace_back(entity_address);
    }
    return near_entities;
}

std::vector<entity> world::get_far_entities() const {
    const auto count = memory::read<uint32_t>(m_address + O_WORLD_FAR_TABLE + 0x8);
    if(!count) { return std::vector<entity>{0}; };

    std::vector<entity> far_enitities;
    for(auto index = 0; index < count; index++) {
        auto entity_address = memory::read<address_t>(m_far_table_address + index * 0x8);
        far_enitities.emplace_back(entity_address);
    }
    return far_enitities;
}

std::vector<entity> world::get_slow_entities() const {
    return std::vector<entity>();
}

std::vector<entity> world::get_entities() const {
    std::vector<entity> entities;
    if (config::table_entity_near) {
        auto near_entities = world::get_near_entities();
        entities.insert(entities.end(), near_entities.begin(), near_entities.end());
    }
    if (config::table_entity_far) {
        auto far_entities = world::get_far_entities();
        entities.insert(entities.end(), far_entities.begin(), far_entities.end());
    }
    if (config::table_entity_slow) {
        auto slow_entities = world::get_slow_entities();
        entities.insert(entities.end(), slow_entities.begin(), slow_entities.end());
    }
    return entities;
}

std::vector<entity> world::get_items() const {
    const auto count = memory::read<uint32_t>(m_address + O_WORLD_ITEM_TABLE + 0x8);
    std::vector<entity> items;
    for (uint32_t index = 0; index < count; index++) { // Iterating item table
        const auto flag = memory::read<uint32_t>(m_item_table_address + index * 0x18);
        if (flag == 1) {
            auto item_address = memory::read<address_t>((m_item_table_address + index * 0x18) + 0x8);
            items.push_back(entity(item_address));
        }
    }
    return items;
}

entity world::get_localplayer() {
    const auto t1 = memory::read<address_t>(m_address + O_WORLD_LOCALPLAYER);
    const auto t2 = memory::read<address_t>(t1 + 0x8);
    return entity { t2 - 0xA8 };
}

address_t world::get_address() {
    if(!g_module_address) { return 0; }
    return memory::read<address_t>(g_module_address + O_WORLD);
}