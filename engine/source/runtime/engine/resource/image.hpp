#pragma once
#include "../../pre-compile.h"

namespace Helios
{
    enum struct Image_Format: uint32_t
    {
        u8x1,
        u8x2,
        u8x3,
        u8x4,
    };

    struct Image final
    {
        std::string name;
        Image(std::string source) noexcept;
        Image(std::string source, int w, int h, Image_Format format) noexcept;
        ~Image() noexcept;

        auto format() const noexcept -> Image_Format;
        auto channel() const noexcept -> int;
        auto width() const noexcept -> int;
        auto height() const noexcept -> int;
        auto data() const noexcept -> unsigned char*;

        static auto load(std::string const& path, bool flip = false) -> std::shared_ptr<Image>;
    private:
        int width_{0};
        int height_{0};
        Image_Format format_{Image_Format::u8x3};
        unsigned char* data_{nullptr};
    };
}
