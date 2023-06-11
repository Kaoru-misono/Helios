#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Helios::Scene
{
    #define MAX_FOV 89.0f
    #define MIN_FOV 10.0f

    class Camera
    {
    public:
        Camera();
        ~Camera() = default;

        auto update() -> void;

        //camera controll
        auto move(glm::vec3 delta) -> void;
        auto rotate(glm::vec2 delta) -> void;
        auto zoom(float offset) -> void;

        auto get_position()const -> glm::vec3 { return eye_position_; }
        //auto rotation()const -> glm::mat3 { return glm::toMat3(rotation_); }

        auto forward() const -> glm::vec3 { return glm::normalize(-*reinterpret_cast<const glm::vec3 *>(&inv_view_matrix_[2])); }
        auto right()   const -> glm::vec3 { return glm::normalize( *reinterpret_cast<const glm::vec3 *>(&inv_view_matrix_[0])); }
        auto up()      const -> glm::vec3 { return glm::normalize( *reinterpret_cast<const glm::vec3 *>(&inv_view_matrix_[1])); }

        auto set_camera_parameters(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f)) -> void;
        auto set_fov(float fov) -> void { fov_ = fov; }
        auto set_aspect(float aspect) -> void { aspect_ = aspect; }
        auto set_near_far_plane(float near, float far) -> void { near_plane_ = near; far_plane_ = far; }

        auto get_view_matrix()const -> glm::mat4 { return view_matrix_; }
        auto get_inv_view_matrix()const -> glm::mat4 { return inv_view_matrix_; }
        auto get_projection_matrix()const  -> glm::mat4 { return proj_matrix_; }
    private:
        auto calculate_view_martix() -> void;
        auto calculate_proj_matrix() -> void;
    private:

        float fov_{45.0f};
        float aspect_{ 4.0f/3.0f };
        float near_plane_{ 0.1f };
        float far_plane_{ 1000.0f };

        glm::vec3 eye_position_{ 0.0f };
        glm::vec3 look_dir_{ 0.0f };
        glm::mat4 view_matrix_{ 1.0 };
        glm::mat4 inv_view_matrix_{ 1.0f };
        glm::mat4 proj_matrix_{ 1.0f };

        glm::vec3 up_{ 0.0f, 1.0f, 0.0f };

        bool need_update_matrix_{ false };

        //OpenGL normalized right hand camera coordinate system, OpenGL Camera look at negative z
        static const glm::vec3 negative_z;
        static const glm::vec3 positive_y;
        static const glm::vec3 positive_x;
    };

    inline const glm::vec3 Camera::negative_z{ 0.0f, 0.0f, -1.0f };
    inline const glm::vec3 Camera::positive_y{ 0.0f, 1.0f,  0.0f };
    inline const glm::vec3 Camera::positive_x{ 1.0f, 0.0f,  0.0f };
}
