/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "content_browser_panel.h"

#include "smile/graphic/renderer/render_engine.h"
#include "smile/graphic/renderer/resource/resource_manager.h"
#include "smile/core/project/project_manager.h"

#include <imgui/imgui.h>

namespace smile
{
    ContentBrowserPanel::ContentBrowserPanel( const Ref< project::Project > &pProject )
        : m_pProject{ pProject },
          m_BaseDirectory{ project::ProjectManager::GetActive()->GetAssetDirectory() },
          m_CurrentDirectory{ m_BaseDirectory }
    {
        m_TreeNodes.emplace_back( TreeNode{ ".", 0 } );

        graphic::ResourceManager &resourceManager = graphic::RenderEngine::GetRenderSystem().GetResourceManager();
        m_pDirectoryIcon = resourceManager.CreateTexture2D( "resources/icons/content_browser/directory_icon.png" );
        m_pFileIcon = resourceManager.CreateTexture2D( "resources/icons/content_browser/file_icon.png" );

        RefreshAssetTree();

        m_Mode = Mode::FileSystem;
    }

    void ContentBrowserPanel::OnImGuiRender()
    {
        ImGui::Begin( "Content Browser" );

        const char *label = m_Mode == Mode::Asset ? "Asset" : "File";
        if ( ImGui::Button( label ) )
        {
            m_Mode = m_Mode == Mode::Asset ? Mode::FileSystem : Mode::Asset;
        }

        if ( m_CurrentDirectory != m_BaseDirectory )
        {
            ImGui::SameLine();
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

        if ( m_Mode == Mode::Asset )
        {
            TreeNode *pNode = &m_TreeNodes[0];

            const auto currentDir = std::filesystem::relative( m_CurrentDirectory, m_pProject->GetAssetDirectory() );
            for ( const auto &path : currentDir )
            {
                // if only one level
                if ( pNode->Path == currentDir )
                    break;

                if ( pNode->Children.find( path ) != pNode->Children.end() )
                {
                    pNode = &m_TreeNodes[pNode->Children[path]];
                    continue;
                }
                else
                {
                    SM_ASSERT( false, "Cannot find path" );
                }
            }

            for ( const auto &[item, treeNodeIndex] : pNode->Children )
            {
                bool isDirectory = std::filesystem::is_directory( m_pProject->GetAssetDirectory() / item );

                std::string itemStr = item.generic_string();

                ImGui::PushID( itemStr.c_str() );
                memory::Ref< graphic::Texture > pIcon = isDirectory ? m_pDirectoryIcon : m_pFileIcon;
                ImGui::PushStyleColor( ImGuiCol_Button, ImVec4{ 0, 0, 0, 0 } );
                ImGui::ImageButton( static_cast< ImTextureID >( pIcon->GetData() ), { thumbnailSize, thumbnailSize } );

                if ( ImGui::BeginPopupContextItem() )
                {
                    if ( ImGui::MenuItem( "Delete" ) )
                    {
                        // TODO
                        SM_ASSERT( false, "Not implemented" );
                    }
                    ImGui::EndPopup();
                }

                if ( ImGui::BeginDragDropSource() )
                {
                    asset::AssetHandle handle = m_TreeNodes[treeNodeIndex].Handle;
                    ImGui::SetDragDropPayload( "ContentBrowserItem", &handle, sizeof( asset::AssetHandle ) );
                    ImGui::EndDragDropSource();
                }

                ImGui::PopStyleColor();
                if ( ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked( ImGuiMouseButton_Left ) )
                {
                    if ( isDirectory )
                        m_CurrentDirectory /= item.filename();
                }

                ImGui::TextWrapped( itemStr.c_str() );

                ImGui::NextColumn();

                ImGui::PopID();
            }
        }
        else
        {
            for ( const auto &directoryEntry : std::filesystem::directory_iterator( m_CurrentDirectory ) )
            {
                const auto &path = directoryEntry.path();
                std::string fileName = path.filename().string();

                ImGui::PushID( fileName.c_str() );
                memory::Ref< graphic::Texture > pIcon = directoryEntry.is_directory() ? m_pDirectoryIcon : m_pFileIcon;
                ImGui::PushStyleColor( ImGuiCol_Button, ImVec4{ 0, 0, 0, 0 } );
                ImGui::ImageButton( pIcon->GetData(), { thumbnailSize, thumbnailSize } );

                auto relativePath = std::filesystem::relative( path, m_pProject->GetAssetDirectory() );

                if ( ImGui::BeginPopupContextItem() )
                {
                    if ( ImGui::MenuItem( "Import" ) )
                    {
                        m_pProject->GetEditorAssetManager()->ImportAsset( relativePath );
                        RefreshAssetTree();
                    }
                    ImGui::EndPopup();
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
        }

        ImGui::Columns( 1 );

        /*ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
        ImGui::SliderFloat("Padding", &padding, 0, 32);*/

        ImGui::End();
    }

    void ContentBrowserPanel::RefreshAssetTree()
    {
        const asset::AssetRegistry &assetRegistry =
            project::ProjectManager::GetActive()->GetEditorAssetManager()->GetAssetRegistry();

        for ( const auto &[handle, metadata] : assetRegistry )
        {
            Uint32 currentNodeIndex = 0;

            for ( const auto &path : metadata.FilePath )
            {
                auto it = m_TreeNodes[currentNodeIndex].Children.find( path.generic_string() );
                if ( it != m_TreeNodes[currentNodeIndex].Children.end() )
                {
                    currentNodeIndex = it->second;
                }
                else
                {
                    // Add node
                    TreeNode newNode{ path, handle };
                    newNode.Parent = currentNodeIndex;
                    m_TreeNodes.emplace_back( std::move( newNode ) );

                    m_TreeNodes[currentNodeIndex].Children[path] = m_TreeNodes.size() - 1;
                    currentNodeIndex = m_TreeNodes.size() - 1;
                }
            }
        }
    }
}