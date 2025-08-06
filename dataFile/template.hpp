
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fmt/core.h>
#include <cmath>
#include <cstdint>
#include <boost/container/small_vector.hpp>

#define MAX_MIXS 39
#define EFFECT_AMT %EFFECT_AMT%
#define BASE_AMT %BASE_AMT%
#define ITEM_AMT %ITEM_AMT%
#define MAX_ITEM_EFFECT_CHANGERS %EFFECT_CHANGERS_AMT%
#define SCALER %SCALER%
#define GAME_VERSION ""

constexpr uint8_t MIX_BYTES = std::ceil(std::log2(MAX_MIXS) / 8.0);
constexpr uint8_t EFFECTS_BYTES = std::ceil(EFFECT_AMT / 8.0);
constexpr uint8_t BASES_BYTES = std::ceil(std::log2(BASE_AMT) / 8.0);
constexpr uint8_t ITEMS_BYTES = std::ceil(std::log2(ITEM_AMT) / 8.0);



using boost::container::small_vector;
using fmt::format;
using std::string;
using std::vector;


constexpr uint16_t EFFECT_MODIFIERS[] = %EFFECT_MODIFIERS%;

//not a fan of having to have these up here, but i don't want another file
//has to be below EFFECT_MODIFIERS because it uses, at least IDEs allow compress/unexpand stuff

