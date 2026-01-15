from scripts.pyLib import (
    EffectLookup,
    pathPrefix,
    EffectData,
    MixData,
    BaseData,
    ItemData,
    CustomerData,
)

# os.system("which python")
# os.system("pwd")
# if(os.path.exists("./getData") and os.path.isdir("./getData")):
#     from getData.pyLib import *
# else:
#     from pyLib import *
# from pyLib import *
# print(os.getcwd())


nameKeyOrID = "Name"
floatScaler = 100  # there are some float values but float is bad, so we use ints with a scaler so we can use less memory among other things


with open(pathPrefix + "./dataFile/template.hpp", "r") as f:
    template = f.read()


def formatAsEffectSet(effectName):
    if effectName == "NULL":
        return f"EffectSet((uint64_t)0)"
    if isinstance(effectName, int):
        return f"EffectSet((uint64_t){effectName})"
    if isinstance(effectName, str):
        bitSet = pow(2, EffectLookup[effectName])
        return f"EffectSet((uint64_t){bitSet})"


def makeReplaceMask(item):
    mask = 0
    for key in MixData[item[nameKeyOrID]].keys():
        mask += pow(2, EffectLookup[key])
    return mask


def makeReplaceMap(item):
    baseMap = [0 for effect in EffectData]
    for key in MixData[item[nameKeyOrID]].keys():
        baseMap[EffectLookup[key]] = EffectLookup[MixData[item[nameKeyOrID]][key]]
    return baseMap


# setup amts

# print("zomb mod")
# print(EffectData[-1]["addBaseValueMultiplier"])

ItemAmt = len(ItemData)
EffectAmt = len(EffectData)
BaseAmt = len(BaseData)
ItemEffectChangers = max([len(MixData[mixKey].keys()) for mixKey in MixData.keys()])
# print(template[:300])
template = template.replace(r"$EFFECT_AMT$", str(EffectAmt))
template = template.replace(r"$BASE_AMT$", str(BaseAmt))
template = template.replace(r"$ITEM_AMT$", str(ItemAmt))
template = template.replace(r"$SCALER$", str(floatScaler))
template = template.replace(r"$EFFECT_CHANGERS_AMT$", str(ItemEffectChangers))

EffectModifiers = []
EffectAddiction = []
EffectNames = []

for effect in EffectData:
    EffectModifiers.append(effect["addBaseValueMultiplier"])
    EffectAddiction.append(effect["addictiveness"])
    EffectNames.append(effect[nameKeyOrID])

# print(EffectModifiers)
EffectModifiersStr = (
    "{"
    + ",".join([str(round(effectMod * floatScaler)) for effectMod in EffectModifiers])
    + "}"
)
# print(EffectModifiersStr)

# not used in main calc, so can be normal float
EffectAddictionStr = (
    "{" + ",".join([str(round(addict, 3)) for addict in EffectAddiction]) + "}"
)
EffectNamesStr = "{" + ",".join([f'"{effect}"' for effect in EffectNames]) + "}"
EffectEnumStr = ",\n".join(
    [f'\t{name.upper().replace(" ","_").replace("-","_")}' for name in EffectNames]
)


template = template.replace(r"$EFFECT_MODIFIERS$", EffectModifiersStr)
template = template.replace(r"$EFFECT_ADDICTION$", EffectAddictionStr)
template = template.replace(r"$EFFECT_NAMES$", EffectNamesStr)
template = template.replace(r"$EFFECT_ENUM$", EffectEnumStr)

# what the base sells for with effects changing it
BasePrices = []
# what does the base cost, this is done by taking the ingridents and dividing by how much you get, currently this is manually, make sure to follow the order in the BaseData.json file
# uses float scaler
BaseCosts = [375, 438, 500, 562, 1900, 3383]
# the effects that bases like weed start with
BaseBaseEffects = []
BaseNames = []

for base in BaseData:
    BasePrices.append(base["basePrice"])
    # it weird, it stores it as an array, did dev intend for items to have multiple effects in the future, they seem to have intended for different mix maps for different things, like weed and meth mix differently , but both link to the weed mix map, it's intresting
    if len(base["effects"]) > 0:
        BaseBaseEffects.append(base["effects"][0])
    else:
        BaseBaseEffects.append("NULL")
    BaseNames.append(base[nameKeyOrID])

BasePricesStr = "{" + ",".join([str(int(price)) for price in BasePrices]) + "}"
BaseCostsStr = "{" + ",".join([str(cost) for cost in BaseCosts]) + "}"
BaseBaseEffectsStr = (
    "{" + ",".join([formatAsEffectSet(effect) for effect in BaseBaseEffects]) + "}"
)
BaseNamesStr = "{" + ",".join([f'"{name}"' for name in BaseNames]) + "}"
BaseEnumStr = ",\n".join(
    [f'\t{name.upper().replace(" ","_").replace("-","_")}' for name in BaseNames]
)


template = template.replace(r"$BASE_PRICES$", BasePricesStr)
template = template.replace(r"$BASE_COSTS$", BaseCostsStr)
template = template.replace(r"$BASE_EFFECTS$", BaseBaseEffectsStr)
template = template.replace(r"$BASE_NAMES$", BaseNamesStr)
template = template.replace(r"$BASE_ENUM$", BaseEnumStr)


ItemPrices = []
ItemEffects = []
ItemEffectReplaceMask = []
ItemEffectReplaceMap = []
ItemNames = []

for item in ItemData:
    ItemPrices.append(item["price"])
    ItemEffects.append(EffectLookup[item["effects"][0]])

    ItemEffectReplaceMask.append(makeReplaceMask(item))
    ItemEffectReplaceMap.append(makeReplaceMap(item))

    ItemNames.append(item[nameKeyOrID])


ItemPricesStr = (
    "{" + ",".join([str(int(price * floatScaler)) for price in ItemPrices]) + "}"
)
ItemEffectsStr = "{" + ",".join([str(effect) for effect in ItemEffects]) + "}"
ItemEffectReplaceMaskStr = (
    "{" + ",".join([formatAsEffectSet(mask) for mask in ItemEffectReplaceMask]) + "}"
)
ItemEffectReplaceMapStr = (
    "{\n\t"
    + ",\n\t".join(
        [
            ("{" + ", ".join([str(effectId) for effectId in effectMap]) + "}")
            for effectMap in ItemEffectReplaceMap
        ]
    )
    + "}"
)
ItemNamesStr = "{" + ",".join([f'"{name}"' for name in ItemNames]) + "}"
ItemEnumStr = ",\n".join(
    [f'\t{name.upper().replace(" ","_").replace("-","_")}' for name in ItemNames]
)

template = template.replace(r"$ITEM_PRICES$", ItemPricesStr)
template = template.replace(r"$ITEM_EFFECTS$", ItemEffectsStr)
template = template.replace(r"$ITEM_EFFECT_REPLACE_MASK$", ItemEffectReplaceMaskStr)
template = template.replace(r"$ITEM_EFFECT_REPLACE_MAP$", ItemEffectReplaceMapStr)
template = template.replace(r"$ITEM_NAMES$", ItemNamesStr)
template = template.replace(r"$ITEM_ENUM$", ItemEnumStr)



CustomerNamesEnum = ",\n".join(
    [f'\t{name["fullName"].upper().replace(" ","_").replace("-","_").replace(".","")}' for name in CustomerData]
)
template = template.replace(r"$CUSTOMER_ENUM$", CustomerNamesEnum)


with open(pathPrefix + "./dataFile/dataFile.hpp", "w") as f:
    f.write(template)
