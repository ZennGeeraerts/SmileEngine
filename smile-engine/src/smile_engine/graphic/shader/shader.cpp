#include "smpch.h"
#include "shader.h"

namespace smile::graphic
{
    void Shader::SetName( const std::string &assetFile )
    {
        // Find name from asset path
        auto lastSlash = assetFile.find_last_of( "/\\" );
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = assetFile.rfind( '.' );
        auto count = lastDot == std::string::npos ? assetFile.size() - lastSlash : lastDot - lastSlash;
        Name = assetFile.substr( lastSlash, count );
    }
}