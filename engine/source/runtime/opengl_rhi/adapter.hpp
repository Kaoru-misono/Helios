#pragma once
#include <glad/glad.h>
#include "rhi/texture.hpp"
namespace Helios
{
    auto inline to_gl_enum(Texture_Sampler::Filter filter) -> GLenum
    {
        switch (filter) {
            case Texture_Sampler::Filter::linear: return GL_LINEAR;
            case Texture_Sampler::Filter::nearest: return GL_NEAREST;
            case Texture_Sampler::Filter::liner_mipmap_liner: return GL_LINEAR_MIPMAP_LINEAR;
            case Texture_Sampler::Filter::liner_mipmap_nearest: return GL_LINEAR_MIPMAP_NEAREST;
            case Texture_Sampler::Filter::nearest_mipmap_liner: return GL_NEAREST_MIPMAP_LINEAR;
            case Texture_Sampler::Filter::nearest_mipmap_nearest: return GL_NEAREST_MIPMAP_NEAREST;
            default: break;
        }
        return GL_FALSE;
    }

    auto inline to_gl_enum(Texture_Sampler::Warp warp) -> GLenum
    {
        switch (warp) {
            case Texture_Sampler::Warp::none: break;
            case Texture_Sampler::Warp::repeat: return GL_REPEAT;
            case Texture_Sampler::Warp::clamp_to_edge: return GL_CLAMP_TO_EDGE;
            case Texture_Sampler::Warp::clamp_to_border: return GL_CLAMP_TO_BORDER;
            case Texture_Sampler::Warp::mirrored_repeat: return GL_MIRRORED_REPEAT;
            case Texture_Sampler::Warp::mirror_calmp_to_edge: return GL_MIRROR_CLAMP_TO_EDGE;
            default: break;
        }
        return GL_FALSE;
    }
}