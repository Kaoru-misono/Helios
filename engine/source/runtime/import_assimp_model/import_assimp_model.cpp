#include "import_assimp_model.hpp"
#include <deque>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/ProgressHandler.hpp>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>
#include <assimp/scene.h>
#include <assimp/config.h>
#include <span>
#include <logger/logger_marco.hpp>

namespace Helios
{
    namespace
    {
        auto tex_type_to_assimp = std::unordered_map<Assimp_Model::Texture_Type, aiTextureType>{
            { Assimp_Model::Texture_Type::unknown,      aiTextureType::aiTextureType_UNKNOWN },
            { Assimp_Model::Texture_Type::diffuse,      aiTextureType::aiTextureType_DIFFUSE },
            { Assimp_Model::Texture_Type::specular,     aiTextureType::aiTextureType_SPECULAR },
            { Assimp_Model::Texture_Type::ambient,      aiTextureType::aiTextureType_AMBIENT },
            { Assimp_Model::Texture_Type::opacity,      aiTextureType::aiTextureType_OPACITY },
            { Assimp_Model::Texture_Type::height,       aiTextureType::aiTextureType_HEIGHT },
            { Assimp_Model::Texture_Type::emissive,     aiTextureType::aiTextureType_EMISSIVE },
            { Assimp_Model::Texture_Type::normal,       aiTextureType::aiTextureType_NORMALS },
            { Assimp_Model::Texture_Type::shininess,    aiTextureType::aiTextureType_SHININESS },
            { Assimp_Model::Texture_Type::displacement, aiTextureType::aiTextureType_DISPLACEMENT },
            { Assimp_Model::Texture_Type::reflection,   aiTextureType::aiTextureType_REFLECTION },
            { Assimp_Model::Texture_Type::lightmap,     aiTextureType::aiTextureType_LIGHTMAP },
            { Assimp_Model::Texture_Type::base_color,   aiTextureType::aiTextureType_BASE_COLOR },
            { Assimp_Model::Texture_Type::normal_camera,     aiTextureType::aiTextureType_NORMAL_CAMERA },
            { Assimp_Model::Texture_Type::emission_color,    aiTextureType::aiTextureType_EMISSION_COLOR },
            { Assimp_Model::Texture_Type::metalness,         aiTextureType::aiTextureType_METALNESS },
            { Assimp_Model::Texture_Type::diffuse_roughness, aiTextureType::aiTextureType_DIFFUSE_ROUGHNESS },
            { Assimp_Model::Texture_Type::ambient_occlusion, aiTextureType::aiTextureType_AMBIENT_OCCLUSION }
        };

