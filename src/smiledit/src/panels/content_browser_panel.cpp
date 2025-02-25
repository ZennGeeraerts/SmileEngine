/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "content_browser_panel.h"

#include "engine/graphic/renderer/render_engine.h"
#include "engine/core/project/project_manager.h"

#include <imgui/imgui.h>

namespace smile
{
    ContentBrowserPanel::ContentBrowserPanel()
        : m_BaseDirectory{ project::ProjectManager::GetActive()->GetAssetDirectory() },
          m_CurrentDirectory{ m_BaseDirectory }
    {
        graphic::GraphicsDevice *pDevice = graphic::RenderEngine::GetRenderSystem().GetGraphicsDevice();
        m_pDirectoryIcon = pDevice->CreateTexture2D( "resources/icons/content_browser/directory_icon.png" );
        m_pFileIcon = pDevice->CreateTexture2D( "resources/icons/content_browser/file_icon.png" );
    }

    void ContentBrowserPanel::OnImGuiRender()
    {
        ImGui::Begin( "Content Browser" );

        if ( m_CurrentDirectory != m_BaseDirectory )
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
            std::string fileName = path.filename().string();

            ImGui::PushID( fileName.c_str() );
            memory::Ref< graphic::Texture > pIcon = directoryEntry.is_directory() ? m_pDirectoryIcon : m_pFileIcon;
            ImGui::PushStyleColor( ImGuiCol_Button, ImVec4{ 0, 0, 0, 0 } );
            ImGui::ImageButton( pIcon->GetData(), { thumbnailSize, thumbnailSize } );

            if ( ImGui::BeginDragDropSource() )
            {
                auto relativePath = std::filesystem::path{ path };
                const wchar_t *itemPath = relativePath.c_str();
                ImGui::SetDragDropPayload(
                    "ContentBrowserItem", itemPath, ( wcslen( itemPath ) + 1 ) * sizeof( wchar_t ), ImGuiCond_Once );
                ImGui::EndDragDropSource();
            }

            ImGui::PopStyleColor();

            if ( ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked( ImGuiMouseButton_Left ) )
            {
                if ( directoryEntry.is_directory() )
                    m_CurrentDirectory /= path.filename();
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