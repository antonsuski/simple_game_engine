#include <algorithm>
#include <array>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <vector>

#include <SDL.h>

#include "engine.hxx"
#include "gl_assist.hxx"
#include "glad.h"

static std::ostream& operator<<(std::ostream& out, const SDL_version& v)
{
    out << static_cast<int>(v.major) << ".";
    out << static_cast<int>(v.minor) << ".";
    out << static_cast<int>(v.patch);
    return out;
}

static bool init_default_shader(unsigned int& shader_prog_id)
{
    unsigned int vert_sh;
    unsigned int frag_sh;
    int          success;
    char         infoLog[512];

    const char* vertex_sh_src =
        "#version 320 es                                      \n"
        "precision mediump float;                             \n"
        "layout(location = 0) in vec3 a_pos;                  \n"
        "void main(void)                                      \n"
        "{                                                    \n"
        "   gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);\n"
        "}                                                    \0";

    vert_sh = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_sh, 1, &vertex_sh_src, NULL);
    glCompileShader(vert_sh);
    glGetShaderiv(vert_sh, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vert_sh, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    const char* fragment_sh_src =
        "#version 320 es                                       \n"
        "precision mediump float;                             \n"
        "out vec4 frag_color;                                  \n"
        "uniform vec2 mouse_coord;                             \n"
        "void main(void)                                       \n"
        "{                                                     \n"
        "float b_color = abs((mouse_coord.x + mouse_coord.y) / 2.0f);\n"
        "   frag_color = vec4(mouse_coord.y, mouse_coord.x, 0.2f, \n"
        "1.0f);         \n"
        "}                                                     \0";

    frag_sh = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_sh, 1, &fragment_sh_src, NULL);
    glCompileShader(frag_sh);
    glGetShaderiv(frag_sh, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(frag_sh, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    shader_prog_id = glCreateProgram();
    glAttachShader(shader_prog_id, vert_sh);
    glAttachShader(shader_prog_id, frag_sh);
    glLinkProgram(shader_prog_id);
    glGetProgramiv(shader_prog_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_prog_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vert_sh);
    glDeleteShader(frag_sh);

    return static_cast<bool>(success);
}

struct bind
{
    engine::event event;
    SDL_KeyCode   key_code;

    bind(SDL_KeyCode key_code_, engine::event::event_type event_,
         std::string name_)
        : event(event_, name_)
        , key_code(key_code_)
    {
    }
};

static const bind unknown_bind{ SDLK_UNKNOWN, engine::event::unknown,
                                "unknown" };

std::array<bind, 9> key_map{ { { SDLK_a, engine::event::left, "left" },
                               { SDLK_d, engine::event::right, "right" },
                               { SDLK_w, engine::event::up, "up" },
                               { SDLK_s, engine::event::down, "down" },
                               { SDLK_q, engine::event::button_1, "button_1" },
                               { SDLK_e, engine::event::button_2, "button_2" },
                               { SDLK_SPACE, engine::event::select, "select" },
                               { SDLK_RETURN, engine::event::start, "start" },
                               { SDLK_ESCAPE, engine::event::turn_off,
                                 "turn_off" } } };

static bool check_event(const SDL_Event& sdl_event, const bind*& engine_bind)
{
    const auto it = std::find_if(
        key_map.begin(), key_map.end(),
        [&](const bind& b) { return b.key_code == sdl_event.key.keysym.sym; });

    if (it != end(key_map))
    {
        engine_bind = &(*it);
        return true;
    }
    engine_bind = &unknown_bind;
    return false;
}

namespace engine
{

std::ostream& operator<<(std::ostream& out, const event& e)
{
    std::uint32_t value   = static_cast<std::uint32_t>(e.type);
    std::uint32_t minimal = static_cast<std::uint32_t>(event::left);
    std::uint32_t maximal = static_cast<std::uint32_t>(event::turn_off);
    if (value >= minimal && value <= maximal)
    {
        if (e.event_state)
        {
            out << e.name;
            out << "_running";
            return out;
        }
        out << e.name;
        out << "_is_finished";
        return out;
    }
    else
    {
        throw std::runtime_error("too big event value");
    }
}

event::event()
    : name("uknown")
    , type(event::unknown)
    , event_state(false)
{
}

event::event(event::event_type e_type, std::string e_name)
    : type(e_type)
    , name(e_name)
    , event_state(false)
{
}

class engine_core final : public engine
{
    SDL_Window*   window{ nullptr };
    SDL_GLContext gl_context{};

    int32_t main_window_height{}, main_window_width{};

    float mouse_coords_x, mouse_coords_y;

    uint32_t shader_prog_id;

public:
    bool handl_imput(event& e) override final
    {
        SDL_Event sdl_e;

        if (SDL_PollEvent(&sdl_e))
        {
            const bind* key_bind = nullptr;

            if (sdl_e.type == SDL_QUIT)
            {
                e.type = event::turn_off;
                e.name = "turn_off";
                return true;
            }
            if (sdl_e.type == SDL_KEYDOWN)
            {
                if (check_event(sdl_e, key_bind))
                {
                    e.event_state = true;
                    e.type        = key_bind->event.type;
                    e.name        = key_bind->event.name;
                    return true;
                }
            }
            if (sdl_e.type == SDL_KEYUP)
            {
                if (check_event(sdl_e, key_bind))
                {
                    e.event_state = true;
                    e.type        = key_bind->event.type;
                    e.name        = key_bind->event.name;
                    return true;
                }
            }
            if (sdl_e.type == SDL_MOUSEMOTION)
            {
                SDL_GetWindowSize(window, &main_window_width,
                                  &main_window_height);

                mouse_coords_x =
                    ((2.f * sdl_e.motion.x / main_window_width)) - 1.f;
                mouse_coords_y =
                    ((2.f * sdl_e.motion.y / main_window_height)) - 1.f;
                mouse_coords_y *= -1.f;

                std::cout << std::setprecision(2) << "mcX:" << mouse_coords_x
                          << std::setw(10) << "mcY:" << mouse_coords_y
                          << std::endl;
                return true;
            }
        }

        return false;
    }

    void update() override final {}
    void render() override final {}

    void render(const vbo_v_3& vbo_buffer) final override
    {
        int uniform;
        glUseProgram(shader_prog_id);
        uniform = glGetUniformLocation(shader_prog_id, "mouse_coord");
        glUniform2f(uniform, mouse_coords_x, mouse_coords_y);
        vbo_buffer.bind_vao();
        glDrawArrays(GL_TRIANGLES, 0, vbo_buffer.get_vertex_count());
        glBindVertexArray(0);
    }

    void render(const vbo_v_3&      vbo_buffer,
                const shader_es_32& shader) final override
    {
        int uniform;

        vbo_buffer.bind_vao();
        shader.use();

        uniform = glGetUniformLocation(shader_prog_id, "mouse_coord");
        glUniform2f(uniform, mouse_coords_x, mouse_coords_y);

        glDrawArrays(GL_TRIANGLES, 0, vbo_buffer.get_vertex_count());
        glBindVertexArray(0);
    }

    void render(const vbo_6&        vbo_buffer,
                const shader_es_32& shader) final override
    {
        shader.use();
        vbo_buffer.bind_vao();
        glDrawArrays(GL_TRIANGLES, 0, vbo_buffer.get_vertex_count());
        glBindVertexArray(0);
    }

    void render(const vbo_v_8&      vbo_buffer,
                const shader_es_32& shader) final override
    {
        int uniform;

        shader.use();
        //        uniform = glGetUniformLocation(shader_prog_id, "mouse_coord");
        //        glUniform2f(uniform, mouse_coords_x, mouse_coords_y);
        vbo_buffer.bind_vao();
        glDrawElements(GL_TRIANGLES, vbo_buffer.get_vertex_conut(),
                       GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void render(const vbo_8&        vbo_buffer,
                const shader_es_32& shader) final override
    {
        shader.use();
        vbo_buffer.bind_vao();
        glDrawArrays(GL_TRIANGLES, 0, vbo_buffer.get_vertex_count());

        // glDrawElements(GL_TRIANGLES, vbo_buffer.get_index_count(),
        //                GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void render(const vbo_8& vbo_buffer, const shader_es_32& shader,
                const texture& texture) final override
    {
        texture.bind_texture();
        shader.use();
        vbo_buffer.bind_vao();
        glDrawArrays(GL_TRIANGLES, 0, vbo_buffer.get_vertex_count());
    }

    void render(const object2d& object) final override { object.render(); }

    void swap_buffers() final override
    {
        SDL_GL_SwapWindow(window);
        glViewport(0, 0, main_window_width, main_window_height);
        glClearColor(0.3f, 0.3f, 0.8f, 0.0f);
        GL_CHECK()
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GL_CHECK()
    }

    bool init(int32_t w, int32_t h) override final
    {
        main_window_width  = w;
        main_window_height = h;

        SDL_version compiled{ 0, 0, 0 };
        SDL_version linked{ 0, 0, 0 };

        SDL_VERSION(&compiled);
        SDL_GetVersion(&linked);

        std::clog << "comiled version: " << compiled << std::endl;
        std::clog << "comiled version: " << linked << std::endl;

        if (SDL_COMPILEDVERSION !=
            SDL_VERSIONNUM(linked.major, linked.minor, linked.patch))
        {
            std::cerr << "warning: SDL2 compiled and linked version mismatch: "
                      << compiled << " " << linked << std::endl;
        }

        if (SDL_Init(SDL_INIT_EVERYTHING))
        {
            std::cerr << "init failed sdl_error:" << SDL_GetError()
                      << std::endl;
            return false;
        }
        else
        {
            window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      main_window_width, main_window_height,
                                      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
            std::clog << "sdl_init success" << std::endl;
        }

        int gl_major_ver       = 3;
        int gl_minor_ver       = 2;
        int gl_context_profile = SDL_GL_CONTEXT_PROFILE_ES;

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_ver);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_ver);

        gl_context = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, gl_context);
        SDL_GL_SetSwapInterval(1);

        if (gl_context == nullptr)
        {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                                SDL_GL_CONTEXT_PROFILE_ES);
            gl_context = SDL_GL_CreateContext(window);
        }

        int result =
            SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &gl_major_ver);
        assert(result == 0);
        result =
            SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &gl_minor_ver);
        assert(result == 0);

        if (gl_major_ver != 3 || gl_minor_ver != 2)
        {
            std::clog << "current context opengl version: " << gl_major_ver
                      << '.' << gl_minor_ver << '\n'
                      << "need openg ES version at least: 3.2\n"
                      << std::endl;
            throw std::runtime_error("opengl version too low");
        }
        else
        {
            if (gl_context_profile == SDL_GL_CONTEXT_PROFILE_ES)
            {
                std::clog << "OpenGL ES " << gl_major_ver << '.' << gl_minor_ver
                          << '\n';
            }
            else
            {
                std::clog << "OpenGL Core " << gl_major_ver << '.'
                          << gl_minor_ver << '\n';
            }
        }

        if (gladLoadGLES2Loader(
                static_cast<GLADloadproc>(SDL_GL_GetProcAddress)) == 0)
        {
            std::clog << "error: failed to initialize glad" << std::endl;
        }

        return init_opengl();
    }

    bool init_opengl()
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(callback_opengl_debug, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                              nullptr, GL_TRUE);

        glViewport(0, 0, main_window_width, main_window_height);
        GL_CHECK()

        std::clog << "Resolution: " << main_window_width << "x"
                  << main_window_height << std::endl;

        glEnable(GL_DEPTH_TEST);
        GL_CHECK()
        glEnable(GL_BLEND);
        GL_CHECK()
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        GL_CHECK()

        if (!init_default_shader(shader_prog_id))
        {
            std::cerr << "Can't initializate default shader programm\n";
            return false;
        }

        return true;
    }
    ~engine_core() final override {}

    void tmp_test_method(unsigned int& sh_prog,
                         unsigned int& vao_id) override final
    {
        unsigned int vbo_id;
        unsigned int ebo_id;

        float vertices[]   = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                               0.0f,  0.0f,  0.5f, 0.0f };
        float verts_cube[] = {
            0.5f,  0.5f,  0.0f, // top right
            0.5f,  -0.5f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, // bottom left
            -0.5f, 0.5f,  0.0f  // top left
        };

        unsigned int indices[] = {
            // note that we start from 0!
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

        glGenVertexArrays(1, &vao_id);

        glGenBuffers(1, &vbo_id);
        glGenBuffers(1, &ebo_id);
        glBindVertexArray(vao_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts_cube), verts_cube,
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                     GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void*)0);
        glEnableVertexAttribArray(0);

        init_default_shader(sh_prog);
    }
    virtual void tmp_test_method2(const unsigned int& sh_prog,
                                  const unsigned int& vao_id) override final
    {
        glUseProgram(sh_prog);
        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        //        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
};

static bool already_exist = false;

engine* create_engine()
{
    if (already_exist)
    {
        throw std::runtime_error("engine already exist");
    }
    engine* result = new engine_core();
    already_exist  = true;
    return result;
}

void destroy_engine(engine* e)
{
    if (already_exist == false)
    {
        throw std::runtime_error("engine not created");
    }

    if (nullptr == e)
    {
        throw std::runtime_error("e is nullptr");
    }

    delete e;
}

engine::~engine(){};
} // namespace engine
