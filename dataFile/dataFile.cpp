

#include "./dataFile.hpp"

// modifys effects in place
void mixItem(EffectSet &effects, const uint8_t itemId)
{

    uint64_t containedEffects = effects.effects.data & ITEM_EFFECT_REPLACE_MASK[itemId].effects.data;

    if (containedEffects != 0) [[likely]]
    {
        effects.effects.data &= ~containedEffects;
        const uint8_t *EFFECT_REPLACE_MASK = ITEM_EFFECT_REPLACE_MAP[itemId];

        while (containedEffects)
        {
            //     //get lowest bit
            const uint8_t bit = __builtin_ctzll(containedEffects);

            const uint8_t replaceBit = EFFECT_REPLACE_MASK[bit];

            effects.effects.data |= ((effects.effects.data >> replaceBit) & 1) << bit;

            // final thing
            effects.effects.data |= (1UL << replaceBit);

            //     // remove lowest bit
            containedEffects &= containedEffects - 1;
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

// time g++ -c -O3 -march=native --std=c++23 -o ./getData/dataFile/dataFile.o ./getData/dataFile/dataFile.cpp