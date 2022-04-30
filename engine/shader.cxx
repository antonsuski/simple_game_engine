#include "shader.hxx"
#include "gl_assist.hxx"

#include <vector>

namespace engine
{
shader_es_32::shader_es_32(const char* vert_shader_path,
                           const char* frag_shader_path)
{
    std::string   vert_shader_src;
    std::string   frag_shader_src;
    std::ifstream vert_shader_file;
    std::ifstream frag_shader_file;

    vert_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    frag_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vert_shader_file.open(vert_shader_path);
        frag_shader_file.open(frag_shader_path);
        std::stringstream vert_shader_stream, frag_shader_stream;

        vert_shader_stream << vert_shader_file.rdbuf();
        frag_shader_stream << frag_shader_file.rdbuf();

        vert_shader_file.close();
        frag_shader_file.close();

        vert_shader_src = vert_shader_stream.str();
        frag_shader_src = frag_shader_stream.str();
    }
    catch (std::ifstream::failure& ex)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vert_shader_code = vert_shader_src.data();
    const char* frag_shader_code = frag_shader_src.c_str();

    // vertex shader compile

    GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_shader_code, nullptr);
    GL_CHECK()

    glCompileShader(vert_shader);
    GL_CHECK()

    GLint compiled_status = 0;
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &compiled_status);
    GL_CHECK()

    if (compiled_status == 0)
    {
        GLint info_len = 0;
        glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &info_len);
        GL_CHECK()

        std::vector<char> info_chars(static_cast<size_t>(info_len));
        glGetShaderInfoLog(vert_shader, info_len, nullptr, info_chars.data());
        GL_CHECK()

        glDeleteShader(vert_shader);
        GL_CHECK()

        std::string shader_type_name = "vertex";
        std::cerr << "Error compiling shader(vertex)\n"
                  << vert_shader_code << "\n"
                  << info_chars.data();
    }

    // fragment shader compile

    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_shader_code, nullptr);
    GL_CHECK()

    glCompileShader(frag_shader);
    GL_CHECK()

    compiled_status = 0;
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &compiled_status);
    GL_CHECK()
    if (compiled_status == 0)
    {
        GLint info_len = 0;
        glGetShaderiv(frag_shader, GL_INFO_LOG_LENGTH, &info_len);
        GL_CHECK()

        std::vector<char> info_chars(static_cast<size_t>(info_len));
        glGetShaderInfoLog(frag_shader, info_len, nullptr, info_chars.data());
        GL_CHECK()

        glDeleteShader(frag_shader);
        GL_CHECK()

        std::cerr << "Error compiling shader(fragment)\n"
                  << frag_shader_code << "\n"
                  << info_chars.data();
    }

    // gfx   programm

    id = glCreateProgram();
    GL_CHECK()
    if (0 == id)
    {
        std::cerr << "failed to create gl program";
    }

    glAttachShader(id, vert_shader);
    GL_CHECK()

    glAttachShader(id, frag_shader);
    GL_CHECK()

    //    glBindAttribLocation(id, 0, "a_pos");
    //    GL_CHECK()

    //    glBindAttribLocation(id, 1, "a_color");
    //    GL_CHECK()

    glLinkProgram(id);
    GL_CHECK()

    GLint linked_status = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &linked_status);
    GL_CHECK()
    if (linked_status == 0)
    {
        GLint infoLen = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLen);
        GL_CHECK()
        std::vector<char> infoLog(static_cast<size_t>(infoLen));
        glGetProgramInfoLog(id, infoLen, nullptr, infoLog.data());
        GL_CHECK()
        std::cerr << "Shader constructor!\n";
        std::cerr << "Error linking program:\n" << infoLog.data();
        glDeleteProgram(id);
        GL_CHECK()
    }

    glEnable(GL_DEPTH_TEST);
    GL_CHECK()

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
}

void shader_es_32::use()
{
    glUseProgram(id);
    GL_CHECK()
}

void shader_es_32::use() const
{
    glUseProgram(id);
    GL_CHECK()
}

void shader_es_32::set_uniform_4mat(std::string_view uniform_name,
                                    glm::mat4&       mat)
{
    use();
    const int uniform_location = glGetUniformLocation(id, uniform_name.data());
    GL_CHECK()
    if (uniform_location == -1)
    {
        std::cerr << "can't get uniform location from shader\n";
        throw std::runtime_error("can't get uniform location");
    }

    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(mat));
    GL_CHECK()
}

void shader_es_32::set_uniform_4mat(std::string_view uniform_name,
                                    trans_mat_4x4&   mat)
{
    use();
    const int uniform_location = glGetUniformLocation(id, uniform_name.data());
    GL_CHECK()
    if (uniform_location == -1)
    {
        std::cerr << "can't get uniform location from shader\n";
        throw std::runtime_error("can't get uniform location");
    }

    float matrix[16] = { mat.col_0.x, mat.col_1.x, mat.col_2.x, mat.delta.x,
                         mat.col_0.y, mat.col_1.y, mat.col_2.y, mat.delta.y,
                         mat.col_0.z, mat.col_1.z, mat.col_2.z, mat.delta.z,
                         mat.col_0.w, mat.col_1.w, mat.col_2.w, mat.delta.w };
    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, &matrix[0]);
    GL_CHECK()
}

bool already_reported{ false };

void shader_es_32::set_uniform_4f(std::string_view& uniforms_name,
                                  uniform&          uniforms_data)
{
    use();
    glUniform4f(glGetUniformLocation(id, uniforms_name.data()),
                uniforms_data.u0, uniforms_data.u1, uniforms_data.u2,
                uniforms_data.u3);
    GL_CHECK()
}

void shader_es_32::set_uniform_4f(uniform& uniforms_data)
{
    use();
    glUniform4f(glGetUniformLocation(id, uniforms_data.get_name().data()),
                uniforms_data.u0, uniforms_data.u1, uniforms_data.u2,
                uniforms_data.u3);
    GL_CHECK()
}

void shader_es_32::set_uniform_1i(std::string_view uniform_name, GLint& value)
{
    use();
    glUniform1i(glGetUniformLocation(id, uniform_name.data()), value);
    GL_CHECK();
}
} // namespace engine
