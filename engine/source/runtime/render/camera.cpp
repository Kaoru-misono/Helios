#include "camera.hpp"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "logger/logger_marco.hpp"
#include "window/window.hpp"
namespace Helios::Scene
{
    Camera::Camera()
    {
        Window::instance().registerOnWindowSizeFunc(
            [&](int width, int height) {
                set_aspect((float)width/(float)height);
            });
    }
    auto Camera::update() -> void
    {
        if (matrix_dirty)
        {
            calculate_view_martix();
            calculate_proj_matrix();
            matrix_dirty = false;
        }
    }

    auto Camera::move(glm::vec3 delta) -> void
    {
        eye_position_ += delta;
        matrix_dirty = true;
    }

    auto Camera::rotate(glm::vec2 delta) -> void
    {
        delta *= 0.005f;

        float dot = glm::dot(up_, forward());
        if ((dot < -0.99f && delta.x > 0.0f) || // angle nearing 180 degrees
            (dot > 0.99f && delta.x < 0.0f))    // angle nearing 0 degrees
            delta.x = 0.0f;

        auto forward_axis = forward();

        glm::quat yaw = glm::angleAxis(-delta.x, up());
        glm::quat pitch = glm::angleAxis(-delta.y, right());

        auto target = eye_position_ + pitch * yaw * forward_axis;

        set_camera_parameters(eye_position_, target);

    }

    auto Camera::zoom(float offset) -> void
    {
        // > 0 = zoom in (decrease FOV by <offset> angles)
        fov_ = std::clamp(fov_ - offset, MIN_FOV, MAX_FOV);
        //LOG_INFO("fov: {0}", fov_);
        matrix_dirty = true;
    }

    auto Camera::set_camera_parameters(
        const glm::vec3& eye_position,
        const glm::vec3& target,
        const glm::vec3& up
    ) -> void
    {
        eye_position_ = eye_position;
        up_ = up;
        look_dir_ = target - eye_position;

        matrix_dirty = true;
    }

    auto Camera::calculate_view_martix() -> void
    {
        view_matrix_ = glm::lookAtRH(eye_position_, eye_position_ + look_dir_, up_);
        inv_view_matrix_ = glm::inverse(view_matrix_);
    }

    auto Camera::calculate_proj_matrix() -> void
    {
        auto perspective_matrix = glm::perspective(glm::radians(fov_), aspect_, near_plane_, far_plane_);
        proj_matrix_ = perspective_matrix;
    }
}
