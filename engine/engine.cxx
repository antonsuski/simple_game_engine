#include <algorithm>
#include <array>
#include <cassert>
#include <string_view>
#include <vector>

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>

#include "engine.hxx"
#include "om_gl_check.hxx"

PFNGLCREATESHADERPROC            glCreateShader            = nullptr;
PFNGLSHADERSOURCEPROC            glShaderSource            = nullptr;
PFNGLCOMPILESHADERPROC           glCompileShader           = nullptr;
PFNGLGETSHADERIVPROC             glGetShaderiv             = nullptr;
PFNGLGETSHADERINFOLOGPROC        glGetShaderInfoLog        = nullptr;
PFNGLDELETESHADERPROC            glDeleteShader            = nullptr;
PFNGLCREATEPROGRAMPROC           glCreateProgram           = nullptr;
PFNGLATTACHSHADERPROC            glAttachShader            = nullptr;
PFNGLBINDATTRIBLOCATIONPROC      glBindAttribLocation      = nullptr;
PFNGLLINKPROGRAMPROC             glLinkProgram             = nullptr;
PFNGLGETPROGRAMIVPROC            glGetProgramiv            = nullptr;
PFNGLGETPROGRAMINFOLOGPROC       glGetProgramInfoLog       = nullptr;
PFNGLDELETEPROGRAMPROC           glDeleteProgram           = nullptr;
PFNGLUSEPROGRAMPROC              glUseProgram              = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC     glVertexAttribPointer     = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLVALIDATEPROGRAMPROC         glValidateProgram         = nullptr;
// PFNGLDEBUGMESSAGECALLBACKPROC    glDebugMessageCallback    = nullptr;

template <typename T>
static void load_gl_func(const char* func_name, T& result)
{
    void* gl_pointer = SDL_GL_GetProcAddress(func_name);
    if (nullptr == gl_pointer)
    {
        throw std::runtime_error(std::string("can't load GL function") +
                                 func_name);
    }
    result = reinterpret_cast<T>(gl_pointer);
}

static std::ostream& operator<<(std::ostream& out, const SDL_version& v)
{
    out << static_cast<int>(v.major) << ".";
    out << static_cast<int>(v.minor) << ".";
    out << static_cast<int>(v.patch);
    return out;
}

struct bind
{
    engine::event event_;

    bind(engine::event::event_type event_type, std::string name_)
        : event_(event_type, name_)
    {
    }
};

std::array<bind, 5> key_map{ { { engine::event::left, "left" },
                               { engine::event::right, "right" },
                               { engine::event::up, "up" },
                               { engine::event::down, "down" },
                               { engine::event::turn_off, "turn_off" } } };
namespace engine
{
event::event(event::event_type e_type, std::string e_name)
    : type(e_type)
    , name(e_name)
    , event_state(false)
{
}

std::ostream& operator<<(std::ostream& out, const event& e)
{
    std::uint32_t value   = static_cast<std::uint32_t>(e.type);
    std::uint32_t minimal = static_cast<std::uint32_t>(event::up);
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

    class engine_core : public engine
    {
        SDL_Window*   window{ nullptr };
        SDL_GLContext gl_context{};

    public:
        void handl_imput(event& e) override final {}
        void update() override final {}
        void render() override final {}
        bool init() override final
        {
            SDL_version compiled{ 0, 0, 0 };
            SDL_version linked{ 0, 0, 0 };

            SDL_VERSION(&compiled);
            SDL_GetVersion(&linked);

            std::clog << "comiled version: " << compiled;
            std::clog << "comiled version: " << linked;

            if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_VIDEO))
            {
                std::cerr << "init failed sdl_error:" << SDL_GetError()
                          << std::endl;
                return false;
            }
            else
            {
                window = SDL_CreateWindow(
                    "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED, 600, 400, SDL_WINDOW_SHOWN);
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
                    std::clog << "OpenGL Core " << gl_major_ver << '.'
                              << gl_minor_ver << '\n';
                }
            }
            try
            {
                load_gl_func("glCreateShader", glCreateShader);
                load_gl_func("glShaderSource", glShaderSource);
                load_gl_func("glCompileShader", glCompileShader);
                load_gl_func("glGetShaderiv", glGetShaderiv);
                load_gl_func("glGetShaderInfoLog", glGetShaderInfoLog);
                load_gl_func("glDeleteShader", glDeleteShader);
                load_gl_func("glCreateProgram", glCreateProgram);
                load_gl_func("glAttachShader", glAttachShader);
                load_gl_func("glBindAttribLocation", glBindAttribLocation);
                load_gl_func("glLinkProgram", glLinkProgram);
                load_gl_func("glGetProgramiv", glGetProgramiv);
                load_gl_func("glGetProgramInfoLog", glGetProgramInfoLog);
                load_gl_func("glDeleteProgram", glDeleteProgram);
                load_gl_func("glUseProgram", glUseProgram);
                load_gl_func("glVertexAttribPointer", glVertexAttribPointer);
                load_gl_func("glEnableVertexAttribArray",
                             glEnableVertexAttribArray);
                load_gl_func("glValidateProgram", glValidateProgram);
            }
            catch (std::exception& ex)
            {
                return ex.what();
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

            glViewport(0, 0, width_, height_);
            OM_GL_CHECK()

            std::clog << "Resolution: " << width_ << "x" << height_
                      << std::endl;

            genBuffers();

            glEnable(GL_DEPTH_TEST);
            OM_GL_CHECK()

            glEnable(GL_BLEND);
            OM_GL_CHECK()
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            OM_GL_CHECK()

            return true;
        }
    };
}

} // namespace engine