        auto postprocessing_step_to_assimp = std::unordered_map<Assimp_Config::Postprocessing_Step, aiPostProcessSteps>{
            { Assimp_Config::Postprocessing_Step::calc_tangent_space,           ::aiProcess_CalcTangentSpace },
            { Assimp_Config::Postprocessing_Step::join_identical_vertices,      ::aiProcess_JoinIdenticalVertices },
            { Assimp_Config::Postprocessing_Step::make_left_handed,             ::aiProcess_MakeLeftHanded },
            { Assimp_Config::Postprocessing_Step::triangulate,                  ::aiProcess_Triangulate },
            { Assimp_Config::Postprocessing_Step::remove_component,             ::aiProcess_RemoveComponent },
            { Assimp_Config::Postprocessing_Step::gen_normals,                  ::aiProcess_GenNormals },
            { Assimp_Config::Postprocessing_Step::gen_smooth_normals,           ::aiProcess_GenSmoothNormals },
            { Assimp_Config::Postprocessing_Step::split_large_meshes,           ::aiProcess_SplitLargeMeshes },
            { Assimp_Config::Postprocessing_Step::pre_transform_vertices,       ::aiProcess_PreTransformVertices },
            { Assimp_Config::Postprocessing_Step::limit_bone_weights,           ::aiProcess_LimitBoneWeights },
            { Assimp_Config::Postprocessing_Step::validate_data_structure,      ::aiProcess_ValidateDataStructure },
            { Assimp_Config::Postprocessing_Step::improve_cache_locality,       ::aiProcess_ImproveCacheLocality },
            { Assimp_Config::Postprocessing_Step::remove_redundant_materials,   ::aiProcess_RemoveRedundantMaterials },
            { Assimp_Config::Postprocessing_Step::fix_infacing_normals,         ::aiProcess_FixInfacingNormals },
            { Assimp_Config::Postprocessing_Step::sort_by_primitive_type,       ::aiProcess_SortByPType },
            { Assimp_Config::Postprocessing_Step::find_degenerates,             ::aiProcess_FindDegenerates },
            { Assimp_Config::Postprocessing_Step::find_invalid_data,            ::aiProcess_FindInvalidData },
            { Assimp_Config::Postprocessing_Step::gen_UV,                       ::aiProcess_GenUVCoords },
            { Assimp_Config::Postprocessing_Step::transform_UV,                 ::aiProcess_TransformUVCoords },
            { Assimp_Config::Postprocessing_Step::find_instances,               ::aiProcess_FindInstances },
            { Assimp_Config::Postprocessing_Step::optimize_meshes,              ::aiProcess_OptimizeMeshes },
            { Assimp_Config::Postprocessing_Step::optimize_graph,               ::aiProcess_OptimizeGraph },
            { Assimp_Config::Postprocessing_Step::flip_UV,                      ::aiProcess_FlipUVs },
            { Assimp_Config::Postprocessing_Step::flip_winding_order,           ::aiProcess_FlipWindingOrder },
            { Assimp_Config::Postprocessing_Step::split_by_bone_count,          ::aiProcess_SplitByBoneCount },
            { Assimp_Config::Postprocessing_Step::debone,                       ::aiProcess_Debone },
            { Assimp_Config::Postprocessing_Step::global_scale,                 ::aiProcess_GlobalScale }
        };

        auto from_assimp(::aiMatrix4x4 m) -> glm::mat4
        {
            return glm::mat4{
                glm::vec4{m.a1, m.b1, m.c1, m.d1},
                glm::vec4{m.a2, m.b2, m.c2, m.d2},
                glm::vec4{m.a3, m.b3, m.c3, m.d3},
                glm::vec4{m.a4, m.b4, m.c4, m.d4},
            };
        }

        auto from_assimp(::aiVector3D v) noexcept -> glm::vec3
        {
            return { v.x, v.y, v.z };
        }

        auto from_assimp(::aiQuaternion q) noexcept -> glm::quat
        {
            return { q.w, q.x, q.y, q.z };
        }

    }

    auto Assimp_Model::load_model(std::string const& path, Assimp_Config config) -> Assimp_Model
    {
        using Array_Index = Assimp_Model::Array_Index;
        using Node_Address_To_Index = std::unordered_map<std::string, Array_Index>;

        Assimp::Importer importer;
        //TODO: set postprocessing step
        auto scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG_ERROR("ASSIMP:: {0}", importer.GetErrorString());
        }

