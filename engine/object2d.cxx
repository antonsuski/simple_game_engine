#include "object2d.hxx"

namespace engine
{
object2d::~object2d() {}

object2d::object2d(std::string buffer_path, std::string shader_path,
                   std::string texture_path, texture::img_format format)
    : _buffer{ buffer_path }
    , _shader{ shader_path + ".vs", shader_path + ".fs" }
    , _texture{ texture_path, format }
{
    size     = _buffer.get_size();
    position = { 0.f, 0.f };
    angle    = 0.f;
    model = view = glm::mat4(1.0f);
}

object2d::object2d(std::string buffer_path, std::string vs_path,
                   std::string fs_path, std::string texture_path,
                   texture::img_format format)
    : _buffer{ buffer_path }
    , _shader{ vs_path, fs_path }
    , _texture{ texture_path, format }
{
    size     = _buffer.get_size();
    position = { 0.f, 0.f };
    angle    = 0.f;
    model = view = glm::mat4(1.0f);
}

const shader_es_32* object2d::get_shader() const
{
    return &_shader;
}

const vbo_8* object2d::get_vbo() const
{
    return &_buffer;
}

const texture* object2d::get_texture() const
{
    return &_texture;
}

glm::mat4 object2d::get_view() const
{
    return glm::mat4(view);
}

glm::mat4 object2d::get_model() const
{
    return glm::mat4(model);
}

bool object2d::is_collide_point(v_2 mouse_coords)
{
    float x_min{ this->position.x - this->size.x / 2.f },
        x_max{ this->position.x + this->size.x / 2.f };
    float y_min{ this->position.y - this->size.y / 2.f },
        y_max{ this->position.y + this->size.y / 2.f };

    return x_min <= mouse_coords.x && mouse_coords.x <= x_max &&
           y_min <= mouse_coords.y && mouse_coords.y <= y_max;
}

void object2d::scale(glm::vec3 vector)
{
    size  = { vector.r * size.x, vector.g * size.y };
    model = glm::scale(model, glm::vec3(size.x, size.y, 0));
}

void object2d::translate(glm::vec3 vector)
{
    position = { vector.r + position.x, vector.g + position.y };
    std::clog << "pos: " << position << std::endl;
    std::clog << "vec " << vector.r << " " << vector.g << std::endl;
    view = glm::translate(view, glm::vec3(position.x, position.y, 0));
}

void object2d::rotate(float angle)
{
    this->angle = angle;
    view = glm::rotate(view, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
}

} // namespace engine
