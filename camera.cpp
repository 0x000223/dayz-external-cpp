//
// Created by Max on 06/05/2022.
//

#include "camera.h"

vector3 camera::get_view_translation() const {
    return memory::read<vector3>(m_address + O_CAMERA_VIEWTRANSLATION);
}

vector3 camera::get_view_right() const {
    return memory::read<vector3>(m_address + O_CAMERA_VIEWRIGHT);
}

vector3 camera::get_view_up() const {
    return memory::read<vector3>(m_address + O_CAMERA_VIEWUP);
}

vector3 camera::get_view_forward() const {
    return memory::read<vector3>(m_address + O_CAMERA_VIEFORWARD);
}

vector3 camera::get_projection_d1() const {
    return memory::read<vector3>(m_address + O_CAMERA_PROJECTION_D1);
}

vector3 camera::get_projection_d2() const {
    return memory::read<vector3>(m_address + O_CAMERA_PROJECTION_D2);
}

vector3 camera::get_viewport_size() const {
    return memory::read<vector3>(m_address + O_CAMERA_VIEWPORTSIZE);
}

vector2 camera::world_to_screen(vector3& position) {
    const auto view_translation = get_view_translation();
    const auto delta = position - view_translation;

    const auto x = delta.dot(camera::get_view_right());
    const auto y = delta.dot(camera::get_view_up());
    const auto z = delta.dot(camera::get_view_forward());

    if (z < 0.1f) {
        return vector2::zero;
    }

    const auto viewport_size = camera::get_viewport_size();

    return vector2 {
        .x = viewport_size.x * (1 + (x / get_projection_d1().x / z)),
        .y = viewport_size.y * (1 - (y / get_projection_d2().y / z))
    };
}