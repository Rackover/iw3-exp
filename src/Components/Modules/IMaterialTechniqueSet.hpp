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

        const char* getName() override { return "IMaterialTechniqueSet"; };
        Game::XAssetType getType() override { return Game::XAssetType::ASSET_TYPE_TECHNIQUE_SET; };
        void dump(Game::IW3::XAssetHeader header) override { Dump(header.techniqueSet); };

    private:
        static Game::IW4::MaterialTechniqueSet* Dump(Game::IW3::MaterialTechniqueSet* techset);

        static std::string DumpTechnique(Game::IW3::MaterialTechnique* tech);
        static std::string DumpDecl(Game::IW3::MaterialVertexDeclaration* decl);
        static std::string DumpVS(Game::IW3::MaterialVertexShader* vs);
        static std::string DumpPS(Game::IW3::MaterialPixelShader* ps);
        static void SaveConvertedTechset(Game::IW4::MaterialTechniqueSet* techset);

        static std::unordered_map <std::int32_t, std::int32_t> iw3CodeConstMap;
        static Utils::Memory::Allocator localAllocator;
    };
}
