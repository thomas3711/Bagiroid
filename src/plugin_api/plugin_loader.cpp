#include "plugin_loader.h"

#include <SDL3/SDL.h>
#include <filesystem>
#include <system_error>

namespace fs = std::filesystem;

namespace
{
    // Platform-specific extension for a dynamically loadable library.
    const char* PluginExtension()
    {
#if defined(_WIN32)
        return ".dll";
#elif defined(__APPLE__)
        return ".dylib";
#else
        return ".so";
#endif
    }
}

PluginLoader::~PluginLoader()
{
    Unload();
}

bool PluginLoader::LoadFromDirectory(const std::string& directory)
{
    std::error_code ec;
    if (!fs::is_directory(directory, ec))
    {
        SDL_Log("Plugin directory '%s' not found, using built-in generator", directory.c_str());
        return false;
    }

    const std::string extension = PluginExtension();

    for (const auto& entry : fs::directory_iterator(directory, ec))
    {
        if (ec)
        {
            break;
        }

        if (!entry.is_regular_file())
        {
            continue;
        }

        if (entry.path().extension() != extension)
        {
            continue;
        }

        if (Load(entry.path().string()))
        {
            return true;
        }
    }

    SDL_Log("No compatible plugin found in '%s', using built-in generator", directory.c_str());
    return false;
}

bool PluginLoader::Load(const std::string& library_path)
{
    // Load library
    SDL_SharedObject* lib = SDL_LoadObject(library_path.c_str());
    if (lib == nullptr)
    {
        SDL_Log("Failed to load plugin '%s': %s", library_path.c_str(), SDL_GetError());
        return false;
    }

    // Cast the function pointers
    auto version_fn = reinterpret_cast<PluginApiVersionFn>(SDL_LoadFunction(lib, PLUGIN_API_VERSION_SYMBOL));
    auto gen_fn = reinterpret_cast<PluginGenerateFn>(SDL_LoadFunction(lib, PLUGIN_GENERATE_SYMBOL));
    auto free_fn_local = reinterpret_cast<PluginFreeFn>(SDL_LoadFunction(lib, PLUGIN_FREE_SYMBOL));

    if (version_fn == nullptr || gen_fn == nullptr || free_fn_local == nullptr)
    {
        SDL_Log("Plugin '%s' is missing required symbols, skipping", library_path.c_str());
        SDL_UnloadObject(lib);
        return false;
    }

    int version = version_fn();
    if (version != PLUGIN_VERSION)
    {
        SDL_Log("Plugin '%s' reports API version %d, host expects %d, skipping", library_path.c_str(), version, PLUGIN_VERSION);
        SDL_UnloadObject(lib);
        return false;
    }

    Unload();

    handle = lib;
    path = library_path;
    generate_fn = gen_fn;
    free_fn = free_fn_local;

    SDL_Log("Loaded brick plugin '%s' (API version %d)", library_path.c_str(), version);
    return true;
}

void PluginLoader::Unload()
{
    if (handle != nullptr)
    {
        SDL_UnloadObject(static_cast<SDL_SharedObject*>(handle));
        handle = nullptr;
    }

    path.clear();
    generate_fn = nullptr;
    free_fn = nullptr;
}

GeneratedBricksData* PluginLoader::Generate(const BrickGenContext& context) const
{
    if (generate_fn == nullptr)
    {
        return nullptr;
    }

    return generate_fn(&context);
}

void PluginLoader::Free(GeneratedBricksData* data) const
{
    if (free_fn != nullptr && data != nullptr)
    {
        free_fn(data);
    }
}
