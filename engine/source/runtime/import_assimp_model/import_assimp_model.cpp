#include "import_assimp_model.hpp"
#include <assimp/ProgressHandler.hpp>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>
#include <assimp/scene.h>
#include <assimp/config.h>

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

                // auto postprocessing_step_to_assimp = util::Enum_Map{
                //     Assimp_Config::Postprocessing_Step::calc_tangent_space,           ::aiProcess_CalcTangentSpace,
                //     Assimp_Config::Postprocessing_Step::join_identical_vertices,      ::aiProcess_JoinIdenticalVertices,
                //     // Removed deliberately. The Engine do not support left-handed coordinate system.
                //     // > Assimp_Config::Postprocessing_Step::make_left_handed,             ::aiProcess_MakeLeftHanded,
                //     // Removed deliberately. Always triangulate.
                //     // > Assimp_Config::Postprocessing_Step::triangulate,                  ::aiProcess_Triangulate,
                //     Assimp_Config::Postprocessing_Step::remove_component,             ::aiProcess_RemoveComponent,
                //     Assimp_Config::Postprocessing_Step::gen_normals,                  ::aiProcess_GenNormals,
                //     Assimp_Config::Postprocessing_Step::gen_smooth_normals,           ::aiProcess_GenSmoothNormals,
                //     Assimp_Config::Postprocessing_Step::split_large_meshes,           ::aiProcess_SplitLargeMeshes,
                //     Assimp_Config::Postprocessing_Step::pre_transform_vertices,       ::aiProcess_PreTransformVertices,
                //     Assimp_Config::Postprocessing_Step::limit_bone_weights,           ::aiProcess_LimitBoneWeights,
                //     Assimp_Config::Postprocessing_Step::validate_data_structure,      ::aiProcess_ValidateDataStructure,
                //     Assimp_Config::Postprocessing_Step::improve_cache_locality,       ::aiProcess_ImproveCacheLocality,
                //     Assimp_Config::Postprocessing_Step::remove_redundant_materials,   ::aiProcess_RemoveRedundantMaterials,
                //     Assimp_Config::Postprocessing_Step::fix_infacing_normals,         ::aiProcess_FixInfacingNormals,
                //     Assimp_Config::Postprocessing_Step::sort_by_primitive_type,       ::aiProcess_SortByPType,
                //     Assimp_Config::Postprocessing_Step::find_degenerates,             ::aiProcess_FindDegenerates,
                //     Assimp_Config::Postprocessing_Step::find_invalid_data,            ::aiProcess_FindInvalidData,
                //     Assimp_Config::Postprocessing_Step::gen_UV,                       ::aiProcess_GenUVCoords,
                //     Assimp_Config::Postprocessing_Step::transform_UV,                 ::aiProcess_TransformUVCoords,
                //     Assimp_Config::Postprocessing_Step::find_instances,               ::aiProcess_FindInstances,
                //     Assimp_Config::Postprocessing_Step::optimize_meshes,              ::aiProcess_OptimizeMeshes,
                //     Assimp_Config::Postprocessing_Step::optimize_graph,               ::aiProcess_OptimizeGraph,
                //     Assimp_Config::Postprocessing_Step::flip_UV,                      ::aiProcess_FlipUVs,
                //     Assimp_Config::Postprocessing_Step::flip_winding_order,           ::aiProcess_FlipWindingOrder,
                //     Assimp_Config::Postprocessing_Step::split_by_bone_count,          ::aiProcess_SplitByBoneCount,
                //     Assimp_Config::Postprocessing_Step::debone,                       ::aiProcess_Debone,
                //     Assimp_Config::Postprocessing_Step::global_scale,                 ::aiProcess_GlobalScale,
                // };
    }
}