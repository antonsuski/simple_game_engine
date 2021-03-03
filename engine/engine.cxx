#include <algorithm>
#include <array>
#include <cassert>
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

std::ostream& operator<<(std::ostream& out, const engine::event& e)
{
    std::uint32_t value   = static_cast<std::uint32_t>(e.type);
    std::uint32_t minimal = static_cast<std::uint32_t>(engine::event::up);
    std::uint32_t maximal = static_cast<std::uint32_t>(engine::event::turn_off);
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

std::array<bind, 5> key_map{ { { SDLK_a, engine::event::left, "left" },
                               { SDLK_d, engine::event::right, "right" },
                               { SDLK_w, engine::event::up, "up" },
                               { SDLK_s, engine::event::down, "down" },
                               { SDLK_ESCAPE, engine::event::turn_off,
                                 "turn_off" } } };

static bool check_event(const SDL_Event& sdl_event, const bind*& engine_bind)
{
    const auto it =
        std::find_if(key_map.begin(), key_map.end(), [&](const bind& b) {
            return b.key_code == sdl_event.key.keysym.sym;
        });

    if (it != end(key_map))
    {
        engine_bind = &(*it);
        return true;
    }

    return false;
}

namespace engine
{
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

class engine_core final : public engine
{
    SDL_Window*   window{ nullptr };
    SDL_GLContext gl_context{};

    int32_t main_window_height{};
    int32_t main_window_width{};

public:
    bool handl_imput(event& e) override final
    {
        SDL_Event sdl_e;
        if (SDL_PollEvent(&sdl_e))
        {
            const bind* key_bind = nullptr;
            switch (sdl_e.type)
            {
                case SDL_QUIT:
                {
                    e.type = event::turn_off;
                    e.name = "turn_off";
                    return true;
                }
                break;
                case SDL_KEYDOWN:
                {
                    check_event(sdl_e, key_bind);
                    e.event_state = true;
                    e.type        = key_bind->event.type;
                    e.name        = key_bind->event.name;
                }
                break;
                case SDL_KEYUP:
                {
                    check_event(sdl_e, key_bind);
                    e.event_state = false;
                    e.type        = key_bind->event.type;
                    e.name        = key_bind->event.name;
                }
                break;
            }
        }
        return true;
    }
    void update() override final {}
    void render() override final {}

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

        return true;
    }
    ~engine_core() final override {}
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
