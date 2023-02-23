#pragma once

namespace Components
{
    class IMaterialTechniqueSet : public AssetHandler::AssetInterface
    {
    public:
        IMaterialTechniqueSet();
        ~IMaterialTechniqueSet();

        static const std::unordered_map<Game::IW4::MaterialTechniqueType, Game::IW3::MaterialTechniqueType> techniqueTypeTableFromIW4;
        static const std::unordered_map<Game::IW3::ShaderCodeConstants, Game::IW4::ShaderCodeConstants> constantsTableToIW4;
        static const std::unordered_map<Game::IW3::MaterialTextureSource, Game::IW4::MaterialTextureSource> samplerTableToIW4;

        static const std::string techsetSuffix;

        const char* GetName() override { return "IMaterialTechniqueSet"; };
        Game::IW3::XAssetType GetType() override { return Game::IW3::XAssetType::ASSET_TYPE_TECHNIQUE_SET; };
        Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override { return { Convert(header.techniqueSet) }; };

    private:
        static Game::IW4::MaterialTechniqueSet* Convert(Game::IW3::MaterialTechniqueSet* techset);

        static Game::IW3::MaterialTechnique* ConvertTechnique(Game::IW3::MaterialTechnique* tech);
        static Game::IW4::MaterialVertexDeclaration* ConvertDecl(Game::IW3::MaterialVertexDeclaration* decl);
        static Game::IW3::MaterialVertexShader* ConvertVS(Game::IW3::MaterialVertexShader* vs);
        static Game::IW3::MaterialPixelShader* ConvertPS(Game::IW3::MaterialPixelShader* ps);
        static std::string GetDeclarationName(const Game::IW3::MaterialVertexDeclaration* decl);

        static const char* declStreamDestinations[];
        static const char* declStreamSources[];
        static std::unordered_map <std::int32_t, std::int32_t> iw3CodeConstMap;
    };
}
