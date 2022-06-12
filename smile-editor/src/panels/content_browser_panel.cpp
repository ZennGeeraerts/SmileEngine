#include "content_browser_panel.h"

#include <thirdparty/imgui/imgui.h>

namespace smile
{
    extern const std::filesystem::path assetPath = "assets";

    ContentBrowserPanel::ContentBrowserPanel() : currentDirectory{ assetPath }
    {
        directoryIcon = renderer::Texture2D::create( "resources/icons/content_browser/directory_icon.png" );
        fileIcon = renderer::Texture2D::create( "resources/icons/content_browser/file_icon.png" );
    }

    void ContentBrowserPanel::onImGuiRender()
    {
        ImGui::Begin( "Content Browser" );

        if ( currentDirectory != std::filesystem::path{ assetPath } )
        {
            if ( ImGui::Button( "Back" ) )
            {
                currentDirectory = currentDirectory.parent_path();
            }
        }

        static float padding = 16.f;
        static float thumbnail_size = 128.f;
        float cell_size = thumbnail_size + padding;

        const float panel_width = ImGui::GetContentRegionAvail().x;
        int column_count
            = static_cast< int >( panel_width / cell_size );
        column_count = std::max( column_count, 1 );

        ImGui::Columns( column_count, 0, false );

        for ( const auto &directory_entry : std::filesystem::directory_iterator( currentDirectory ) )
        {
            const auto &path = directory_entry.path();
            auto relative_path = std::filesystem::relative( directory_entry.path(), assetPath );
            std::string file_name = relative_path.filename().string();

            ImGui::PushID( file_name.c_str() );
            Ref< renderer::Texture2D > icon = directory_entry.is_directory() ? directoryIcon : fileIcon;
            ImGui::PushStyleColor( ImGuiCol_Button, ImVec4{ 0, 0, 0, 0 } );
            ImGui::ImageButton( icon->getData(), { thumbnail_size, thumbnail_size } );

            if ( ImGui::BeginDragDropSource() )
            {
                const wchar_t *item_path = relative_path.c_str();
                ImGui::SetDragDropPayload(
                    "ContentBrowserItem", item_path, ( wcslen( item_path ) + 1 ) * sizeof( wchar_t ), ImGuiCond_Once );
                ImGui::EndDragDropSource();
            }

            ImGui::PopStyleColor();

            if ( ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked( ImGuiMouseButton_Left ) )
            {
                if ( directory_entry.is_directory() )
                    currentDirectory /= directory_entry.path().filename();
            }

            ImGui::TextWrapped( file_name.c_str() );

            ImGui::NextColumn();

            ImGui::PopID();
        }

        ImGui::Columns( 1 );

        /*ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
        ImGui::SliderFloat("Padding", &padding, 0, 32);*/

        ImGui::End();
    }
}