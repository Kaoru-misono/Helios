#include "camera.hpp"
#include <algorithm>
#include "logger/logger_marco.hpp"

namespace Helios::Scene
{
    Camera::Camera()
    {

    }

    auto Camera::move(glm::vec3 delta) -> void
    {
        position_ += delta;
    }

    auto Camera::rotate(glm::vec2 delta) -> void
    {
        //need a slower speed
        delta *= 0.005f;

        // limit pitch
        float dot = glm::dot(up_vector, forward());
        if ((dot < -0.99f && delta.x > 0.0f) || // angle nearing 180 degrees
            (dot > 0.99f && delta.x < 0.0f))    // angle nearing 0 degrees
            delta.x = 0.0f;


        // pitch is relative to current sideways rotation
        // yaw happens independently
        // this prevents roll
        glm::quat pitch, yaw;
        pitch = glm::angleAxis(delta.x, positive_x);
        yaw = glm::angleAxis(delta.y, positive_y);

        rotation_ = pitch * rotation_ * yaw;

        inv_rotation_ = glm::conjugate(rotation_);
    }

    auto Camera::zoom(float offset) -> void
    {
        // > 0 = zoom in (decrease FOV by <offset> angles)
        fov_ = std::clamp(fov_ - offset, MIN_FOV, MAX_FOV);
    }

    auto Camera::look_at(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up) -> void
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

        // actually we rotate model, so we need a rotation inverse
        inv_rotation_ = glm::conjugate(rotation_);
    }

    auto Camera::set_camera_properties(
        const glm::vec3& position,
        const glm::vec3& target,
        const glm::vec3& up
    ) -> void
    {
        look_at(position, target, up);
    }

    auto Camera::get_view_matrix() -> glm::mat4
    {
        view_matrix = glm::lookAt(position_, position_ + forward(), up());

        return view_matrix;
    }

    auto Camera::get_projection_matrix()const -> glm::mat4
    {
        //reverse z
        glm::mat4 perspective_matrix = glm::perspective(glm::radians(fov_), aspect_, far_plane, near_plane);
        return perspective_matrix;
    }


}
