#include "engine.hxx"
#include "glad/glad.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "om_gl_check.hxx"
#include "stuff.hxx"

#include <SDL.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <shader.hxx>

bool init_imgui(SDL_Window* window, SDL_GLContext gl_context,
                uint8_t glsl_version);
void create_imgui_tool_window();
void render_imgui();

static std::ostream& operator<<(std::ostream& out, const SDL_version& v)
{
    out << static_cast<int>(v.major) << ".";
    out << static_cast<int>(v.minor) << ".";
    out << static_cast<int>(v.patch);
    return out;
}

namespace engine
{
bool init_imgui(SDL_Window* window, SDL_GLContext gl_context,
                uint8_t glsl_version);
void create_imgui_tool_window();
void render_imgui();

std::ostream& operator<<(std::ostream& stream, const event& e)
{
    std::uint32_t value   = static_cast<std::uint32_t>(e.key);
    std::uint32_t minimal = static_cast<std::uint32_t>(event::up);
    std::uint32_t maximal = static_cast<std::uint32_t>(event::turn_off);
    if (value >= minimal && value <= maximal)
    {
        if (e.is_running)
        {
            stream << e.name;
            stream << "_running";
            return stream;
        }
        stream << e.name;
        stream << "_is_finished";
        return stream;
    }
    else
    {
        throw std::runtime_error("too big event value");
    }
}

struct bind
{
    bind(SDL_Keycode bind_key, event bind_event)
        : key(bind_key)
        , e(bind_event)
    {
    }

    SDL_Keycode key;
    event       e;
};

const std::array<bind, 8> keys{ {
    { SDLK_w, { event::up, "up", false } },
    { SDLK_a, { event::left, "left", false } },
    { SDLK_s, { event::down, "down", false } },
    { SDLK_d, { event::right, "right", false } },
    { SDLK_q, { event::button_1, "button_1", false } },
    { SDLK_e, { event::button_2, "button_2", false } },
    { SDLK_ESCAPE, { event::select, "select", false } },
    { SDLK_RETURN, { event::start, "start", false } },
} };

static bool check_event(const SDL_Event& sdl_event, const bind*& key)
{
    using namespace std;

    const auto it = find_if(begin(keys), end(keys), [&](const bind& b) {
        return b.key == sdl_event.key.keysym.sym;
    });

    if (it != end(keys))
    {
        key = &(*it);
        return true;
    }
    return false;
}

class core_one final : public core
{
private:
    SDL_Window*   window      = nullptr;
    SDL_GLContext gl_context  = nullptr;
    SDL_version   compiled    = { 0, 0, 0 };
    SDL_version   linked      = { 0, 0, 0 };
    GLuint        program_id_ = 0;
    GLuint        VBO         = 0;
    GLuint        VAO         = 0;
    GLuint        EBO         = 0;
    GLuint        shd_proc    = 0;
    int           width_      = 0;
    int           height_     = 0;

public:
    uniform tmp_uni;

    void mouse_capture(bool flag) override final
    {
        // SDL_SetWindowGrab(window, SDL_TRUE);
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    void set_uniforms()
    {
        //        glUseProgram(shd_proc);
        //        OM_GL_CHECK()

        GLint uniform_id = glGetUniformLocation(shd_proc, "in_uniform");
        OM_GL_CHECK()

        // std::cout << uniform_id << std::endl;

        glUseProgram(shd_proc);
        OM_GL_CHECK()

        glUniform4f(uniform_id, tmp_uni.u0, tmp_uni.u1, tmp_uni.u2, tmp_uni.u3);
        OM_GL_CHECK()
    }

    ~core_one() final override {}

    void genBuffers()
    {
        glGenBuffers(1, &VBO);
        OM_GL_CHECK()
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        OM_GL_CHECK()
        glGenVertexArrays(1, &VAO);
        OM_GL_CHECK()
        glBindVertexArray(VAO);
        OM_GL_CHECK()
    }

    bool init_my_opengl() override
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(callback_opengl_debug, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                              nullptr, GL_TRUE);

        glViewport(0, 0, width_, height_);
        OM_GL_CHECK()

        std::clog << "Resolution: " << width_ << "x" << height_ << std::endl;

        genBuffers();

        glEnable(GL_DEPTH_TEST);
        OM_GL_CHECK()

        glEnable(GL_BLEND);
        OM_GL_CHECK()
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        OM_GL_CHECK()

        return true;
    }

