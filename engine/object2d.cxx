#include "object2d.hxx"

namespace engine
{
object2d::~object2d() {}

object2d::object2d(std::string buffer_path, std::string shader_path,
                   std::string texture_path)
    : _buffer{ buffer_path }
    , _shader{ shader_path + ".vs", shader_path + ".fs" }
    , _texture{ texture_path }
{
}

void object2d::render() const
{
    _texture.bind_texture();
    _shader.use();
    _buffer.bind_vao();
    glDrawArrays(GL_TRIANGLES, 0, _buffer.get_vertex_count());
}

} // namespace engine
