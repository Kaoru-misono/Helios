#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>

namespace Helios
{

    struct Assimp_Config final
    {
        enum struct Postprocessing_Step: std::uint32_t
        {
            calc_tangent_space,
            join_identical_vertices,
            make_left_handed,    
            triangulate,         
            remove_component,
            gen_normals,
            gen_smooth_normals,
            split_large_meshes,
            pre_transform_vertices,
            limit_bone_weights,
            validate_data_structure,
            improve_cache_locality,
            remove_redundant_materials,
            fix_infacing_normals,
            sort_by_primitive_type,
            find_degenerates,
            find_invalid_data,
            gen_UV,
            transform_UV,
            find_instances,
            optimize_meshes,
            optimize_graph,
            flip_UV,
            flip_winding_order,
            split_by_bone_count,
            debone,
            global_scale,
            last_flag_,
        };

        using Postprocessing_Steps = std::unordered_set<Postprocessing_Step>;

        Postprocessing_Steps postprocessing_steps{
        Postprocessing_Step::calc_tangent_space,
        Postprocessing_Step::join_identical_vertices,
        Postprocessing_Step::gen_smooth_normals,
        Postprocessing_Step::limit_bone_weights,
        Postprocessing_Step::validate_data_structure,
        Postprocessing_Step::improve_cache_locality,
        Postprocessing_Step::remove_redundant_materials,
        Postprocessing_Step::find_invalid_data,
        Postprocessing_Step::transform_UV,
        Postprocessing_Step::optimize_meshes,
        };
        float max_smoothing_angle_degrees{80.0f};
        std::string hash_override{};
        //bool use_cache{true};
    };

    class Assimp_Model
    {
    public:
        using Array_Index = std::int32_t;

        enum struct Texture_Type
        {
            unknown,
            diffuse,
            specular,
            ambient,
            opacity,
            height,
            emissive,
            normal,
            shininess,
            displacement,
            reflection,
            lightmap,
            base_color,
            normal_camera,
            emission_color,
            metalness,
            diffuse_roughness,
            ambient_occlusion,
        };

        struct Node
        {
            Array_Index parent{-1};     // index for nodes array
            glm::mat4 transformation{};

            std::string name;
        };

        struct Mesh final
        {
            struct Triangle final
            {
                std::uint32_t a{};
                std::uint32_t b{};
                std::uint32_t c{};
            };

            struct Shape final
            {
                std::vector<glm::vec3> position;
                std::vector<glm::vec3> normal;
                std::vector<glm::vec3> tangent;
                std::vector<glm::vec3> bitangent;
                std::vector<glm::vec2> texcoord;
                std::vector<glm::vec2> texcoord2;
                std::vector<glm::vec4> color;
                std::vector<glm::vec4> color2;
            };

            struct Shape_Key final
            {
                float default_weight{};
                Shape shape;
                std::string name;
            };

            struct Vertex_Weight final
            {
                Array_Index vertex{-1}; // index for Shape::position array
                float weight{};
            };

            struct Bone final
            {
                Array_Index driver{-1}; // index for nodes array
                glm::mat4 transformation{};
                std::vector<Vertex_Weight> weights;
            };

            Array_Index parent{-1};     // index for nodes array
            Array_Index material{-1};   // index for materials array

            std::vector<Triangle> topology;
            Shape main_shape;
            std::vector<Shape_Key> shape_keys;
            std::vector<Bone> bones;
            std::string name;
        };

        struct Material final
        {
           glm::vec4 diffuse_color{};
           glm::vec4 specular_color{};
           glm::vec4 ambient_color{};
           glm::vec4 transparent_color{};
           glm::vec4 emissive_color{};

            float shininess{};
            float opacity{};
            float glTF_PBR_metallic_roughness_roughness_factor{};
            float glTF_PBR_metallic_roughness_metallic_factor{};

            std::unordered_map<Texture_Type, std::string> texture_paths;

            std::string name;
        };

        auto load_model(std::string const& path, Assimp_Config config) -> Assimp_Model;
    
    private:
        std::vector<Node> nodes;        // guaranteed to be in level-order
        std::vector<Mesh> meshes;
        std::vector<Material> materials;
        std::unordered_map<std::string, std::string> textures; 
    };
}