//
// Created by Max on 06/05/2022.
//

#include "entity.h"

const std::vector<uint32_t> entity::HUMAN_BONE_INDICES = {
    0, 19, 20, 21, 24,	// Pelvis to head
    94, 97, 99,			// Left arm
    61, 63, 65,			// Right arm
    9, 12, 14, 16,		// Left leg
    1, 4, 6, 8			// Right leg
};

const std::vector<uint32_t> entity::HUMAN_JOINT_INDICES = {
    0, 19, 19, 20, 20, 21, 21, 24,
    21, 94, 94, 97, 97, 99,
    21, 61, 61, 63, 63, 65,
    0, 9, 9, 12, 12, 14, 14, 16,
    0, 1, 1, 4, 4, 6, 6, 8
};

const std::vector<uint32_t> entity::INFECTED_BONE_INDICES = {
    0, 15, 16, 19, 21,	// Pelvis to head
    56, 59, 60,			// Left arm
    24, 25, 27,			// Right arm
    9, 12, 13, 14,		// Left leg
    1, 4, 6, 7			// Right leg
};

const std::vector<uint32_t> entity::INFECTED_JOINT_INDICES = {
    0, 15, 15, 16, 16, 19, 19, 21,
    21, 56, 56, 59, 59, 60,
    21, 24, 24, 25, 25, 27,
    0, 9, 9, 12, 12, 13, 13, 14,
    0, 1, 1, 4, 4, 6, 6, 7
};

address_t entity::get_skeleton_address() {
    if (!m_network_id) {
        return memory::read<address_t>(m_address + O_ENTITY_INFECTED_SKELETON);
    }
    return memory::read<address_t>(m_address + O_ENTITY_PLAYER_SKELETON);
}

vector3 entity::get_position() {
    return memory::read<vector3>(m_visual_state_address + O_VISUAL_STATE_POSITION);
}

vector3 entity::get_bone_position(const uint32_t index) {
    const auto matrix_a = memory::read<matrix44>(m_visual_state_address + 0x8);
    const auto matrix_b = memory::read<vector3>(m_matrix_address + 0x54 + index * sizeof(matrix44));
    return vector3 {
        .x = (matrix_a.m[0] * matrix_b.x) + (matrix_a.m[3] * matrix_b.y) + (matrix_a.m[6] * matrix_b.z) + matrix_a.m[9],
        .y = (matrix_a.m[1] * matrix_b.x) + (matrix_a.m[4] * matrix_b.y) + (matrix_a.m[7] * matrix_b.z) + matrix_a.m[10],
        .z = (matrix_a.m[2] * matrix_b.x) + (matrix_a.m[5] * matrix_b.y) + (matrix_a.m[8] * matrix_b.z) + matrix_a.m[11],
    };
}
