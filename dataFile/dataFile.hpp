
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fmt/core.h>
#include <cmath>
#include <cstdint>
#include <boost/container/small_vector.hpp>

#define MAX_MIXS 39
#define EFFECT_AMT 35
#define BASE_AMT 6
#define ITEM_AMT 16
#define MAX_ITEM_EFFECT_CHANGERS 11
#define SCALER 100

constexpr uint8_t MIX_BYTES = std::ceil(std::log2(MAX_MIXS) / 8.0);
constexpr uint8_t EFFECTS_BYTES = std::ceil(EFFECT_AMT / 8.0);
constexpr uint8_t BASES_BYTES = std::ceil(std::log2(BASE_AMT) / 8.0);
constexpr uint8_t ITEMS_BYTES = std::ceil(std::log2(ITEM_AMT) / 8.0);



using boost::container::small_vector;
using fmt::format;
using std::string;
using std::vector;


constexpr uint16_t EFFECT_MODIFIERS[] = {10,18,16,12,0,14,0,28,0,22,20,26,24,0,32,30,36,0,0,34,38,40,48,42,0,0,44,46,54,56,50,0,52,60,58};

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


const float EFFECT_ADDICTION[] = {0.0,0.235,0.104,0.096,0.0,0.104,0.0,0.1,0.0,0.34,0.0,0.0,0.327,0.0,0.607,0.0,0.1,0.1,0.0,0.309,0.665,0.2,0.472,0.343,0.0,0.0,0.37,0.803,0.611,0.1,0.235,0.0,0.607,0.336,0.598};

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



const std::string EFFECTNAMES[] = {"Calming","Euphoric","Focused","Munchies","Paranoia","Refreshing","Smelly","Calorie-Dense","Disorienting","Energizing","Gingeritis","Sedating","Sneaky","Toxic","Athletic","Balding","Foggy","Laxative","Seizure-Inducing","Slippery","Spicy","Bright-Eyed","Glowing","Jennerising","Lethal","Schizophrenic","Thought-Provoking","Tropic Thunder","Anti-gravity","Cyclopean","Electrifying","Explosive","Long faced","Shrinking","Zombifying"};

constexpr uint16_t BASE_PRICES[] = {35,35,35,35,70,150};

constexpr uint16_t BASE_COSTS[] = {375,438,500,562,1900,3383};

// the base effects that each base has
constexpr EffectSet BASE_BASE_EFFECTS[] = {EffectSet((uint64_t)1),EffectSet((uint64_t)32),EffectSet((uint64_t)512),EffectSet((uint64_t)2048),EffectSet((uint64_t)0),EffectSet((uint64_t)0)};

const std::string BASE_NAMES[] = {"OG Kush","Sour Diesel","Green Crack","Granddaddy Purple","Meth","Cocaine"};

constexpr uint16_t ITEM_PRICES[] = {900,200,800,700,200,300,600,500,500,900,800,700,600,400,300,400}; // these are 10x to add 2 places of fixed decimals

constexpr uint8_t ITEM_EFFECTS[] = {26,10,21,20,9,7,14,11,13,32,23,16,19,15,12,27};

// for each item (based on index/item id) a bit mask of what effects to replace, used in the replace map

constexpr EffectSet ITEM_EFFECT_REPLACE_MASK[] = {EffectSet((uint64_t)6446712832),EffectSet((uint64_t)4831847253),EffectSet((uint64_t)10200678410),EffectSet((uint64_t)8858521608),EffectSet((uint64_t)603146),EffectSet((uint64_t)8883568780),EffectSet((uint64_t)173082886),EffectSet((uint64_t)10284580879),EffectSet((uint64_t)9672202010),EffectSet((uint64_t)335807488),EffectSet((uint64_t)73891),EffectSet((uint64_t)8666239493),EffectSet((uint64_t)66074),EffectSet((uint64_t)2147483781),EffectSet((uint64_t)1079058973),EffectSet((uint64_t)8606859522)};

// for an item, and a certain effect (replacee), what effect should be the replacer
constexpr uint8_t ITEM_EFFECT_REPLACE_MAP[ITEM_AMT][EFFECT_AMT] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 1, 30, 0, 0},
	{12, 0, 18, 0, 23, 0, 28, 0, 2, 26, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 5, 0, 0},
	{0, 34, 0, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 3, 0},
	{0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 21, 0, 1, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 5, 0},
	{0, 17, 0, 14, 0, 0, 0, 0, 0, 0, 26, 0, 4, 1, 0, 0, 29, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 1, 0, 0, 0, 0, 31, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 10, 17, 0, 0, 0, 19, 0, 0, 0, 0, 9, 0},
	{0, 9, 33, 0, 0, 0, 0, 0, 30, 0, 0, 3, 0, 0, 0, 0, 17, 0, 0, 0, 1, 0, 8, 0, 0, 15, 0, 12, 0, 0, 0, 0, 0, 0, 0},
	{21, 13, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 10, 0, 0, 16, 5, 0, 0, 4, 0},
	{0, 20, 0, 11, 0, 0, 0, 0, 22, 1, 6, 0, 27, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 8, 0, 0, 2, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0, 0, 0},
	{15, 18, 0, 0, 0, 26, 0, 10, 0, 0, 0, 0, 0, 12, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{22, 0, 8, 0, 0, 0, 0, 0, 0, 29, 0, 0, 0, 0, 17, 0, 0, 0, 2, 13, 0, 0, 0, 4, 0, 0, 9, 0, 0, 0, 0, 0, 0, 30, 0},
	{0, 11, 0, 25, 28, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{28, 0, 23, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 0, 0, 0},
	{19, 0, 10, 28, 15, 0, 0, 0, 0, 4, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 21, 0, 13, 0, 0, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0},
	{0, 21, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0}};

const std::string ITEM_NAMES[] = {"Addy","Banana","Battery","Chili","Cuke","Donut","Energy Drink","Flu Medicine","Gasoline","Horse Semen","Iodine","Mega Bean","Motor Oil","Mouth Wash","Paracetamol","Viagor"};




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
