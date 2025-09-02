/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "serializer.h"

#include "smile/core/project/project_manager.h"
#include "smile/core/yaml/math.h"

#include "smile/graphic/scene/ecs/camera_component.h"
#include "smile/graphic/scene/ecs/mesh_renderer_component.h"
#include "smile/graphic/scene/ecs/skinned_mesh_renderer_component.h"
#include "smile/graphic/animation/ecs/animator_component.h"
#include "smile/graphic/sprite/ecs/sprite_renderer_component.h"
#include "smile/graphic/sprite/texture_manager.h"

namespace smile::graphic::serializer
{
    static void SerializeMaterial( yaml::Emitter &output, const Ref< Material > &pMaterial )
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
                memory::Ref< TextureAsset > pTextureAsset = TextureManager::GetInstance().GetTexture( pTexture );

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

    void Serialize( yaml::Emitter &output, world::Entity entity )
    {
        if ( entity.HasComponent< ecs::CameraComponent >() )
        {
            output << YAML::Key << "CameraComponent";
            output << YAML::BeginMap;

            auto &cameraComponent = entity.GetComponent< ecs::CameraComponent >();
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

        if ( entity.HasComponent< ecs::MeshRendererComponent >() )
        {
            output << YAML::Key << "MeshRendererComponent";
            output << YAML::BeginMap;

            auto &meshRendererComponent = entity.GetComponent< ecs::MeshRendererComponent >();

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

        if ( entity.HasComponent< ecs::SkinnedMeshRendererComponent >() )
        {
            output << YAML::Key << "SkinnedMeshRendererComponent";
            output << YAML::BeginMap;

            auto &skinnedMeshRendererComponent = entity.GetComponent< ecs::SkinnedMeshRendererComponent >();
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

        if ( entity.HasComponent< ecs::AnimatorComponent >() )
        {
            output << YAML::Key << "AnimatorComponent";
            output << YAML::BeginMap;

            auto &animatorComponent = entity.GetComponent< ecs::AnimatorComponent >();
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

        if ( entity.HasComponent< ecs::SpriteRendererComponent >() )
        {
            output << YAML::Key << "SpriteRendererComponent";
            output << YAML::BeginMap;

            auto &spriteRendererComponent = entity.GetComponent< ecs::SpriteRendererComponent >();
            output << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;

            if ( spriteRendererComponent.pTexture )
            {
                memory::Ref< TextureAsset > pTextureAsset =
                    TextureManager::GetInstance().GetTexture( spriteRendererComponent.pTexture );

                output << YAML::Key << "Texture" << YAML::Value << pTextureAsset->m_Handle;
            }
            else
            {
                output << YAML::Key << "Texture" << YAML::Value << "";
            }
        }
    }

    void Deserialize( const yaml::Node &data, world::Entity entity )
    {
        auto cameraData = data["CameraComponent"];
        if ( cameraData )
        {
            auto &cameraComponent = entity.AddComponent< ecs::CameraComponent >();

            auto cameraProps = cameraData["Camera"];
            cameraComponent.Camera.SetProjectionType(
                static_cast< SceneCamera::ProjectionType >( cameraProps["ProjectionType"].as< int >() ) );

            cameraComponent.Camera.SetFOV( cameraProps["FOV"].as< float >() );
            cameraComponent.Camera.SetPerspectiveNearPlane( cameraProps["PerspectiveNearPlane"].as< float >() );
            cameraComponent.Camera.SetPerspectiveFarPlane( cameraProps["PerspectiveFarPlane"].as< float >() );

            cameraComponent.Camera.SetSize( cameraProps["Size"].as< float >() );
            cameraComponent.Camera.SetOrthographicNearPlane( cameraProps["OrthographicNearPlane"].as< float >() );
            cameraComponent.Camera.SetOrthographicFarPlane( cameraProps["OrthographicFarPlane"].as< float >() );

            cameraComponent.IsPrimary = cameraData["bPrimary"].as< bool >();
            cameraComponent.HasFixedAspectRatio = cameraData["bFixedAspectRatio"].as< bool >();
        }

        auto meshRendererData = data["MeshRendererComponent"];
        if ( meshRendererData )
        {
            auto &meshRendererComponent = entity.AddComponent< ecs::MeshRendererComponent >();

            meshRendererComponent.MeshIndex = meshRendererData["MeshIndex"].as< Uint32 >();

            auto modelPath = meshRendererData["Model"].as< std::string >();
            if ( !modelPath.empty() )
            {
                auto path = project::ProjectManager::GetAssetFileSystemPath( modelPath );
                meshRendererComponent.pModel = ModelLoader::LoadModel( path.string() );

                auto pMeshFilter = meshRendererComponent.pModel->GetMeshFilter( meshRendererComponent.MeshIndex );
                meshRendererComponent.pMesh =
                    MeshFactory::CreateMesh( pMeshFilter, meshRendererComponent.pMaterial->GetBufferLayout() );
            }

            auto material = meshRendererData["Material"];

            auto floatValues = material["FloatValues"];
            for ( auto it{ floatValues.begin() }; it != floatValues.end(); ++it )
            {
                std::string semantic = ( *it ).first.as< std::string >();
                auto value = ( *it ).second.as< float >();
                meshRendererComponent.pMaterial->SetFloatValue( semantic, value );
            }

            auto intValues = material["IntValues"];
            for ( auto it{ intValues.begin() }; it != intValues.end(); ++it )
            {
                std::string semantic = ( *it ).first.as< std::string >();
                auto value = ( *it ).second.as< int >();
                meshRendererComponent.pMaterial->SetIntValue( semantic, value );
            }

            auto boolValues = material["BoolValues"];
            for ( auto it{ boolValues.begin() }; it != boolValues.end(); ++it )
            {
                std::string semantic = ( *it ).first.as< std::string >();
                auto value = ( *it ).second.as< bool >();
                meshRendererComponent.pMaterial->SetBoolValue( semantic, value );
            }

            auto float2Values = material["Float2Values"];
            for ( auto it{ float2Values.begin() }; it != float2Values.end(); ++it )
            {
                std::string semantic = ( *it ).first.as< std::string >();
                auto value = ( *it ).second.as< DirectX::XMFLOAT2 >();
                meshRendererComponent.pMaterial->SetFloat2Value( semantic, value );
            }

            auto float3Values = material["Float3Values"];
            for ( auto it{ float3Values.begin() }; it != float3Values.end(); ++it )
            {
                std::string semantic = ( *it ).first.as< std::string >();
                auto value = ( *it ).second.as< DirectX::XMFLOAT3 >();
                meshRendererComponent.pMaterial->SetFloat3Value( semantic, value );
            }

            auto texture2DValues = material["Texture2DValues"];
            for ( auto it{ texture2DValues.begin() }; it != texture2DValues.end(); ++it )
            {
                std::string semantic = ( *it ).first.as< std::string >();
                asset::AssetHandle assetHandle = ( *it ).second.as< Uint64 >();

                memory::Ref< TextureAsset > pTextureAsset = TextureManager::GetInstance().GetTexture( assetHandle );
                if ( pTextureAsset )
                {
                    meshRendererComponent.pMaterial->SetTexture2D( semantic, pTextureAsset->GetTexture() );
                }
                else
                {
                    meshRendererComponent.pMaterial->SetTexture2D( semantic, nullptr );
                }
            }
        }

        auto skinnedMeshRendererData = data["SkinnedMeshRendererComponent"];
        if ( skinnedMeshRendererData )
        {
            auto &skinnedMeshRendererComponent = entity.AddComponent< ecs::SkinnedMeshRendererComponent >();

            skinnedMeshRendererComponent.MeshIndex = skinnedMeshRendererData["MeshIndex"].as< Uint32 >();

            auto modelPath = skinnedMeshRendererData["Model"].as< std::string >();
            if ( !modelPath.empty() )
            {
                auto path = project::ProjectManager::GetAssetFileSystemPath( modelPath );
                skinnedMeshRendererComponent.pModel = ModelLoader::LoadModel( path );

                auto pSkinnedMeshFilter =
                    skinnedMeshRendererComponent.pModel->GetSkinnedMeshFilter( skinnedMeshRendererComponent.MeshIndex );
                skinnedMeshRendererComponent.pSkinnedMesh = MeshFactory::CreateSkinnedMesh(
                    pSkinnedMeshFilter, skinnedMeshRendererComponent.pMaterial->GetBufferLayout() );
            }

            auto material = skinnedMeshRendererData["Material"];

            auto floatValues = material["FloatValues"];
            for ( auto it{ floatValues.begin() }; it != floatValues.end(); ++it )
            {
                std::string semantic = ( *it ).first.as< std::string >();
                auto value = ( *it ).second.as< float >();
                skinnedMeshRendererComponent.pMaterial->SetFloatValue( semantic, value );
            }

            auto intValues = material["IntValues"];
            for ( auto it{ intValues.begin() }; it != intValues.end(); ++it )
            {
                std::string semantic = ( *it ).first.as< std::string >();
                auto value = ( *it ).second.as< int >();
                skinnedMeshRendererComponent.pMaterial->SetIntValue( semantic, value );
            }

            auto boolValues = material["BoolValues"];
            for ( auto it{ boolValues.begin() }; it != boolValues.end(); ++it )
            {
                std::string semantic = ( *it ).first.as< std::string >();
                auto value = ( *it ).second.as< bool >();
                skinnedMeshRendererComponent.pMaterial->SetBoolValue( semantic, value );
            }

            auto float2Values = material["Float2Values"];
            for ( auto it{ float2Values.begin() }; it != float2Values.end(); ++it )
            {
                std::string semantic = ( *it ).first.as< std::string >();
                auto value = ( *it ).second.as< DirectX::XMFLOAT2 >();
                skinnedMeshRendererComponent.pMaterial->SetFloat2Value( semantic, value );
            }

            auto float3Values = material["Float3Values"];
            for ( auto it{ float3Values.begin() }; it != float3Values.end(); ++it )
            {
                std::string semantic = ( *it ).first.as< std::string >();
                auto value = ( *it ).second.as< DirectX::XMFLOAT3 >();
                skinnedMeshRendererComponent.pMaterial->SetFloat3Value( semantic, value );
            }

            auto texture2DValues = material["Texture2DValues"];
            for ( auto it{ texture2DValues.begin() }; it != texture2DValues.end(); ++it )
            {
                std::string semantic = ( *it ).first.as< std::string >();
                asset::AssetHandle assetHandle = ( *it ).second.as< Uint64 >();

                memory::Ref< TextureAsset > pTextureAsset = TextureManager::GetInstance().GetTexture( assetHandle );
                if ( pTextureAsset )
                {
                    skinnedMeshRendererComponent.pMaterial->SetTexture2D( semantic, pTextureAsset->GetTexture() );
                }
                else
                {
                    skinnedMeshRendererComponent.pMaterial->SetTexture2D( semantic, nullptr );
                }
            }
        }

        auto animatorData = data["AnimatorComponent"];
        if ( animatorData )
        {
            auto &animatorComponent = entity.AddComponent< ecs::AnimatorComponent >();

            animatorComponent.CurrentClipIndex = animatorData["ClipIndex"].as< Uint32 >();

            auto modelPath = animatorData["Model"].as< std::string >();
            if ( !modelPath.empty() )
            {
                auto path = project::ProjectManager::GetAssetFileSystemPath( modelPath );
                animatorComponent.pModel = ModelLoader::LoadModel( path );
                animatorComponent.pAnimationClips = animatorComponent.pModel->GetAnimationClips();
            }
        }

        auto spriteRendererData = data["SpriteRendererComponent"];
        if ( spriteRendererData )
        {
            auto &spriteRendererComponent = entity.AddComponent< ecs::SpriteRendererComponent >();

            spriteRendererComponent.Color = spriteRendererData["Color"].as< DirectX::XMFLOAT4 >();

            asset::AssetHandle textureAssetHandle = spriteRendererData["Texture"].as< Uint64 >();

            memory::Ref< TextureAsset > pTextureAsset = TextureManager::GetInstance().GetTexture( textureAssetHandle );
            if ( pTextureAsset )
            {
                spriteRendererComponent.pTexture = pTextureAsset->GetTexture();
            }
            else
            {
                spriteRendererComponent.pTexture = nullptr;
            }
        }
    }
}