        auto copy_into_shape_if_exists = [] (auto mesh, auto& shape) {
            auto copy_if_exists = [&] (auto src, auto& dst) {
                static_assert(sizeof(src[0]) == sizeof(dst[0]), "Unsupported platform.");

                if (src == nullptr || mesh->mNumVertices == 0)
                    return;

                dst.resize(std::size_t(mesh->mNumVertices));
                std::copy_n(
                    reinterpret_cast<char const*>(src),
                    std::size_t(mesh->mNumVertices) * sizeof(src[0]),
                    reinterpret_cast<char*>(dst.data())
                );
            };

            auto copy_if_exists_to_2d = [&] (auto src, auto& dst) {
                if (src == nullptr || mesh->mNumVertices == 0)
                    return;

                dst.reserve(std::size_t(mesh->mNumVertices));
                for (auto last=src+mesh->mNumVertices; src!=last; src++)
                    dst.emplace_back(src->x, src->y);
            };

            copy_if_exists(mesh->mVertices, shape.position);
            copy_if_exists(mesh->mNormals, shape.normal);
            copy_if_exists(mesh->mTangents, shape.tangent);
            copy_if_exists(mesh->mBitangents, shape.bitangent);
            copy_if_exists_to_2d(mesh->mTextureCoords[0], shape.texcoord);
            copy_if_exists_to_2d(mesh->mTextureCoords[1], shape.texcoord2);
            copy_if_exists(mesh->mColors[0], shape.color);
            copy_if_exists(mesh->mColors[1], shape.color2);
        };

        auto load_textures = [&] (Assimp_Model& model) {
            auto textures = std::span<aiTexture*>{scene->mTextures, scene->mNumTextures};
            for (auto tex: textures) {
                if (tex->mHeight == 0) {
                    auto data = (char*) tex->pcData;
                    model.textures.emplace(tex->mFilename.C_Str(), std::string{data});
                } else {
                    LOG_ERROR("Embeded raw image data is unsupported.");
                }
            }
        };

        auto load_materials = [&] (Assimp_Model& model) {
            auto next_mat_name_id = 0;
            auto textures = std::span<::aiTexture*>{scene->mTextures, scene->mNumTextures};

            for (int i = 0; i < int(scene->mNumMaterials); i++) {
                auto mat = scene->mMaterials[i];

                Assimp_Model::Material result;

                result.name = mat->GetName().C_Str();
                if (result.name.empty()) result.name = "material" + std::to_string(next_mat_name_id++);

                static_assert(sizeof(glm::vec4) == sizeof(::aiColor4D), "Unsupported platform.");
                mat->Get(AI_MATKEY_COLOR_DIFFUSE, reinterpret_cast<::aiColor4D&>(result.diffuse_color));
                mat->Get(AI_MATKEY_COLOR_SPECULAR, reinterpret_cast<::aiColor4D&>(result.specular_color));
                mat->Get(AI_MATKEY_COLOR_AMBIENT, reinterpret_cast<::aiColor4D&>(result.ambient_color));
                mat->Get(AI_MATKEY_COLOR_TRANSPARENT, reinterpret_cast<::aiColor4D&>(result.transparent_color));
                mat->Get(AI_MATKEY_COLOR_EMISSIVE, reinterpret_cast<::aiColor4D&>(result.emissive_color));

                mat->Get(AI_MATKEY_SHININESS, result.shininess);
                mat->Get(AI_MATKEY_OPACITY, result.opacity);
                mat->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR, result.glTF_PBR_metallic_roughness_roughness_factor);
                mat->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR, result.glTF_PBR_metallic_roughness_metallic_factor);

                for (auto tex_type_assimp_tex_type: tex_type_to_assimp) {
                    auto tex_type = tex_type_assimp_tex_type.first;
                    auto assimp_tex_type = tex_type_assimp_tex_type.second;

                    aiString path;
                    if (mat->GetTexture(assimp_tex_type, 0, &path) != ::aiReturn_SUCCESS)
                        continue;
                    if (path.C_Str()[0] == '\0')    // skip empty path
                        continue;

                    auto embeded_tex = (aiTexture*) nullptr;

                    if (path.C_Str()[0] == '*') {
                        auto tex_idx = std::atoi(&path.C_Str()[1]);
                        embeded_tex = textures[tex_idx];
                    } else {
                        for (auto tex: textures) {
                            if (tex->mFilename == path) {
                                embeded_tex = tex;
                                break;
                            }
                        }
                    }

                    if (embeded_tex) path = embeded_tex->mFilename;

                    result.texture_paths.emplace(tex_type, path.C_Str());
                }

