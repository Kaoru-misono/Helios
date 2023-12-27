#pragma once
namespace Helios
{
    struct Texture_Sampler
    {
        enum struct Filter
        {
            nearest,
            linear,
            nearest_mipmap_liner,
            nearest_mipmap_nearest,
            liner_mipmap_nearest,
            liner_mipmap_liner,
        };

        enum struct Warp
        {
            none,
            repeat,
            clamp_to_edge,
            clamp_to_border,
            mirrored_repeat,
            mirror_calmp_to_edge,
        };

        Filter min_filter{Filter::linear};
        Filter mag_filter{Filter::linear};

        Warp warp_s{Warp::repeat};
        Warp warp_t{Warp::repeat};
        Warp warp_r{Warp::none};

        int sampler_unit{0};
    };
    struct Texture
    {
        enum struct Kind
        {
            TEX_2D,
            TEX_3D,
            TEX_CUBE,
        };

        enum struct Format
        {
            r8,
            rgb8,
            rgba8,
            depth24,
        };
        Kind kind{Kind::TEX_2D};
        Format format{Format::rgba8};
        virtual ~Texture() {}
        virtual auto bind() const -> void = 0;
        virtual auto unbind() const -> void = 0;
        virtual auto set_sampler(Texture_Sampler const& sampler, int texture_unit) const -> void = 0;
    };
}
