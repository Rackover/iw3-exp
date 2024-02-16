#pragma once

namespace Components
{
	class IXModel : public AssetHandler::AssetInterface
	{

	public:
		
		IXModel();
		~IXModel();

		const char* GetName() override { return "IXModel"; };
		Game::IW3::XAssetType GetType() override { return Game::IW3::XAssetType::ASSET_TYPE_XMODEL; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override { return { Convert(header.model) }; };

		static void Reset();

	private:

		struct IW3ModelSurf
		{
			const Game::IW3::XSurface* surface;
			const uint16_t numSurfs;

			IW3ModelSurf() : surface(nullptr), numSurfs(0) {};

			IW3ModelSurf(const Game::IW3::XSurface* surfs, uint8_t numSurfs) :
				surface(surfs),
				numSurfs(numSurfs)
			{
			}


			bool operator==(const IW3ModelSurf& other) const
			{
				return (surface == other.surface
					&& numSurfs == other.numSurfs);
			}
		};

		struct IW3ModelSurfHasher
		{
			std::size_t operator()(const IW3ModelSurf& k) const
			{
				// Compute individual hash values for first,
				// second and third and combine them using XOR
				// and bit shifting:

				return ((std::hash<DWORD>()(reinterpret_cast<DWORD>(k.surface))
					^ (std::hash<uint16_t>()(k.numSurfs) << 1)) >> 1);
			}
		};

		struct IW4ModelSurfCouple
		{
			Game::IW4::XSurface* surfs;
			Game::IW4::XModelSurfs* xModelSurfs;

			IW4ModelSurfCouple() {};

			IW4ModelSurfCouple(Game::IW4::XSurface* surfs, Game::IW4::XModelSurfs* xModelSurfs)
			{
				this->surfs = surfs;
				this->xModelSurfs = xModelSurfs;
			}
		};

		// This is used to link existing surfaces and allow Zonebuilder to optimize them when writing the zones
		// and prevent model duplication
		static std::unordered_map<IW3ModelSurf, IW4ModelSurfCouple, IW3ModelSurfHasher> convertedModelSurfaces;

		static Game::IW4::XModel* Convert(Game::IW3::XModel* model);

		static void AddMissingMultiplayerModelBones(Game::IW4::XModel* model);

		static uint8_t GetIndexOfBone(const Game::IW4::XModel* model, std::string name);
		static uint8_t GetParentIndexOfBone(const Game::IW4::XModel* model, uint8_t index);
		static void SetParentIndexOfBone(Game::IW4::XModel* model, uint8_t boneIndex, uint8_t parentIndex);
		static std::string GetParentOfBone(Game::IW4::XModel* model, uint8_t index);
		static uint8_t GetHighestAffectingBoneIndex(const Game::IW4::XModelLodInfo* lod);
		static void RebuildPartBits(Game::IW4::XModel* model);
		static uint8_t InsertBone(Game::IW4::XModel* model, const std::string& boneName, const std::string& parentName, Utils::Memory::Allocator& allocator);

		static void SetBoneTrans(Game::IW4::XModel* model, uint8_t boneIndex, bool baseMat, float x, float y, float z);
		static void SetBoneQuaternion(Game::IW4::XModel* model, uint8_t boneIndex, bool baseMat, float x, float y, float z, float w);
	};
}
