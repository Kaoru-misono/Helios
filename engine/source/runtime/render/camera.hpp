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

        glm::vec3 position_{0.0f, 0.0f, 0.0f};
        glm::quat rotation_;
        glm::quat inv_rotation_;
        float near_plane{ 1000.0f };
        float far_plane{ 0.1f };
        glm::vec3 up_vector{ 0.0f, 1.0f, 0.0f };
        const glm::vec3 negative_z{ 0.0f, 0.0f, -1.0f };
        const glm::vec3 positive_y{ 0.0f, 1.0f,  0.0f };

        glm::mat4 view_matrix{ 1.0 };

        auto set_camera_properties(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f)) -> void;
        auto get_view_matrix() -> glm::mat4;
        auto get_projection_matrix()const  -> glm::mat4;
    protected:
        float fov{45.0f};
        float aspect{ 4.0f/3.0f };
    };
}