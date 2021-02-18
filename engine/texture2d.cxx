#include <string_view>
#include <vector>

#include "gl_assist.hxx"
#include "shader.hxx"
#include "texture2d.hxx"

namespace engine
{

void texture2d::bind()
{
    //    GLboolean is_texture = glIsTexture(id);
    //    assert(is_texture);
    //    GL_CHECK()
    glBindTexture(GL_TEXTURE_2D, id);
    GL_CHECK();
}

std::uint32_t texture2d::get_width()
{
    return width;
}

std::uint32_t texture2d::get_height()
{
    return height;
}

int texture2d::get_id()
{
    return id;
}

texture2d::~texture2d()
{
    GLuint id_ = id;
    glDeleteTextures(1, &id_);
    GL_CHECK()
}

texture2d::texture2d(std::string_view path_, size_t id_, shader_es_32* shd)
{
    id          = id_;
    path        = path_;
    x_pos       = -1;
    y_pos       = -1;
    width       = -1;
    height      = -1;
    coordinates = { -1, -1, -1, -1, -1, -1, -1, -1 };

    txt_shader = shd;

    txt_shader->use();
    int shd_proc = txt_shader->id;

    int location = glGetUniformLocation(shd_proc, "ourTxt");
    GL_CHECK()

    assert(-1 != location);
    int texture_unit = id;
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    GL_CHECK()

    if (!load_texture(path))
    {
        std::cerr << "failed load texture\n";
    }
    else
    {
        std::cerr << "texture is loaded" << std::endl;
    }

    glUniform1i(location, 0 + texture_unit);
    GL_CHECK()

    glEnable(GL_BLEND);
    GL_CHECK()

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GL_CHECK()
}

bool texture2d::load_texture(std::string_view path)
{
    std::vector<std::byte> png_file_in_memory;
    std::ifstream          ifs(path.data(), std::ios_base::binary);
    if (!ifs)
    {
        return false;
    }
    ifs.seekg(0, std::ios_base::end);
    size_t pos_in_file = static_cast<size_t>(ifs.tellg());
    png_file_in_memory.resize(pos_in_file);
    ifs.seekg(0, std::ios_base::beg);
    if (!ifs)
    {
        return false;
    }

    ifs.read(reinterpret_cast<char*>(png_file_in_memory.data()),
             static_cast<std::streamsize>(png_file_in_memory.size()));
    if (!ifs.good())
    {
        return false;
    }

    std::vector<std::byte> image;
    unsigned long          w = 0;
    unsigned long          h = 0;

    int width_, height_, nr_channals;

    unsigned char* ptr_data =
        stbi_load(path.data(), &width_, &height_, &nr_channals, 0);
    if (ptr_data == nullptr)
    {
        std::cerr << "error: Can't load image :[" << path << "]" << std::endl;
        return false;
    }
    else
    {
        std::clog << "Image :[" << path << "] load successful" << std::endl;
    }

    GLuint tex_handl = id;
    glGenTextures(1, &tex_handl);
    GL_CHECK()
    glBindTexture(GL_TEXTURE_2D, tex_handl);
    GL_CHECK()

    GLint mipmap_level = 0;
    GLint border       = 0;
    // clang-format off
        glTexImage2D(GL_TEXTURE_2D, // Specifies the target texture of the active texture unit
                     mipmap_level,  // Specifies the level-of-detail number. Level 0 is the base image level
                     GL_RGBA,       // Specifies the internal format of the texture
                     static_cast<GLsizei>(w),
                     static_cast<GLsizei>(h),
                     border,        // Specifies the width of the border. Must be 0. For GLES 2.0
                     GL_RGBA,       // Specifies the format of the texel data. Must match internalformat
                     GL_UNSIGNED_BYTE, // Specifies the data type of the texel data
                     ptr_data/*&image[0]*/);    // Specifies a pointer to the image data in memory
    // clang-format on
    GL_CHECK()

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GL_CHECK()
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    GL_CHECK()
    return true;
}

texture_2d_es_32::texture_2d_es_32(std::string_view path)
{
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D,
                  texture_id); // все последующие GL_TEXTURE_2D-операции теперь
    // будут влиять на данный текстурный объект
    // установка параметров наложения текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_BORDER); // установка метода наложения текстуры
    // GL_REPEAT (стандартный метод наложения)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // установка параметров фильтрации текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // загрузка изображения, создание текстуры и генерирование
    // mipmap-уровней
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data =
        stbi_load(path.data(), &width, &height, &nr_channels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, data);
        GL_CHECK()
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::clog << "ERROR::Failed to load texture: " << stbi_failure_reason();
        std::clog << std::endl;
    }

    std::clog << "texture_resolution: " << width << "x" << height << std::endl;
    std::clog << "nr_channels: " << nr_channels << std::endl << std::endl;

    stbi_image_free(data);
}

void texture_2d_es_32::bind()
{
    glBindTexture(GL_TEXTURE_2D, texture_id);
    GL_CHECK()
}

int texture_2d_es_32::get_id()
{
    return texture_id;
}

void animation_2d_es_32::make_atlas_map()
{
    for (int row_counter(row - 1); row_counter >= 0; --row_counter)
    {
        for (size_t col_counter{ 0 }; col_counter < col; ++col_counter)
        {
            std::clog << col_counter << "_" << row_counter << " ";
            // clang-format off
            atlas.push_back(v_8( col_counter * offset_x, (row_counter + 1) * offset_y,
                                 col_counter * offset_x, row_counter * offset_y,
                                 (col_counter + 1) * offset_x, (row_counter + 1) * offset_y,
                                 (col_counter + 1) * offset_x, row_counter * offset_y));
            // clang-format on
        }
        std::clog << std::endl;
    }

    for (size_t i{ 0 }; i < col * row; i++)
    {
        std::clog << atlas[i];
    }

    std::clog << std::endl;
}

animation_2d_es_32::animation_2d_es_32(std::string_view path, size_t col,
                                       size_t row)
    : texture_2d_es_32(path)
    , col(col)
    , row(row)
    , current_frame(0)
    , animation_speed(0.015f)
{
    offset_x = 1.f / row;
    offset_y = 1.f / col;

    std::clog << "animation_info:\ncol: " << col << " row: " << row
              << " offset_x: " << offset_x << " offset_y: " << offset_y
              << std::endl
              << "current_frame: " << current_frame << std::endl
              << std::endl;

    make_atlas_map();
}

void animation_2d_es_32::switch_frame()
{
    current_frame = ++current_frame % ((col * row) + 1);
}

void animation_2d_es_32::switch_frame(size_t frame)
{
    current_frame = frame % ((col * row) + 1);
}

void animation_2d_es_32::set_animation_speed(float speed)
{
    animation_speed = speed;
}

v_8 animation_2d_es_32::get_current_frame()
{
    return atlas[current_frame];
}

} // namespace engine
