/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "world_serializer.h"

#include "smile/common/logging/logger.h"
#include "smile/common/primitive/text/string.h"
#include "entity.h"
#include "components.h"
#include "component_serializer_registry.h"
#include "smile/core/project/project_manager.h"
#include "smile/core/yaml/string.h"
#include "smile/core/yaml/math.h"
#include "smile/graphic/sprite/texture_manager.h"

#include <fstream>

namespace smile::world
{
    WorldSerializer::WorldSerializer( memory::Ref< World > pWorld ) : m_pWorld{ pWorld }
    {
    }

    void WorldSerializer::Serialize( const std::filesystem::path &filePath )
    {
        YAML::Emitter output{};
        output << YAML::BeginMap;
        output << YAML::Key << "World" << YAML::Value << "Untitled";
        output << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

        m_pWorld->m_ECSEngine.Each(
            [&]( auto entityID )
            {
                Entity entity{ entityID, m_pWorld.GetPointer() };
                if ( !entity )
                    return;
                SerializeEntity( output, entity );
            } );

        output << YAML::EndSeq;
        output << YAML::EndMap;

        std::ofstream fileOutput{ filePath };
        fileOutput << output.c_str();
    }

    static void SerializeMaterial( YAML::Emitter &output, const Ref< graphic::Material > &pMaterial )
    {
        output << YAML::Key << "Material";
        output << YAML::BeginMap;

        output << YAML::Key << "FloatValues";
        output << YAML::BeginMap;
        const auto &floatValues{ pMaterial->GetFloatValues() };
        for ( auto it{ floatValues.begin() }; it != floatValues.end(); ++it )
        {
            output << YAML::Key << ( *it ).first << YAML::Value << ( *it ).second;
        }
        output << YAML::EndMap;

        output << YAML::Key << "IntValues";
        output << YAML::BeginMap;
        const auto &intValues{ pMaterial->GetIntValues() };
        for ( auto it{ intValues.begin() }; it != intValues.end(); ++it )
        {
            output << YAML::Key << ( *it ).first << YAML::Value << ( *it ).second;
        }
        output << YAML::EndMap;

        output << YAML::Key << "BoolValues";
        output << YAML::BeginMap;
        const auto &boolValues{ pMaterial->GetBoolValues() };
        for ( auto it{ boolValues.begin() }; it != boolValues.end(); ++it )
        {
            output << YAML::Key << ( *it ).first << YAML::Value << ( *it ).second;
        }
        output << YAML::EndMap;

        output << YAML::Key << "Float2Values";
        output << YAML::BeginMap;
        const auto &float2Values{ pMaterial->GetFloat2Values() };
        for ( auto it{ float2Values.begin() }; it != float2Values.end(); ++it )
        {
            output << YAML::Key << ( *it ).first << YAML::Value << ( *it ).second;
        }
        output << YAML::EndMap;

        output << YAML::Key << "Float3Values";
        output << YAML::BeginMap;
        const auto &float3Values{ pMaterial->GetFloat3Values() };
        for ( auto it{ float3Values.begin() }; it != float3Values.end(); ++it )
        {
            output << YAML::Key << ( *it ).first << YAML::Value << ( *it ).second;
        }
        output << YAML::EndMap;

        output << YAML::Key << "Texture2DValues";
        output << YAML::BeginMap;
        const auto &texture2DValues{ pMaterial->GetTexture2DValues() };
        for ( const auto &[semantic, pTexture] : texture2DValues )
        {
            if ( pTexture )
            {
                memory::Ref< graphic::TextureAsset > pTextureAsset =
                    graphic::TextureManager::GetInstance().GetTexture( pTexture );

                output << YAML::Key << semantic << YAML::Value << pTextureAsset->m_Handle;
            }
            else
            {
                output << YAML::Key << semantic << YAML::Value << 0;
            }
        }
        output << YAML::EndMap;

        output << YAML::EndMap;
    }

