#pragma once

namespace Components
{
    class IMaterialTechniqueSet : public AssetHandler::AssetInterface
    {
    public:
        IMaterialTechniqueSet();
        ~IMaterialTechniqueSet();

        const char* getName() override { return "IMaterialTechniqueSet"; };
        Game::XAssetType getType() override { return Game::XAssetType::ASSET_TYPE_TECHNIQUE_SET; };
        void dump(Game::IW3::XAssetHeader header) override { Dump(header.techniqueSet); };

    private:
        static void Dump(Game::IW3::MaterialTechniqueSet* techset);

        static void DumpTechnique(Game::IW3::MaterialTechnique* tech);
        static std::string DumpDecl(Game::IW3::MaterialVertexDeclaration* decl);
        static void DumpVS(Game::IW3::MaterialVertexShader* vs);
        static void DumpPS(Game::IW3::MaterialPixelShader* ps);

        static std::unordered_map <std::int32_t, std::int32_t> iw3CodeConstMap;
    };
}