                model.materials.emplace_back(std::move(result));
            }
        };

        auto load_meshes = [&] (Assimp_Model& model) {
            auto next_mesh_name_id = 0;

            for (auto i = 0u; i < scene->mNumMeshes; i++) {
                auto mesh = scene->mMeshes[i];

                Assimp_Model::Mesh result;

                result.name = "mesh" + std::to_string(next_mesh_name_id++);
                auto name = std::string{mesh->mName.C_Str()};
                if (!name.empty()) {
                    result.name += ": ";
                    result.name += name;
                }

                result.material = Array_Index(mesh->mMaterialIndex);

                auto copy_if_exists_to_triangle = [&] (auto src, auto& dst) {
                    static_assert(sizeof(unsigned int) == sizeof(std::uint32_t), "Unsupported platform.");

                    if (src == nullptr || mesh->mNumFaces == 0)
                        return;

                    dst.reserve(std::size_t(mesh->mNumFaces));

                    for (auto last = src + mesh->mNumFaces; src != last; src++) {
                        dst.push_back({
                            src->mIndices[0],
                            src->mIndices[1],
                            src->mIndices[2],
                        });
                    }
                };

                auto copy_if_exists_to_shape_keys = [&] (auto src, auto& dst) {
                    if (src == nullptr || mesh->mNumAnimMeshes == 0)
                        return;

                    auto next_shape_key_name_id = 0;
                    dst.resize(std::size_t(mesh->mNumAnimMeshes));

                    for (auto i = 0u; i < mesh->mNumAnimMeshes; i++) {
                        auto assimp_key = src[i];
                        auto& key = dst[i];

                        key.name = "shape" + std::to_string(next_shape_key_name_id++);
                        auto name = std::string{assimp_key->mName.C_Str()};
                        if (!name.empty()) {
                            key.name += ": ";
                            key.name += name;
                        }

                        key.default_weight = assimp_key->mWeight;
                        copy_into_shape_if_exists(assimp_key, key.shape);
                    }
                };

                copy_if_exists_to_triangle(mesh->mFaces, result.topology);
                copy_into_shape_if_exists(mesh, result.main_shape);
                copy_if_exists_to_shape_keys(mesh->mAnimMeshes, result.shape_keys);

                model.meshes.emplace_back(std::move(result));
            }
        };

        auto load_hierarchy = [&] (Assimp_Model& model) -> Node_Address_To_Index {
            auto next_node_name_id = 0;
            std::unordered_map<::aiNode*, Array_Index> node_to_index{{nullptr, Array_Index(-1)}};
            std::deque<::aiNode*> pending{scene->mRootNode};
            Node_Address_To_Index node_addr_to_idx;

            while (!pending.empty()) {
                auto node = pending.front();
                pending.pop_front();

                auto parent = node_to_index.at(node->mParent);
                auto node_index = Array_Index(model.nodes.size());
                node_to_index.emplace(node, node_index);

                // This assumes [SS/ASSIMP/V1] patchset for Assimp has been applied.
                node_addr_to_idx.emplace("node:" + std::to_string(std::uintptr_t(node)), node_index);

                for (int i=0; i<int(node->mNumMeshes); i++)
                    model.meshes[node->mMeshes[i]].parent = node_index;

                for (int i=0; i<int(node->mNumChildren); i++)
                    pending.emplace_back(node->mChildren[i]);

                auto name = std::string{node->mName.C_Str()};
                if (name.empty()) name = "node" + std::to_string(next_node_name_id++);

                auto transformation = from_assimp(node->mTransformation);
                model.nodes.push_back({
                    parent,
                    transformation,
                    std::move(name),
                });
            }

            return node_addr_to_idx;
        };

        Assimp_Model result;
        {
            load_textures(result);
            load_materials(result);
            load_meshes(result);

            //for bones and animation
            auto node_addr_to_idx = load_hierarchy(result);
        }

        return result;
    }

}
