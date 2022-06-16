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

void object2d::move() const {}

bool object2d::is_collide_sqr(v_2 mouse_coords)
{
    bool collisionX = this->position.x + this->size.x/2.f >= mouse_coords.x &&
                      mouse_coords.x + 0.01 >= this->position.x;
    std::cout << "clox: " << collisionX << std::endl;
    //  collision y-axis?
    bool collisionY = this->position.y + this->size.y/2.f >= mouse_coords.y &&
                      mouse_coords.y + 0.01 >= this->position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

bool object2d::is_collide_cir(v_2 mouse_coords)
{
    // get center point circle first
    glm::vec2 center(glm::vec2(mouse_coords.x, mouse_coords.y) + 0.1f);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(this->size.x / 2.0f, this->size.y / 2.0f);
    glm::vec2 aabb_center(this->position.x + aabb_half_extents.x,
                          this->position.y + aabb_half_extents.y);
    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped =
        glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // add clamped value to AABB_center and we get the value of box closest to
    // circle
    glm::vec2 closest = aabb_center + clamped;
    // retrieve vector between center circle and closest point AABB and check if
    // length <= radius
    difference = closest - center;
    return glm::length(difference) < 0.1f;
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
