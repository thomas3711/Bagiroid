#pragma once
#include <string>
#include "plugin_api.h"

// Handles plugin loading from directory
class PluginLoader
{
private:
    bool Load(const std::string& library_path);

    void* handle = nullptr;
    std::string path;
    PluginGenerateFn generate_fn = nullptr;
    PluginFreeFn free_fn = nullptr;

public:
    ~PluginLoader();

    bool LoadFromDirectory(const std::string& directory);
    void Unload();

    bool IsLoaded() const { return handle != nullptr; }
    const std::string& Path() const { return path; }

    GeneratedBricksData* Generate(const BrickGenContext& context) const;
    void Free(GeneratedBricksData* data) const;
};
