#include "camera.hpp"

namespace Helios::Scene
{
    Camera::Camera()
    {

    }

    auto Camera::set_camera_properties(
        const glm::vec3& position, 
        const glm::vec3& target, 
        const glm::vec3& up
    ) -> void
    {
        position_ = position;
        up_vector = up;
        glm::vec3 forward = glm::normalize(target - position_);
        // think about forward axis and negative_z will coincide after view transform
        rotation_ = glm::qua(forward, negative_z);

        glm::vec3 right = glm::cross(forward, up_vector);
        glm::normalize(right);

        glm::vec3 orth_up = glm::cross(right, forward);
        glm::normalize(orth_up);

        glm::quat up_rotation = glm::qua(rotation_ * orth_up, positive_y);
        rotation_ = up_rotation * rotation_;

        // inverse of the model rotation
        // maps camera space vectors to model vectors
        inv_rotation_ = glm::inverse(rotation_);

    }

    auto Camera::get_view_matrix() -> glm::mat4
    {
        view_matrix = glm::lookAt(position_, position_ + inv_rotation_ * negative_z, inv_rotation_ * positive_y);
        return view_matrix;
    }

    auto Camera::get_projection_matrix()const -> glm::mat4
    {
        //reverse z
        glm::mat4 perspective_matrix = glm::perspective(glm::radians(fov_), aspect_, far_plane, near_plane);
        return perspective_matrix;
    }

   
}