template <typename T>
inline void hash_combine(std::size_t &seed, const T &val)
{
    std::hash<T> hasher;
    seed ^= hasher(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

struct customBitSet
{
    uint64_t data;

    constexpr customBitSet() : data(0) {}

    constexpr customBitSet(const uint64_t data_) : data(data_) {}

    uint8_t bitcount() const
    {
        return __builtin_popcountll(data);
    }

    void setTrue(const uint8_t index)
    {
        data |= (1ULL << index);
    }

    void setFalse(const uint8_t index)
    {
        data &= ~(1ULL << index);
    }

    void set(const uint8_t index, const bool val)
    {
        if (val)
        {
            setTrue(index);
        }
        else
        {
            setFalse(index);
        }
    }

    bool test(const uint8_t index) const
    {
        return (data >> index) & 1;
    }

    uint8_t findNext(const uint8_t index) const
    {
        return __builtin_ctzll(data >> index) + index;
    }

    uint8_t findNext() const
    {
        return __builtin_ctzll(data);
    }

    void clear()
    {
        data = 0;
    }
};

template <typename Type>
void print(const Type &var)
{
    std::cout << var << std::endl;
}


const float EFFECT_ADDICTION[] = %EFFECT_ADDICTION%;

struct EffectSet
{
    customBitSet effects;
    constexpr EffectSet() { clear(); };

    constexpr EffectSet(const uint8_t effectId)
    {
        clear();
        add(effectId);
    };

    EffectSet(const EffectSet &effectsOther, const uint8_t itemId);

    constexpr EffectSet(const uint64_t effects_) : effects(effects_) {
                                                   };

    template <size_t N>
    constexpr EffectSet(uint8_t effects[N])
    {
        for (uint8_t i = 0; i < N; i++)
        {
            add(effects[i]);
        };
    }

    constexpr bool operator==(const EffectSet &other) const
    {
        return effects.data == other.effects.data;
    }

    constexpr uint16_t getAddiction()
    {
        uint16_t addiction = 0;
        for (uint8_t effectId : *this)
        {
            addiction += (int)(EFFECT_ADDICTION[effectId]*1000);
        }
        if(addiction > 1000){
            return 1000;
        }
        return addiction;
    }

    struct Iterator
    {
        const EffectSet *parent;
        uint8_t index;

        Iterator(const EffectSet *parent, uint8_t start)
            : parent(parent), index(start)
        {
            if (!parent->effects.test(0))
            {
                next();
            }
        }

        void next()
        {
            if (index >= EFFECT_AMT || parent->effects.data >> (index + 1) == 0)
            {
                index = EFFECT_AMT;
                return;
            }
            // std::cout << (int)index ;
            index = this->parent->effects.findNext(index + 1);
            // std::cout << " -> " << (int) index << std::endl;
        }

        Iterator &operator++()
        {
            next();
            return *this;
        }

        bool operator!=(const Iterator &other) const
        {
            return index != other.index;
        }

        uint8_t operator*() const
        {
            return index;
        }};

     Iterator begin() const { return Iterator(this, 0); }
    Iterator end() const { return Iterator(this, EFFECT_AMT); }

    constexpr void clear()
    {
        effects.clear();
    };

   void add(const uint8_t effectId)
    {
        // effects |= (1 << effect.id);
        if(effectId == 255){
            print("invaild effect id, did you use invaild name with effect lookup?");
            std::abort();
        }
        effects.setTrue(effectId);
        if (effects.bitcount() > 8)
        {
            std::abort();
        }
    };

    void remove(const uint8_t effectId)
    {
        // effects &= ~(1 << effect.id);
        effects.setFalse(effectId);
        if (effects.bitcount() > 8)
        {
            std::abort();
        }
    };

    size_t size() const
    {
        if (effects.bitcount() > 8)
        {
            std::abort();
        }
        return effects.bitcount();
    }

    uint16_t totalModifier() const
    {
        uint16_t modifier = 100;
        for (uint64_t bits = effects.data; bits; bits &= bits - 1)
        {
            modifier += EFFECT_MODIFIERS[__builtin_ctzll(bits)];
        }
        if (effects.bitcount() > 8)
        {
            std::abort();
        }
        return modifier;
    }

    bool contains(const EffectSet &other) const
    {

        if (effects.bitcount() > 8)
        {
            std::abort();
        }
        return other.effects.data == (effects.data & other.effects.data);
    }

    bool containsAny(const EffectSet &other) const
    {

        if (effects.bitcount() > 8)
        {
            std::abort();
        }
        return effects.data & other.effects.data;
    }
};

void mixItem(EffectSet &effects, const uint8_t itemId);



const std::string EFFECTNAMES[] = %EFFECT_NAMES%;

constexpr uint16_t BASE_PRICES[] = %BASE_PRICES%;

constexpr uint16_t BASE_COSTS[] = %BASE_COSTS%;

// the base effects that each base has
constexpr EffectSet BASE_BASE_EFFECTS[] = %BASE_EFFECTS%;

const std::string BASE_NAMES[] = %BASE_NAMES%;

constexpr uint16_t ITEM_PRICES[] = %ITEM_PRICES%; // these are 10x to add 2 places of fixed decimals

constexpr uint8_t ITEM_EFFECTS[] = %ITEM_EFFECTS%;

// for each item (based on index/item id) a bit mask of what effects to replace, used in the replace map

constexpr EffectSet ITEM_EFFECT_REPLACE_MASK[] = %ITEM_EFFECT_REPLACE_MASK%;

// for an item, and a certain effect (replacee), what effect should be the replacer
constexpr uint8_t ITEM_EFFECT_REPLACE_MAP[ITEM_AMT][EFFECT_AMT] = %ITEM_EFFECT_REPLACE_MAP%;

const std::string ITEM_NAMES[] = %ITEM_NAMES%;




namespace std
{
    template <>
    struct hash<EffectSet>
    {
        size_t operator()(const EffectSet &p) const noexcept
        {

            size_t h1 = std::hash<uint64_t>{}(p.effects.data);
            return h1;
        }
    };
}

template <typename Type, uint8_t Capacity>
struct customVector
{
    Type data[Capacity];
    uint8_t length;

    constexpr customVector() {
        length = 0;
    };

    template <typename OtherType, typename = std::enable_if_t<std::is_convertible_v<OtherType, Type>>>
    constexpr customVector(const customVector<OtherType, Capacity> &other) : data(other.data), length(other.length)
    {
    }


    constexpr customVector(const Type otherData[Capacity], const uint8_t otherLength) : data(otherData), length(otherLength) {}

    const Type &operator[](const uint8_t index) const
    {
        if (index > length)
        {
            throw std::out_of_range("Index out of bounds");
        };
        return data[index];
    }

    Type &operator[](const uint8_t index)
    {
        if (index >= length)
        {
            throw std::out_of_range("Index out of bounds");
        };
        return data[index];
    }

    void push_back(const Type &element)
    {
        if (length >= Capacity)
        {
            throw std::runtime_error("array can't get bigger");
        };
        data[length] = element;
        length++;
    }

    uint8_t size() const
    {
        return length;
    }

    uint8_t find(const Type toFind) const
    {
        for (uint8_t i = 0; i < length; i++)
        {
            if (data[i] == toFind)
            {
                return i;
            }
        }
        return 255;
    }

    struct Iterator
    {
        using Type_ = Type;
        const customVector *parent;
        uint8_t index;

        Iterator(const customVector *parent, uint8_t start)
            : parent(parent), index(start)
        {
        }

        void next()
        {
            index += 1;
        }

        Iterator &operator++()
        {
            next();
            return *this;
        }

        bool operator!=(const Iterator &other) const
        {
            return index != other.index;
        }

        const Type_ &operator*() const
        {
            return parent->data[index];
        }
    };

    Iterator begin() const { return Iterator(this, 0); }
    Iterator end() const { return Iterator(this, length); }
};

struct NodeBase{
    uint8_t baseId = 255; // strain base id
    uint8_t mixFromParent = 255; // item id

    uint32_t makeprice = ((uint32_t)0) - 1; // total ingrident cost so far (note uses a scaler of 100 to allow for decimals )
    EffectSet effects;  // what effects does it have, a position of sorts

    uint8_t mixDepth = ((uint8_t)0) - 1;
    small_vector<uint32_t, ITEM_AMT> childrenIndexs;// parent and children are needed so when a better path is found, cost can be propagated
    uint32_t parentIndex = 25000000;    
    bool isStartNode = false; // when this is true, you can no longer expect parrent, or mixFromParrent to have valid data
    uint32_t selfIndex = 0;
    
    constexpr NodeBase() {}

    NodeBase(const small_vector<uint32_t, ITEM_AMT> &childrenIndexs_) : childrenIndexs(childrenIndexs_) {}

    constexpr NodeBase(NodeBase &node, uint8_t itemId) : baseId(node.baseId), mixFromParent(itemId), makeprice(node.makeprice + ITEM_PRICES[itemId]), effects(node.effects), mixDepth(node.mixDepth + 1), parentIndex(node.selfIndex) {};

    constexpr NodeBase(const uint8_t baseId_, const uint8_t index) : baseId(baseId_), makeprice(BASE_COSTS[baseId_]), effects(BASE_BASE_EFFECTS[baseId_]), mixDepth(0), isStartNode(true), selfIndex(index) {};
};


namespace std
{
	template <>
	struct hash<NodeBase>
	{
		std::size_t operator()(const NodeBase &node) const noexcept
		{
			std::size_t seed = 0;
			hash_combine(seed, node.baseId);
			hash_combine(seed, node.mixFromParent);
			hash_combine(seed, node.makeprice);
			hash_combine(seed, node.effects);
			hash_combine(seed, node.mixDepth);
			hash_combine(seed, node.parentIndex);
            hash_combine(seed, node.isStartNode);
            hash_combine(seed, node.selfIndex);
			for (size_t i = 0; i < node.childrenIndexs.size(); i++)
			{
				hash_combine(seed, node.childrenIndexs[i]);
			}

			return seed;
		}
	};

}


constexpr size_t stringSearch(const std::string *list, uint8_t len, string toFind)
{

    for (size_t i = 0; i < len; i++)
    {
        if (list[i] == toFind)
        {
            return i;
        }
    }
    return 255;
}

constexpr size_t effectLookupByName(string effectName)
{
    return stringSearch(EFFECTNAMES, EFFECT_AMT, effectName);
}

constexpr size_t baseLookupByName(string baseName)
{
    return stringSearch(BASE_NAMES, BASE_AMT, baseName);
}

constexpr size_t itemLookupByName(string itemName)
{
    return stringSearch(ITEM_NAMES, ITEM_AMT, itemName);
}

uint64_t pow(uint64_t base, uint64_t power);
