//
// Created by Max on 06/05/2022.
//

#ifndef DAYZ_EXTERNAL_CPP_ENTITY_H
#define DAYZ_EXTERNAL_CPP_ENTITY_H

#include "memory.h"
#include "offset.h"
#include "math.h"
#include "utility.h"

#include <vector>

class network_identity {
private:
    address_t m_address;
public:
    network_identity() : m_address(0) {};

    network_identity(address_t address) : m_address(address) {};
};

class entity {
private:
    address_t m_address;
    address_t m_visual_state_address;
    address_t m_type_address;
    

public:
    entity() : m_address(0), m_visual_state_address(0), m_type_address(0), m_class_name(""), m_network_id(0), m_skeleton_address(0) {};

    entity(address_t address)
        : m_address(address),
        m_visual_state_address(address != 0 ? memory::read<address_t>(address + O_ENTITY_VISUAL_STATE) : 0),
        m_type_address(address != 0 ? memory::read<address_t>(m_address + O_ENTITY_TYPE) : 0),
        m_class_name(address != 0 ? memory::read_arma_string(m_type_address + O_ENTITYTYPE_CLASSNAME) : ""),
        m_network_id(memory::read<uint32_t>(m_address + O_ENTITY_NETWORK_ID)),
        m_skeleton_address(get_skeleton_address()),
        m_anim_address(memory::read<address_t>(m_skeleton_address + 0xA0)),
        m_matrix_address(memory::read<address_t>(m_anim_address + 0xBF0))
    {};

    uint32_t m_network_id;
    std::string m_class_name;
    address_t m_skeleton_address;
    address_t m_anim_address;
    address_t m_matrix_address;

    static const std::vector<uint32_t> HUMAN_BONE_INDICES;
    static const std::vector<uint32_t> HUMAN_JOINT_INDICES;
    static const std::vector<uint32_t> INFECTED_BONE_INDICES;
    static const std::vector<uint32_t> INFECTED_JOINT_INDICES;

    /*
     * 
     */
    address_t get_skeleton_address();

    /**
     * @brief
     */
    vector3 get_position();

    /*
     *
     */
    vector3 get_bone_position(uint32_t index);
};

#endif //DAYZ_EXTERNAL_CPP_ENTITY_H
