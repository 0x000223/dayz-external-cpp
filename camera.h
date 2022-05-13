//
// Created by Max on 06/05/2022.
//

#ifndef DAYZ_EXTERNAL_CPP_CAMERA_H
#define DAYZ_EXTERNAL_CPP_CAMERA_H

#include "offset.h"
#include "memory.h"
#include "math.h"

class camera {
private:
    address_t m_address;
public:
    camera() : m_address(0) {};

    camera(address_t address)
        : m_address(address)
    {};

    /**
     * @brief
     */
    vector3 get_view_translation() const;

    /**
     * @brief
     */
    vector3 get_view_right() const;

    /**
     * @brief
     */
    vector3 get_view_up() const;

    /**
     * @brief
     */
    vector3 get_view_forward() const;

    /**
     * @brief
     */
    vector3 get_projection_d1() const;

    /**
     * @brief
     */
    vector3 get_projection_d2() const;

    /**
     * @brief
     */
    vector3 get_viewport_size() const;

    /**
     * @brief
     */
    vector2 world_to_screen(vector3& position);
};

#endif //DAYZ_EXTERNAL_CPP_CAMERA_H
