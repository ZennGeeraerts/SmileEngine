#include "content_browser_panel.h"

#include "smile_engine/graphic/graphics_device.h"

#include <thirdparty/imgui/imgui.h>

namespace smile
{
    extern const std::filesystem::path g_AssetPath = "assets";

    ContentBrowserPanel::ContentBrowserPanel() : m_CurrentDirectory{ g_AssetPath }
    {
        graphic::GraphicsDevice *pDevice = graphic::GraphicsDevice::GetInstance();
        m_pDirectoryIcon = pDevice->CreateTexture2D( "resources/icons/content_browser/directory_icon.png" );
        m_pFileIcon = pDevice->CreateTexture2D( "resources/icons/content_browser/file_icon.png" );
    }

    void ContentBrowserPanel::OnImGuiRender()
    {
        ImGui::Begin( "Content Browser" );

        if ( m_CurrentDirectory != std::filesystem::path{ g_AssetPath } )
        {
            if ( ImGui::Button( "Back" ) )
            {
                m_CurrentDirectory = m_CurrentDirectory.parent_path();
            }
        }

        static float padding = 16.f;
        static float thumbnailSize = 128.f;
        float cellSize = thumbnailSize + padding;

        const float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = static_cast< int >( panelWidth / cellSize );
        columnCount = std::max( columnCount, 1 );

        ImGui::Columns( columnCount, 0, false );

        for ( const auto &directoryEntry : std::filesystem::directory_iterator( m_CurrentDirectory ) )
        {
            const auto &path = directoryEntry.path();
            auto relativePath = std::filesystem::relative( directoryEntry.path(), g_AssetPath );
            std::string fileName = relativePath.filename().string();

            ImGui::PushID( fileName.c_str() );
            Ref< graphic::Texture2D > icon = directoryEntry.is_directory() ? m_pDirectoryIcon : m_pFileIcon;
            ImGui::PushStyleColor( ImGuiCol_Button, ImVec4{ 0, 0, 0, 0 } );
            ImGui::ImageButton( icon->GetData(), { thumbnailSize, thumbnailSize } );

            if ( ImGui::BeginDragDropSource() )
            {
                const wchar_t *itemPath = relativePath.c_str();
                ImGui::SetDragDropPayload(
                    "ContentBrowserItem", itemPath, ( wcslen( itemPath ) + 1 ) * sizeof( wchar_t ), ImGuiCond_Once );
                ImGui::EndDragDropSource();
            }

            ImGui::PopStyleColor();

            if ( ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked( ImGuiMouseButton_Left ) )
            {
                if ( directoryEntry.is_directory() )
                    m_CurrentDirectory /= directoryEntry.path().filename();
            }

            ImGui::TextWrapped( fileName.c_str() );

            ImGui::NextColumn();

            ImGui::PopID();
        }

        ImGui::Columns( 1 );

        /*ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
        ImGui::SliderFloat("Padding", &padding, 0, 32);*/

        ImGui::End();
    }
}