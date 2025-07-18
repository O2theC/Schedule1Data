

#include "./dataFile.hpp"










//modifys effects in place
void mixItem(EffectSet &effects, const uint8_t itemId)
{

    const uint64_t containedEffects = effects.effects.data & ITEM_EFFECT_REPLACE_MASK[itemId].effects.data;

    if (containedEffects != 0) [[likely]]
    {
        effects.effects.data &= ~containedEffects;

        for (uint8_t bit = __builtin_ctzll(containedEffects); containedEffects >> bit != 0; bit = __builtin_ctzll(containedEffects >> (bit + 1)) + bit + 1)
        {
            if ((effects.effects.data & (((uint64_t)1) << ITEM_EFFECT_REPLACE_MAP[itemId][bit])) == 0)
            {
                effects.effects.data |= ((uint64_t)1) << ITEM_EFFECT_REPLACE_MAP[itemId][bit];
            }
            else
            {
                effects.effects.data |= ((uint64_t)1 << bit);
            }

            if (containedEffects >> (bit + 1) == 0)
            {
                break;
            }
        }
    }

    if (effects.size() < 8) [[likely]]
    {
        effects.add(ITEM_EFFECTS[itemId]);
    }
};

EffectSet::EffectSet(const EffectSet &effectsOther, const uint8_t itemId) : effects(effectsOther.effects)
{
    mixItem(*this, itemId);
}


uint64_t pow(uint64_t base, uint64_t power)
{
    uint64_t result = 1;
    for (size_t i = 0; i < power; i++)
    {
        result *= base;
    }
    return result;
}



//time g++ -c -O3 -march=native --std=c++23 -o ./getData/dataFile/dataFile.o ./getData/dataFile/dataFile.cpp