    static void SerializeEntity( YAML::Emitter &output, Entity entity )
    {
        SM_ASSERT_MSG( entity.HasComponent< ecs::IDComponent >(),
            "WorldSerializer::SerializeWorld > Entity does not have an IDComponent" );

        output << YAML::BeginMap;
        output << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

        if ( entity.HasComponent< ecs::TagComponent >() )
        {
            output << YAML::Key << "TagComponent";
            output << YAML::BeginMap;

            auto &tag = entity.GetComponent< ecs::TagComponent >().Tag;
            output << YAML::Key << "Tag" << YAML::Value << tag;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< ecs::TransformComponent >() )
        {
            output << YAML::Key << "TransformComponent";
            output << YAML::BeginMap;

            auto &transformComponent = entity.GetComponent< ecs::TransformComponent >();
            output << YAML::Key << "Translation" << YAML::Value << transformComponent.Translation;
            output << YAML::Key << "Rotation" << YAML::Value << transformComponent.Rotation;
            output << YAML::Key << "Scale" << YAML::Value << transformComponent.Scale;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< graphic::ecs::CameraComponent >() )
        {
            output << YAML::Key << "CameraComponent";
            output << YAML::BeginMap;

            auto &cameraComponent = entity.GetComponent< graphic::ecs::CameraComponent >();
            auto &camera = cameraComponent.Camera;
            output << YAML::Key << "Camera" << YAML::Value;
            output << YAML::BeginMap;

            output << YAML::Key << "ProjectionType" << YAML::Value
                   << static_cast< Uint32 >( camera.GetProjectionType() );
            output << YAML::Key << "FOV" << YAML::Value << camera.GetFOV();
            output << YAML::Key << "PerspectiveNearPlane" << YAML::Value << camera.GetPerspectiveNearPlane();
            output << YAML::Key << "PerspectiveFarPlane" << YAML::Value << camera.GetPerspectiveFarPlane();
            output << YAML::Key << "Size" << YAML::Value << camera.GetSize();
            output << YAML::Key << "OrthographicNearPlane" << camera.GetOrthographicNearPlane();
            output << YAML::Key << "OrthographicFarPlane" << camera.GetOrthographicFarPlane();

            output << YAML::EndMap;

            output << YAML::Key << "bPrimary" << YAML::Value << cameraComponent.IsPrimary;
            output << YAML::Key << "bFixedAspectRatio" << YAML::Value << cameraComponent.HasFixedAspectRatio;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< scripting::ecs::ScriptComponent >() )
        {
            output << YAML::Key << "ScriptComponent";
            output << YAML::BeginMap;

            auto &scriptComponent = entity.GetComponent< scripting::ecs::ScriptComponent >();
            output << YAML::Key << "ClassName" << YAML::Value << scriptComponent.ClassName;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< graphic::ecs::MeshRendererComponent >() )
        {
            output << YAML::Key << "MeshRendererComponent";
            output << YAML::BeginMap;

            auto &meshRendererComponent = entity.GetComponent< graphic::ecs::MeshRendererComponent >();

            if ( meshRendererComponent.pModel )
            {
                auto basePath = project::ProjectManager::GetActive()->GetAssetDirectory();
                auto fullPath = std::filesystem::path{ meshRendererComponent.pModel->GetFilePath() };
                auto relativePath = fullPath.lexically_relative( basePath );

                output << YAML::Key << "Model" << YAML::Value << relativePath.string();
            }
            else
            {
                output << YAML::Key << "Model" << YAML::Value << "";
            }

            output << YAML::Key << "MeshIndex" << meshRendererComponent.MeshIndex;

            SerializeMaterial( output, meshRendererComponent.pMaterial );

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< graphic::ecs::SkinnedMeshRendererComponent >() )
        {
            output << YAML::Key << "SkinnedMeshRendererComponent";
            output << YAML::BeginMap;

            auto &skinnedMeshRendererComponent = entity.GetComponent< graphic::ecs::SkinnedMeshRendererComponent >();
            if ( skinnedMeshRendererComponent.pModel )
            {
                auto basePath = project::ProjectManager::GetActive()->GetAssetDirectory();
                auto fullPath = std::filesystem::path{ skinnedMeshRendererComponent.pModel->GetFilePath() };
                auto relativePath = fullPath.lexically_relative( basePath );

                output << YAML::Key << "Model" << YAML::Value << relativePath.string();
            }
            else
            {
                output << YAML::Key << "Model" << YAML::Value << "";
            }

            output << YAML::Key << "MeshIndex" << skinnedMeshRendererComponent.MeshIndex;

            SerializeMaterial( output, skinnedMeshRendererComponent.pMaterial );

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< graphic::ecs::AnimatorComponent >() )
        {
            output << YAML::Key << "AnimatorComponent";
            output << YAML::BeginMap;

            auto &animatorComponent = entity.GetComponent< graphic::ecs::AnimatorComponent >();
            if ( animatorComponent.pModel )
            {
                auto basePath = project::ProjectManager::GetActive()->GetAssetDirectory();
                auto fullPath = std::filesystem::path{ animatorComponent.pModel->GetFilePath() };
                auto relativePath = fullPath.lexically_relative( basePath );

                output << YAML::Key << "Model" << YAML::Value << relativePath.string();
            }
            else
            {
                output << YAML::Key << "Model" << YAML::Value << "";
            }

            output << YAML::Key << "ClipIndex" << animatorComponent.CurrentClipIndex;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< graphic::ecs::SpriteRendererComponent >() )
        {
            output << YAML::Key << "SpriteRendererComponent";
            output << YAML::BeginMap;

            auto &spriteRendererComponent = entity.GetComponent< graphic::ecs::SpriteRendererComponent >();
            output << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;

            if ( spriteRendererComponent.pTexture )
            {
                memory::Ref< graphic::TextureAsset > pTextureAsset =
                    graphic::TextureManager::GetInstance().GetTexture( spriteRendererComponent.pTexture );

                output << YAML::Key << "Texture" << YAML::Value << pTextureAsset->m_Handle;
            }
            else
            {
                output << YAML::Key << "Texture" << YAML::Value << "";
            }
        }

        const auto &componentSerializers = ComponentSerializerRegistry::GetInstance().GetComponentSerializers();
        for ( const auto &serializer : componentSerializers )
        {
            serializer.Value->TrySerialize( output, entity );
        }

        output << YAML::EndMap;
    }