    bool init_opengl()
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(callback_opengl_debug, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                              nullptr, GL_TRUE);
        // init_my_opengl();
        genBuffers();
        GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
        OM_GL_CHECK()
        std::string_view vertex_shader_src = R"(
                                        attribute vec3 a_position;
                                        varying vec4 v_position;

                                        void main()
                                        {
                                            v_position = vec4(a_position, 1.0);
                                            gl_Position = v_position;
                                        }
                                        )";
        const char*      source            = vertex_shader_src.data();
        glShaderSource(vert_shader, 1, &source, nullptr);
        OM_GL_CHECK()

        glCompileShader(vert_shader);
        OM_GL_CHECK()

        GLint compiled_status = 0;
        glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &compiled_status);
        OM_GL_CHECK()
        if (compiled_status == 0)
        {
            GLint info_len = 0;
            glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &info_len);
            OM_GL_CHECK()
            std::vector<char> info_chars(static_cast<size_t>(info_len));
            glGetShaderInfoLog(vert_shader, info_len, nullptr,
                               info_chars.data());
            OM_GL_CHECK()
            glDeleteShader(vert_shader);
            OM_GL_CHECK()

            std::string shader_type_name = "vertex";
            std::cerr << "Error compiling shader(vertex)\n"
                      << vertex_shader_src << "\n"
                      << info_chars.data();
            return false;
        }
        // fargmemt shader
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        OM_GL_CHECK()
        std::string_view fragment_shader_src = R"(
                          precision mediump float;
                          varying mediump vec4 v_position;

                          void main()
                          {
                              if (v_position.z >= 0.0)
                              {
                                  float light_green = 0.5 + v_position.z / 2.0;
                                  gl_FragColor = vec4(0.0, light_green, 0.0, 1.0);
                              } else
                              {
                                  float color = 0.5 - (v_position.z / -2.0);
                                  gl_FragColor = vec4(color, 0.0, 0.0, 1.0);
                              }
                          }
                          )";
        source                               = fragment_shader_src.data();
        glShaderSource(fragment_shader, 1, &source, nullptr);
        OM_GL_CHECK()

        glCompileShader(fragment_shader);
        OM_GL_CHECK()

        compiled_status = 0;
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled_status);
        OM_GL_CHECK()
        if (compiled_status == 0)
        {
            GLint info_len = 0;
            glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &info_len);
            OM_GL_CHECK()
            std::vector<char> info_chars(static_cast<size_t>(info_len));
            glGetShaderInfoLog(fragment_shader, info_len, nullptr,
                               info_chars.data());
            OM_GL_CHECK()
            glDeleteShader(fragment_shader);
            OM_GL_CHECK()

            std::cerr << "Error compiling shader(fragment)\n"
                      << vertex_shader_src << "\n"
                      << info_chars.data();

            // glEnable(GL_DEPTH_TEST);

            return false;
        }

        // now create program and attach vertex and fragment shaders

        program_id_ = glCreateProgram();
        OM_GL_CHECK()
        if (0 == program_id_)
        {
            std::cerr << "failed to create gl program";
            return false;
        }

        glAttachShader(program_id_, vert_shader);
        OM_GL_CHECK()
        glAttachShader(program_id_, fragment_shader);
        OM_GL_CHECK()

        // bind attribute location
        glBindAttribLocation(program_id_, 0, "a_position");
        OM_GL_CHECK()
        // link program after binding attribute locations
        glLinkProgram(program_id_);
        OM_GL_CHECK()
        // Check the link status
        GLint linked_status = 0;
        glGetProgramiv(program_id_, GL_LINK_STATUS, &linked_status);
        OM_GL_CHECK()
        if (linked_status == 0)
        {
            GLint infoLen = 0;
            glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &infoLen);
            OM_GL_CHECK()
            std::vector<char> infoLog(static_cast<size_t>(infoLen));
            glGetProgramInfoLog(program_id_, infoLen, nullptr, infoLog.data());
            OM_GL_CHECK()
            std::cerr << "Error linking program:\n" << infoLog.data();
            glDeleteProgram(program_id_);
            OM_GL_CHECK()
            return false;
        }

        // turn on rendering with just created shader program
        glUseProgram(program_id_);
        OM_GL_CHECK()

        // glEnable(GL_DEPTH_TEST);
        // glDisable(GL_DEPTH_TEST);
        return true;
    }

    bool init(size_t width, size_t height) final override
    {
        //        SDL_version compiled = { 0, 0, 0 };
        //        SDL_version linked   = { 0, 0, 0 };

        SDL_VERSION(&compiled)
        SDL_GetVersion(&linked);

        if (width >= 1366 || height >= 768)
        {
            std::cerr << "uncorrect resolution" << std::endl;
            width_  = 640;
            height_ = 320;
        }
        else
        {
            width_  = width;
            height_ = height;
        }

        if (SDL_COMPILEDVERSION !=
            SDL_VERSIONNUM(linked.major, linked.minor, linked.patch))
        {
            std::cerr << "warning: SDL2 compiled and linked version mismatch: "
                      << compiled << " " << linked << std::endl;
        }

        if (SDL_Init(SDL_INIT_EVERYTHING))
        {
            std::cerr << "init failed sdl_error: " << SDL_GetError()
                      << std::endl;
            return false;
        }
        else
        {
            window = SDL_CreateWindow(
                "title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width,
                height, SDL_WINDOW_OPENGL | SDL_WINDOW_MOUSE_CAPTURE);
            if (window == nullptr)
            {
                std::cerr << "create windoe failed sdl_error: "
                          << SDL_GetError() << std::endl;
                return false;
            }
            int gl_major_ver       = 3;
            int gl_minor_ver       = 2;
            int gl_context_profile = SDL_GL_CONTEXT_PROFILE_ES;

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                                SDL_GL_CONTEXT_PROFILE_ES);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_ver);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_ver);

            gl_context = SDL_GL_CreateContext(window);

            if (gl_context == nullptr)
            {
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                                    SDL_GL_CONTEXT_PROFILE_CORE);
                gl_context = SDL_GL_CreateContext(window);
            }
            int result = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,
                                             &gl_major_ver);
            assert(result == 0);
            result = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
                                         &gl_minor_ver);
            assert(result == 0);

            if (gl_major_ver != 3 || gl_minor_ver != 2)
            {
                std::clog << "current context opengl version: " << gl_major_ver
                          << '.' << gl_minor_ver << '\n'
                          << "need openg ES version at least: 3.2\n"
                          << std::flush;
                throw std::runtime_error("opengl version too low");
            }
            else
            {
                if (gl_context_profile == SDL_GL_CONTEXT_PROFILE_ES)
                {
                    std::clog << "OpenGL ES " << gl_major_ver << '.'
                              << gl_minor_ver << '\n';
                }
                else
                {
                    std::clog << "OpenGL " << gl_major_ver << '.'
                              << gl_minor_ver << '\n';
                }
            }

            if (gladLoadGLES2Loader(SDL_GL_GetProcAddress) == 0)
            {
                std::clog << "error: failed to initialize glad" << std::endl;
            }

            if (SDL_SetWindowInputFocus(window))
            {
                std::clog << SDL_GetError() << std::endl;
            }

            if (SDL_CaptureMouse(SDL_TRUE))
            {
                std::clog << SDL_GetError() << std::endl;
            }
        }

        engine::init_imgui(window, gl_context, static_cast<uint8_t>(300));
        return init_my_opengl();
    }
    bool read_events(events& es) override
    {
        SDL_Event sdl_event;
        if (SDL_PollEvent(&sdl_event))
        {
            es = SDL_GetKeyboardState(nullptr);
        }
    }
    bool read_event(event& e) final override
    {
        SDL_Event sdl_event;
        if (SDL_PollEvent(&sdl_event))
        {
            const bind* key = nullptr;
            if (sdl_event.type == SDL_QUIT)
            {
                e.key  = event::turn_off;
                e.name = "turn_off";
                return true;
            }
            if (sdl_event.type == SDL_KEYDOWN)
            {
                if (check_event(sdl_event, key))
                {
                    e.key        = key->e.key;
                    e.name       = key->e.name;
                    e.is_running = true;
                    return true;
                }
            }
            if (sdl_event.type == SDL_KEYUP)
            {
                if (check_event(sdl_event, key))
                {
                    e.key        = key->e.key;
                    e.name       = key->e.name;
                    e.is_running = false;
                    return true;
                }
            }
            if (sdl_event.type == SDL_MOUSEMOTION)
            {
                float original_x = sdl_event.motion.x;
                float original_y = sdl_event.motion.y;
                float x = 0.0, y = 0.0;

                int width = 0, height = 0;
                SDL_GetWindowSize(window, &width, &height);
                if (width == 0 || height == 0)
                {
                    throw std::runtime_error(
                        "Error! Cannot get window's size.");
                }

                // float x_center = float(width) / 2.0f;
                // float y_center = float(height) / 2.0f;
                x = original_x / float(width) * 2.0f - 1.0f;
                y = original_y / float(height) * 2.0f - 1.0f;
                y *= -1.0f;
                //                tmp_uni.u0 = x;
                //                tmp_uni.u1 = y;
                //                tmp_uni.u2 = 0.4f;
                //                tmp_uni.u3 = 1.0f;

                e.key           = event::mouse_move;
                e.mouse_delta.x = sdl_event.motion.xrel;
                e.mouse_delta.y = sdl_event.motion.yrel;
                e.is_running    = true;
                return true;
            }
        }
        return false;
    }

    void swap_buffers() final override
    {
        SDL_GL_SwapWindow(window);

        glClearColor(0.3f, 0.3f, 0.8f, 0.0f);
        OM_GL_CHECK()
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        OM_GL_CHECK()
    }

    void uninitialize() final override
    {
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    std::vector<engine::v_8> make_grid_(size_t x, size_t y,
                                        std::vector<uint32_t>& indexes_v)
    {
        engine::triangle         tr;
        std::vector<engine::v_8> vert_arr;

        const float x_offset = 2.0f / static_cast<float>(x - 1);
        const float y_offset = 2.0f / static_cast<float>(y - 1);
        const float x_start  = -1.0f;
        const float y_start  = 1.0f;

        //        int   x_count = 639 / 10;
        //        int   y_count = 379 / 10;
        //        float x_step  = 2.0f / float(x_count);
        //        float y_step  = 2.0f / float(y_count);

        for (size_t iterator_y = 0; iterator_y < y; ++iterator_y)
        {
            for (size_t iterator_x = 0; iterator_x < x; ++iterator_x)
            {
                engine::v_8 tmp(
                    x_start + static_cast<float>(iterator_x) * x_offset,
                    y_start - static_cast<float>(iterator_y) * y_offset, 0.f,
                    1.f, 1.f, 1.f, 0.f, 0.f);
                vert_arr.push_back(tmp);
            }
        }

        for (size_t j = 0; j < y - 1; ++j)
        {
            for (size_t i = 0; i < x - 1; ++i)
            {
                uint16_t v0 = j * x + i;
                uint16_t v1 = v0 + 1;
                uint16_t v2 = v0 + x;
                uint16_t v3 = v2 + 1;

                // add two triangles
                //  v0-----v1
                //  |     /|
                //  |    / |
                //  |   /  |
                //  |  /   |
                //  | /    |
                //  v2-----v3
                // we want only cells without internal color fill
                // so generate "triangle" for every age

                indexes_v.insert(end(indexes_v), { v0, v1, v2 });
                indexes_v.insert(end(indexes_v), { v2, v1, v3 });
            }
        }
        indexes_v.push_back(x * y - x);

        //        for (auto i : vert_arr)
        //        {
        //            std::cout << "pos: " << i.x << " " << i.y << " " << i.z <<
        //            '\n'; std::cout << "col: " << i.r << " " << i.g << " " <<
        //            i.b << " "
        //                      << std::endl;
        //        }
        //        int counter = 0;
        //        for (auto i : indexes_v)
        //        {
        //            ++counter;
        //            std::cout << i << " ";
        //            if (counter == 3)
        //            {
        //                counter = 0;
        //                std::cout << std::endl;
        //            }
        //        }

        return vert_arr;
    }

    void render(vbo_v_8& buffer, shader_es_32& shader,
                GLuint& texture_id) final override
    {
        shader.use();

        buffer.bind_vao();
        buffer.bind_buffer();
        glBindTexture(GL_TEXTURE_2D, texture_id);
        OM_GL_CHECK()
        buffer.buffer_data(GL_STATIC_DRAW);
        buffer.bind_ebo();
        buffer.buffer_ebo();
        glBindTexture(GL_TEXTURE_2D, texture_id);
        OM_GL_CHECK()
        glDrawElements(GL_TRIANGLES, buffer.ebo_size, GL_UNSIGNED_INT, 0);
        OM_GL_CHECK()
    }

    void render(vbo_v_8& buffer, shader_es_32& shader, GLuint& texture_id,
                GLuint& texture_) final override
    {
        shader.use();

        buffer.bind_vao();
        buffer.bind_buffer();
        glActiveTexture(GL_TEXTURE0);
        OM_GL_CHECK()
        glBindTexture(GL_TEXTURE_2D, texture_id);
        OM_GL_CHECK()
        glActiveTexture(GL_TEXTURE1);
        OM_GL_CHECK()
        glBindTexture(GL_TEXTURE_2D, texture_);
        OM_GL_CHECK()
        buffer.buffer_data(GL_STATIC_DRAW);
        buffer.bind_ebo();
        buffer.buffer_ebo();
        //        glBindTexture(GL_TEXTURE_2D, texture_id);
        //        OM_GL_CHECK()
        glDrawElements(GL_TRIANGLES, buffer.ebo_size, GL_UNSIGNED_INT, 0);
        OM_GL_CHECK()
    }

    void render(vbo_v_8& buffer, shader_es_32& shader,
                texture_2d_es_32& txt) final override
    {
        create_imgui_tool_window();
        ImGui::Render();

        shader.use();

        buffer.bind_vao();
        buffer.bind_buffer();
        buffer.buffer_data(GL_STATIC_DRAW);
        buffer.bind_ebo();
        buffer.buffer_ebo();

        txt.bind();

        glDrawElements(GL_TRIANGLES, buffer.ebo_size, GL_UNSIGNED_INT, 0);
        OM_GL_CHECK()

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void render_grid(shader_es_32& shader_) final override
    {
        shader_.use();
        shd_proc = shader_.id;
        set_uniforms();

        std::vector<uint32_t>* indexes   = new std::vector<uint32_t>;
        std::vector<v_8>       vert_buff = make_grid_(70, 70, *indexes);

        indexes->push_back(0);
        indexes->push_back(1);
        indexes->push_back(2);

        glBindVertexArray(VAO);
        OM_GL_CHECK()
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        OM_GL_CHECK()
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        OM_GL_CHECK()
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(uint32_t) * indexes->size(), indexes->data(),
                     GL_STATIC_DRAW);
        OM_GL_CHECK()
        glBufferData(GL_ARRAY_BUFFER, sizeof(v_8) * vert_buff.size(),
                     vert_buff.data(), GL_STATIC_DRAW);
        OM_GL_CHECK()
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(v_8), nullptr);
        OM_GL_CHECK()
        glEnableVertexAttribArray(0);
        OM_GL_CHECK()
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(v_8),
                              reinterpret_cast<void*>(sizeof(float) * 3));
        OM_GL_CHECK()
        glEnableVertexAttribArray(1);
        OM_GL_CHECK()
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(v_8),
                              reinterpret_cast<void*>(sizeof(float) * 2));
        OM_GL_CHECK()
        GLuint shd_proc_value = shader_.id;
        glValidateProgram(shd_proc_value);
        OM_GL_CHECK()

        // Check the validate status

        GLint validate_status = 0;
        glGetProgramiv(shd_proc_value, GL_VALIDATE_STATUS, &validate_status);
        OM_GL_CHECK()

        if (validate_status == GL_FALSE)
        {
            GLint infoLen = 0;
            glGetProgramiv(shd_proc_value, GL_INFO_LOG_LENGTH, &infoLen);
            OM_GL_CHECK()
            std::vector<char> infoLog(static_cast<size_t>(infoLen));
            glGetProgramInfoLog(shd_proc_value, infoLen, nullptr,
                                infoLog.data());
            OM_GL_CHECK()
            std::cerr << "Error linking program:\n" << infoLog.data();
            delete indexes;
            throw std::runtime_error("error");
        }

        // set_uniforms();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        OM_GL_CHECK()
        glDrawElements(GL_LINES, indexes->size(), GL_UNSIGNED_INT, 0);
        OM_GL_CHECK()

        delete indexes;
    }

    float get_time_for_init() final override
    {
        std::uint32_t ms_from_library_initialization = SDL_GetTicks();
        float         seconds = ms_from_library_initialization * 0.001f;
        return seconds;
    }
};

static bool already_exist = false;

core* create_engine()
{
    if (already_exist)
    {
        throw std::runtime_error("engine already exist");
    }
    core* result  = new core_one();
    already_exist = true;
    return result;
}

void destroy_engine(core* e)
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

core::~core(){};

bool   show_demo_window    = true;
bool   show_another_window = false;
ImVec4 clear_color         = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

bool init_imgui(SDL_Window* window, SDL_GLContext gl_context,
                uint8_t glsl_version)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    const ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(reinterpret_cast<char*>(glsl_version));
    return true;
}

void create_imgui_tool_window()
{
    static float f       = 0.0f;
    static int   counter = 0;

    ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and
                                   // append into it.

    ImGui::Text("This is some useful text."); // Display some text (you can use
                                              // a format strings too)
    ImGui::Checkbox(
        "Demo Window",
        &show_demo_window); // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f,
                       1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3(
        "clear color",
        (float*)&clear_color); // Edit 3 floats representing a color

    if (ImGui::Button("Button")) // Buttons return true when clicked (most
                                 // widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

} // namespace engine
