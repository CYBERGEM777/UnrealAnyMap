// COPYRIGHT 2022 CYBERGEM [CYBERGEM.NET]

#pragma once

#include "CoreMinimal.h"
#include <any>

namespace Cybergem {

//
//	UNREAL ANY VALUE MAP
//
//	Maps key type K to values of any copy constructible type.

template <typename K>
class FUnrealAnyValueMap
{
public:

	template <typename V>
	void Add(const K& k, const V& v)
	{
		Map.Add(k, std::any(v));
	}

	// Gets a copy of the value
	template <typename V>
	bool Get(const K& k, V& v) const
	{
		if (const auto* Any = Map.Find(k))
		{
			try
			{
				v = std::any_cast<V>(*Any);
				return true;
			}
			catch (...)
			{
				return false;
			}
		}

		return false;
	}

	// Returns a pointer to the value
	template <typename V>
	V* Find(const K& k)
	{
		V* v = nullptr;

		if (auto* Any = Map.Find(k))
		{
			try
			{
				v = std::any_cast<V>(Any);
				return v;
			}
			catch (...)
			{
				return v;
			}
		}

		return v;
	}

	// Returns true if any removed
	bool Remove(const K& k)
	{
		return Map.Remove(k) > 0;
	}

	int32 Num() const
	{
		return Map.Num();
	}

	void Empty(int32 ExpectedNumElements = 0)
	{
		Map.Empty(ExpectedNumElements);
	}

private:

	TMap<K, std::any> Map;
};

//
//	UNREAL ANY MAP
//
//	Maps any key type to values of any copy constructible type.

class FUnrealAnyMap
{
public:

	// Returns the type's name as a const char*
	template <typename K>
	const char* GetTypeName() const
	{
		return typeid(K).name();
	}

	// Adds k,v to AnyValueMap of key type K, creating the map if it doesn't exist
	template <typename K, typename V>
	void Add(const K& k, const V& v)
	{
		Add_Internal(k, v);
	}

	// Special case for string literal keys
	template <typename V>
	void Add(const char* k, const V& v)
	{
		Add_Internal(k, v);
	}

	// Gets a copy of the value
	template <typename K, typename V>
	bool Get(const K& k, V& v) const
	{
		return Get_Internal(k, v);
	}

	// Special case for string literal keys
	template <typename V>
	bool Get(const char* k, V& v) const
	{
		return Get_Internal(k, v);
	}

	// Returns a pointer to the value
	template <typename V, typename K>
	V* Find(const K& k)
	{
		return Find_Internal<V>(k);
	}

	// Special case for string literal keys
	template <typename V>
	V* Find(const char* k)
	{
		return Find_Internal<V>(k);
	}

	// Removes key k from AnyValueMap of key type K
	template <typename K>
	bool Remove(const K& k)
	{
		return Remove_Internal(k);
	}

	// Special case for string literal keys
	bool Remove(const char* k)
	{
		return Remove_Internal(k);
	}

	// Returns the number of elements with key type K
	template <typename K>
	int32 Num() const
	{
		if (auto AnyValueMap = GetAnyValueMap<K>())
		{
			return AnyValueMap->Num();
		}

		return 0;
	}
	
	// Returns the number of key types
	int32 NumKeyTypes() const
	{
		return Map.Num();
	}

	// Empty sub-map with key type K
	// Returns true if a map was emptied
	template <typename K>
	bool Empty(int32 ExpectedNumElements = 0)
	{
		if (auto AnyValueMap = GetAnyValueMap<K>())
		{
			if (ExpectedNumElements <= 0)
			{
				// Remove entirely instead of emptying
				RemoveSubMap<K>();
			}
			else
			{
				AnyValueMap->Empty(ExpectedNumElements);
			}

			return true;
		}

		return false;
	}

	// Empty the entire map
	void Empty(int32 ExpectedNumElements = 0)
	{
		Map.Empty(ExpectedNumElements);
	}

protected:

	template<typename K>
	auto GetAnyValueMap()
	{
		// Decay the type (important for working with string literals)
		return GetAnyValueMap_Internal<std::decay<K>::type>();
	}

	template<typename K>
	const auto GetAnyValueMap() const
	{
		// Decay the type (important for working with string literals)
		return GetAnyValueMap_Internal_Const<std::decay<K>::type>();
	}

	// Removes the sub-map with key type K
	template <typename K>
	void RemoveSubMap()
	{
		// Decay the type (important for working with string literals)
		Map.Remove(GetTypeName<std::decay<K>::type>());
	}

private:

	TMap<const char*, std::any> Map;

	template<typename K>
	auto CreateAnyValueMap()
	{
		// Decay the type (important for working with string literals)
		return CreateAnyValueMap_Internal<std::decay<K>::type>();
	}

	template<typename K>
	auto CreateAnyValueMap_Internal()
	{
		const char* KeyType = GetTypeName<K>();

		return std::any_cast<FUnrealAnyValueMap<K>>(
				&Map.Emplace(KeyType, std::any(FUnrealAnyValueMap<K>()))
		);
	}

	template<typename K>
	FUnrealAnyValueMap<K>* GetAnyValueMap_Internal()
	{
		auto AnyValueMap = GetAnyValueMap_Internal_Const<K>();
		return const_cast<FUnrealAnyValueMap<K>*>(AnyValueMap);
	}

	template<typename K>
	const FUnrealAnyValueMap<K>* GetAnyValueMap_Internal_Const() const
	{
		const char* KeyType = GetTypeName<K>();
		const FUnrealAnyValueMap<K>* AnyValueMap = nullptr;

		if (const std::any* Any = Map.Find(KeyType))
		{
			try
			{
				AnyValueMap = std::any_cast<const FUnrealAnyValueMap<K>>(Any);
				return AnyValueMap;
			}
			catch (...)
			{
				return AnyValueMap;
			}
		}

		return AnyValueMap;
	}

	// Adds k,v to AnyValueMap of key type K, creating the map if it doesn't exist
	template <typename K, typename V>
	void Add_Internal(const K& k, const V& v)
	{
		auto AnyValueMap = GetAnyValueMap<K>();
		if (!AnyValueMap)
		{
			AnyValueMap = CreateAnyValueMap<K>();
		}

		AnyValueMap->Add(k, v);
	}

	// Gets a copy of the value
	template <typename K, typename V>
	bool Get_Internal(const K& k, V& v) const
	{
		if (auto AnyValueMap = GetAnyValueMap<K>())
		{
			return AnyValueMap->Get(k, v);
		}

		return false;
	}

	// Returns a pointer to the value
	template <typename V, typename K>
	V* Find_Internal(const K& k)
	{
		V* v = nullptr;

		if (auto AnyValueMap = GetAnyValueMap<K>())
		{
			v = AnyValueMap->Find<V>(k);
		}

		return v;
	}

	// Removes key k from AnyValueMap of key type K
	template <typename K>
	bool Remove_Internal(const K& k)
	{
		if (auto AnyValueMap = GetAnyValueMap<K>())
		{
			if (AnyValueMap->Remove(k))
			{
				if (AnyValueMap->Num() == 0)
				{
					// Remove empty sub-map
					RemoveSubMap<K>();
				}

				return true;
			}
		}

		return false;
	}
};

}; // namespace Cybergem