    void WorldSerializer::SerializeRuntime( const std::filesystem::path &filePath )
    {
        SM_ASSERT_MSG( false, "WorldSerializer::SerializeRuntime > Not implemented" );
    }

    bool WorldSerializer::Deserialize( const std::filesystem::path &filePath )
    {
        YAML::Node data;
        try
        {
            data = YAML::LoadFile( filePath.string() );
        }
        catch ( YAML::ParserException e )
        {
            SM_LOG_ERROR( "Failed to load .smile file: {0}\n{1}", filePath.string(), e.what() );
            return false;
        }

        if ( !data["World"] )
            return false;

        primitive::String worldName = data["World"].as< primitive::String >();
        SM_LOG_TRACE( "Deserializing world '{}'", worldName );

        auto entities = data["Entities"];
        if ( entities )
        {
            for ( auto entity : entities )
            {
                Uint64 uuid = entity["Entity"].as< Uint64 >();

                primitive::String name{};
                auto tagComponent = entity["TagComponent"];
                if ( tagComponent )
                    name = tagComponent["Tag"].as< primitive::String >();

                SM_LOG_TRACE( "Deserialized entity with ID: {0}, name: {1}", uuid, name );

                Entity deserializedEntity = m_pWorld->CreateEntity( uuid, name );

                auto transformComponent = entity["TransformComponent"];
                if ( transformComponent )
                {
                    auto &tc = deserializedEntity.GetComponent< ecs::TransformComponent >();
                    tc.Translation = transformComponent["Translation"].as< DirectX::XMFLOAT3 >();
                    tc.Rotation = transformComponent["Rotation"].as< DirectX::XMFLOAT3 >();
                    tc.Scale = transformComponent["Scale"].as< DirectX::XMFLOAT3 >();
                }

                auto cameraComponent = entity["CameraComponent"];
                if ( cameraComponent )
                {
                    auto &cc = deserializedEntity.AddComponent< graphic::ecs::CameraComponent >();

                    auto cameraProps = cameraComponent["Camera"];
                    cc.Camera.SetProjectionType( static_cast< graphic::SceneCamera::ProjectionType >(
                        cameraProps["ProjectionType"].as< int >() ) );

                    cc.Camera.SetFOV( cameraProps["FOV"].as< float >() );
                    cc.Camera.SetPerspectiveNearPlane( cameraProps["PerspectiveNearPlane"].as< float >() );
                    cc.Camera.SetPerspectiveFarPlane( cameraProps["PerspectiveFarPlane"].as< float >() );

                    cc.Camera.SetSize( cameraProps["Size"].as< float >() );
                    cc.Camera.SetOrthographicNearPlane( cameraProps["OrthographicNearPlane"].as< float >() );
                    cc.Camera.SetOrthographicFarPlane( cameraProps["OrthographicFarPlane"].as< float >() );

                    cc.IsPrimary = cameraComponent["bPrimary"].as< bool >();
                    cc.HasFixedAspectRatio = cameraComponent["bFixedAspectRatio"].as< bool >();
                }

                auto scriptComponent = entity["ScriptComponent"];
                if ( scriptComponent )
                {
                    auto &sc = deserializedEntity.AddComponent< scripting::ecs::ScriptComponent >();
                    sc.ClassName = scriptComponent["ClassName"].as< std::string >();
                }

                auto meshRendererComponent = entity["MeshRendererComponent"];
                if ( meshRendererComponent )
                {
                    auto &mrc = deserializedEntity.AddComponent< graphic::ecs::MeshRendererComponent >();

                    mrc.MeshIndex = meshRendererComponent["MeshIndex"].as< Uint32 >();

                    auto modelPath = meshRendererComponent["Model"].as< std::string >();
                    if ( !modelPath.empty() )
                    {
                        auto path = project::ProjectManager::GetAssetFileSystemPath( modelPath );
                        mrc.pModel = graphic::ModelLoader::LoadModel( path.string() );

                        auto pMeshFilter = mrc.pModel->GetMeshFilter( mrc.MeshIndex );
                        mrc.pMesh = graphic::MeshFactory::CreateMesh( pMeshFilter, mrc.pMaterial->GetBufferLayout() );
                    }

                    auto material = meshRendererComponent["Material"];

                    auto floatValues = material["FloatValues"];
                    for ( auto it{ floatValues.begin() }; it != floatValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< float >();
                        mrc.pMaterial->SetFloatValue( semantic, value );
                    }

                    auto intValues = material["IntValues"];
                    for ( auto it{ intValues.begin() }; it != intValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< int >();
                        mrc.pMaterial->SetIntValue( semantic, value );
                    }

                    auto boolValues = material["BoolValues"];
                    for ( auto it{ boolValues.begin() }; it != boolValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< bool >();
                        mrc.pMaterial->SetBoolValue( semantic, value );
                    }

                    auto float2Values = material["Float2Values"];
                    for ( auto it{ float2Values.begin() }; it != float2Values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< DirectX::XMFLOAT2 >();
                        mrc.pMaterial->SetFloat2Value( semantic, value );
                    }

                    auto float3Values = material["Float3Values"];
                    for ( auto it{ float3Values.begin() }; it != float3Values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< DirectX::XMFLOAT3 >();
                        mrc.pMaterial->SetFloat3Value( semantic, value );
                    }

                    auto texture2DValues = material["Texture2DValues"];
                    for ( auto it{ texture2DValues.begin() }; it != texture2DValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        asset::AssetHandle assetHandle = ( *it ).second.as< Uint64 >();

                        memory::Ref< graphic::TextureAsset > pTextureAsset =
                            graphic::TextureManager::GetInstance().GetTexture( assetHandle );
                        if ( pTextureAsset )
                        {
                            mrc.pMaterial->SetTexture2D( semantic, pTextureAsset->GetTexture() );
                        }
                        else
                        {
                            mrc.pMaterial->SetTexture2D( semantic, nullptr );
                        }
                    }
                }

                auto skinnedMeshRendererComponent = entity["SkinnedMeshRendererComponent"];
                if ( skinnedMeshRendererComponent )
                {
                    auto &smrc = deserializedEntity.AddComponent< graphic::ecs::SkinnedMeshRendererComponent >();

                    smrc.MeshIndex = skinnedMeshRendererComponent["MeshIndex"].as< Uint32 >();

                    auto modelPath = skinnedMeshRendererComponent["Model"].as< std::string >();
                    if ( !modelPath.empty() )
                    {
                        auto path = project::ProjectManager::GetAssetFileSystemPath( modelPath );
                        smrc.pModel = graphic::ModelLoader::LoadModel( path );

                        auto pSkinnedMeshFilter = smrc.pModel->GetSkinnedMeshFilter( smrc.MeshIndex );
                        smrc.pSkinnedMesh = graphic::MeshFactory::CreateSkinnedMesh(
                            pSkinnedMeshFilter, smrc.pMaterial->GetBufferLayout() );
                    }

                    auto material = skinnedMeshRendererComponent["Material"];

                    auto floatValues = material["FloatValues"];
                    for ( auto it{ floatValues.begin() }; it != floatValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< float >();
                        smrc.pMaterial->SetFloatValue( semantic, value );
                    }

                    auto intValues = material["IntValues"];
                    for ( auto it{ intValues.begin() }; it != intValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< int >();
                        smrc.pMaterial->SetIntValue( semantic, value );
                    }

                    auto boolValues = material["BoolValues"];
                    for ( auto it{ boolValues.begin() }; it != boolValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< bool >();
                        smrc.pMaterial->SetBoolValue( semantic, value );
                    }

                    auto float2Values = material["Float2Values"];
                    for ( auto it{ float2Values.begin() }; it != float2Values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< DirectX::XMFLOAT2 >();
                        smrc.pMaterial->SetFloat2Value( semantic, value );
                    }

                    auto float3Values = material["Float3Values"];
                    for ( auto it{ float3Values.begin() }; it != float3Values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< DirectX::XMFLOAT3 >();
                        smrc.pMaterial->SetFloat3Value( semantic, value );
                    }

                    auto texture2DValues = material["Texture2DValues"];
                    for ( auto it{ texture2DValues.begin() }; it != texture2DValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        asset::AssetHandle assetHandle = ( *it ).second.as< Uint64 >();

                        memory::Ref< graphic::TextureAsset > pTextureAsset =
                            graphic::TextureManager::GetInstance().GetTexture( assetHandle );
                        if ( pTextureAsset )
                        {
                            smrc.pMaterial->SetTexture2D( semantic, pTextureAsset->GetTexture() );
                        }
                        else
                        {
                            smrc.pMaterial->SetTexture2D( semantic, nullptr );
                        }
                    }
                }

                auto animatorComponent = entity["AnimatorComponent"];
                if ( animatorComponent )
                {
                    auto &ac = deserializedEntity.AddComponent< graphic::ecs::AnimatorComponent >();

                    ac.CurrentClipIndex = animatorComponent["ClipIndex"].as< Uint32 >();

                    auto modelPath = animatorComponent["Model"].as< std::string >();
                    if ( !modelPath.empty() )
                    {
                        auto path = project::ProjectManager::GetAssetFileSystemPath( modelPath );
                        ac.pModel = graphic::ModelLoader::LoadModel( path );
                        ac.pAnimationClips = ac.pModel->GetAnimationClips();
                    }
                }

                auto spriteRendererComponent = entity["SpriteRendererComponent"];
                if ( spriteRendererComponent )
                {
                    auto &src = deserializedEntity.AddComponent< graphic::ecs::SpriteRendererComponent >();

                    src.Color = spriteRendererComponent["Color"].as< DirectX::XMFLOAT4 >();

                    asset::AssetHandle textureAssetHandle = spriteRendererComponent["Texture"].as< Uint64 >();

                    memory::Ref< graphic::TextureAsset > pTextureAsset =
                        graphic::TextureManager::GetInstance().GetTexture( textureAssetHandle );
                    if ( pTextureAsset )
                    {
                        src.pTexture = pTextureAsset->GetTexture();
                    }
                    else
                    {
                        src.pTexture = nullptr;
                    }
                }

                const auto &componentSerializers = ComponentSerializerRegistry::GetInstance().GetComponentSerializers();
                for ( const auto &serializer : componentSerializers )
                {
                    serializer.Value->TryDeserialize( entity, deserializedEntity );
                }
            }
        }

        return true;
    }

    bool WorldSerializer::DeserializeRuntime( const std::filesystem::path &filePath )
    {
        SM_ASSERT_MSG( false, "WorldSerializer::DeserializeRuntime > Not implemented" );
        return false;
    }
}