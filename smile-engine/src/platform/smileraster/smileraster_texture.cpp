#include "smpch.h"
#include "smileraster_texture.h"

#include "smile_engine/core/logger.h"
#include "smile_engine/core/application.h"

namespace smile::renderer
{
    SmileRasterTexture2D::SmileRasterTexture2D( const std::string &file_path )
        : filePath{ file_path }, surface{ IMG_Load( file_path.c_str() ) }
    {
        smileRasterContext =
            static_cast< SmileRasterContext * >( Application::getInstance().getWindow().getGraphicsContext() );
        SM_ASSERT( smileRasterContext, "SmileRasterTexture2D > Rendering context is not a SmileRasterContext" );

        if ( !surface )
            return;

        pixels = static_cast< Uint8 * >( surface->pixels );
        width = surface->w;
        height = surface->h;

        texture = smileRasterContext->getDeviceContext()->CreateTexture2D( pixels, width, height );
    }

    SmileRasterTexture2D::~SmileRasterTexture2D()
    {
        SDL_FreeSurface( surface );
    }
}