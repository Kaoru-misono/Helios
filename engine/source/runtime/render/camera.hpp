#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Helios::Scene
{
    #define MAX_FOV 89.0f
    #define MIN_FOV 10.0f

    class Camera
    {
    public:
        Camera();
        ~Camera() = default;

        glm::mat4 view_matrix{ 1.0 };

        auto move(glm::vec3 delta) -> void;
        auto rotate(glm::vec2 delta) -> void;
        auto zoom(float offset) -> void;
        auto look_at(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up) -> void;

        glm::vec3 position() const { return position_; }
        glm::quat rotation() const { return rotation_; }

        auto set_camera_properties(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f)) -> void;
        auto set_fov(float fov) -> void { fov_ = fov; }
        auto set_aspect(float aspect) -> void { aspect_ = aspect; }
        auto set_near_far_plane(float near, float far) -> void { near_plane = near; far_plane = far; }

        auto get_view_matrix() -> glm::mat4;
        auto get_projection_matrix()const  -> glm::mat4;
    protected:
        float fov_{45.0f};
        float aspect_{ 4.0f/3.0f };

        glm::vec3 position_{0.0f, 0.0f, 0.0f};
        glm::quat rotation_;
        glm::quat inv_rotation_;
        float near_plane{ 0.1f };
        float far_plane{ 1000.0f };
        glm::vec3 up_vector{ 0.0f, 1.0f, 0.0f };
        const glm::vec3 negative_z{ 0.0f, 0.0f, -1.0f };
        const glm::vec3 positive_y{ 0.0f, 1.0f,  0.0f };
    };
}
