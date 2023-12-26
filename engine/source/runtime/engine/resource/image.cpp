#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
namespace Helios
{
    Image::Image(std::string source) noexcept: name(source)
    {
        int channels = 0;
        data_ = stbi_load(name.c_str(), &width_, &height_, &channels, 0);

        if (data_) {
            if (channels == 1)
                format_ = Image_Format::u8x1;
            else if (channels == 3)
                format_ = Image_Format::u8x3;
            else if (channels == 4)
                format_ = Image_Format::u8x4;
        } else {
            std::cout << "Texture failed to load at path: " << name << std::endl;
        }
    }

    Image::Image(std::string source, int w, int h, Image_Format format) noexcept: name(source), width_(w), height_(h), format_(format)
    {
    }

    Image::~Image() noexcept
    {
        stbi_image_free(data_);
    }

    auto Image::format() const noexcept -> Image_Format
    {
        return format_;
    }

    auto Image::channel() const noexcept -> int
    {
        return format_ == Image_Format::u8x1 ? 1 : format_ == Image_Format::u8x3 ? 3 : 4;
    }

    auto Image::width() const noexcept -> int
    {
        return width_;
    }

    auto Image::height() const noexcept -> int
    {
        return height_;
    }

    auto Image::data() const noexcept -> unsigned char*
    {
        return data_;
    }

    auto Image::load(std::string path, bool flip) -> std::shared_ptr<Image>
    {
        stbi_set_flip_vertically_on_load(flip ? 1 : 0);
        return make_shared<Image>(path);
    }
}
