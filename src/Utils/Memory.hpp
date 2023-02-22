#pragma once

namespace Utils
{
	class Memory
	{
	public:
		class Allocator
		{
		public:
			typedef void(*FreeCallback)(void*);

			Allocator()
			{
				this->pool.clear();
				this->refMemory.clear();
			}
			~Allocator()
			{
				this->Clear();
			}

			void Clear()
			{
				std::lock_guard<std::mutex> _(this->mutex);

				for (auto i = this->refMemory.begin(); i != this->refMemory.end(); ++i)
				{
					if (i->first && i->second)
					{
						i->second(i->first);
					}
				}

				this->refMemory.clear();

				for (auto data : this->pool)
				{
					Memory::Free(data);
				}

				this->pool.clear();
			}

			void free(void* data)
			{
				std::lock_guard<std::mutex> _(this->mutex);

				auto i = this->refMemory.find(data);
				if (i != this->refMemory.end())
				{
					i->second(i->first);
					this->refMemory.erase(i);
				}

				auto j = std::find(this->pool.begin(), this->pool.end(), data);
				if (j != this->pool.end())
				{
					Memory::Free(data);
					this->pool.erase(j);
				}
			}

			void free(const void* data)
			{
				this->free(const_cast<void*>(data));
			}

			void reference(void* memory, FreeCallback callback)
			{
				std::lock_guard<std::mutex> _(this->mutex);

				this->refMemory[memory] = callback;
			}

			void* Allocate(size_t length)
			{
				std::lock_guard<std::mutex> _(this->mutex);

				void* data = Memory::Allocate(length);
				this->pool.push_back(data);
				return data;
			}
			template <typename T> inline T* Allocate()
			{
				return this->AllocateArray<T>(1);
			}
			template <typename T> inline T* AllocateArray(size_t count = 1)
			{
				return static_cast<T*>(this->Allocate(count * sizeof(T)));
			}

			bool empty()
			{
				return (this->pool.empty() && this->refMemory.empty());
			}

			char* DuplicateString(std::string string)
			{
				std::lock_guard<std::mutex> _(this->mutex);

				char* data = Memory::DuplicateString(string);
				this->pool.push_back(data);
				return data;
			}

		private:
			std::vector<void*> pool;
			std::map<void*, FreeCallback> refMemory;
			std::mutex mutex;
		};

		static void* AllocateAlign(size_t length, size_t alignment);
		static void* Allocate(size_t length);
		template <typename T> static inline T* Allocate()
		{
			return AllocateArray<T>(1);
		}
		template <typename T> static inline T* AllocateArray(size_t count = 1)
		{
			return static_cast<T*>(Allocate(count * sizeof(T)));
		}

		static char* DuplicateString(std::string string);

		static void Free(void* data);
		static void Free(const void* data);

		static void FreeAlign(void* data);
		static void FreeAlign(const void* data);

		static bool IsSet(void* mem, char chr, size_t length);
	};
}
