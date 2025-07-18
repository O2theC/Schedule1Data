import json
import math
import os

nameKeyorID = "Name"


# load the raw data for effects and items
effectData = json.load(open("./getData/data/EffectData.json", "r"))
itemData = json.load(open("./getData/data/ItemData.json", "r"))

effectLookup = dict()
# it's useful to be able to get an effect from it's Name
# as a standard, we use Name, since it's the most human readable
for effect in effectData:
    effectLookup[effect[nameKeyorID]] = effect

rad = 0.4

# this script creates 3 main data files
# mixData.json - for each item, what effects turn into what other effects, basically, the item's conversions
# effectDistance.json - the distance from one effect to another, done in mixs, so for effect A, it would X mixs to get to effect B ...
# this is no longer used currently, was intended to make a heuristic for A* but mixing and the effect changes are way too messy to make a good heuristic...
# if you want to give it a go, go ahead, but be ready for pain
# allTransforms.json - for each effect, what other effects can it be turned to in 1 mix, not sure on this one, more than likely same as effectDistance.json...
# made for my attempt at A*, but since the whole heuristic problem, not likely to be used

mixData = dict()

effectDistance = dict()

allTransforms = dict()

# calculate for each item, what effects turn into what other effects
# this is done by getting the vector of the item's effect (each item has an effect associated with it)
# and applying that vector against the "postion"(each effect has a vector which is also kind of used as a position) of the input effect 
for effect in effectData:
    pos: list[float] = effect["pos"]

    for item in itemData:
        vec = [0, 0]
        for itemEffect in item["effects"]:
            vec[0] += (
                effectLookup[itemEffect]["mixDir"][0]
                * effectLookup[itemEffect]["mixMagnitude"]
            )
            vec[1] += (
                effectLookup[itemEffect]["mixDir"][1]
                * effectLookup[itemEffect]["mixMagnitude"]
            )

        posCopy = pos.copy()
        posCopy[0] += vec[0]
        posCopy[1] += vec[1]

        for effect2 in effectData:
            effect2Pos = effect2["pos"]
            if (
                math.sqrt(
                    pow(posCopy[0] - effect2Pos[0], 2)
                    + pow(posCopy[1] - effect2Pos[1], 2)
                )
                < rad
            ):
                allTransforms[effect[nameKeyorID]] = allTransforms.get(effect[nameKeyorID], [])
                allTransforms[effect[nameKeyorID]].append(effect2[nameKeyorID])

                mixData[item[nameKeyorID]] = mixData.get(item[nameKeyorID], dict())
                # if this fails then that means somehow a vector landed in an area that 2 effects overlap
                # this should not be possible at time of writing, but due to prevoius mistakes in data handleing that made it look like it could
                # well, i just want to make sure
                assert mixData[item[nameKeyorID]].get(effect[nameKeyorID], "") == ""
                mixData[item[nameKeyorID]][effect[nameKeyorID]] = effect2[nameKeyorID]



# below is just for effectDistance.json, feel free to uncomment the exit() to not do it since it takes a min or 2
#exit(0)

import tqdm

global maxDepth, bar

maxDepth = 13



highestDis = 0

if os.path.exists("./getData/extrapolatedData"):
    if os.path.isfile("./getData/extrapolatedData"):
        print("please make extrapolatedData a directory/folder , not a file")
        exit(1)
else:
    print("extrapolatedData folder does not exist, making now")
    os.mkdir("./getData/extrapolatedData")

# 88acf783fb4eec3dea8b009565b1783a7015a8d8a2d9f3949c9786d8577acf015c237cb991f1d56a87ce30f703f90abdd532f68090bb6354fd201dfdbb4e53e3
# a80694f5ce1263f14605ade53e72e94b4c7ab65f778486a44cbbf0d592947229feec06bb8b5b777285cf9aa72bc494cb372f14b31636758a6d2863a84168fafa
def findEffectDistance(effect, effect2, depth, history):
    global maxDepth, bar, highestDis

    if depth >= maxDepth or effect in history:
        return 200

    if effect2[nameKeyorID] in allTransforms.get(effect[nameKeyorID], []):
        effectDistance[effect[nameKeyorID]] = effectDistance.get(effect[nameKeyorID], dict())
        effectDistance[effect[nameKeyorID]][effect2[nameKeyorID]] = 1
        return 1
    elif effect == effect2:
        effectDistance[effect[nameKeyorID]] = effectDistance.get(effect[nameKeyorID], dict())
        effectDistance[effect[nameKeyorID]][effect2[nameKeyorID]] = 0
        return 0
    else:
        dis = 200
        for effect3 in allTransforms.get(effect[nameKeyorID], []):
            dis = min(
                dis,
                findEffectDistance(
                    effectLookup[effect3], effect2, depth + 1, [*history, effect]
                )
                + 1,
            )
        if dis != 200:
            highestDis = max(dis, highestDis)
        return dis

print("working on part 1")
bar = tqdm.tqdm(total=len(effectData) ** 2)
for effect in effectData:
    for effect2 in effectData:
        bar.update(1)
        effectDistance[effect[nameKeyorID]] = effectDistance.get(effect[nameKeyorID], dict())
        if effectDistance[effect[nameKeyorID]].get(effect2[nameKeyorID], None) == None:
            effectDistance[effect[nameKeyorID]][effect2[nameKeyorID]] = findEffectDistance(
                effect, effect2, 0, []
            )
    effectDistance[effect[nameKeyorID]]["zeroPos"] = 200
bar.close()
print("part 1 done")
print("working on part 2")
bar = tqdm.tqdm(total=len(effectData))
for effect2 in effectData:
    dis = 200
    bar.update(1)
    for effectItem in itemData:
        fromEffect = effectLookup[effectItem["effects"][0]]
        dis = min(dis, findEffectDistance(fromEffect, effect2, 0, []) + 1)
    effectDistance["zeroPos"] = effectDistance.get("zeroPos", dict())
    effectDistance["zeroPos"][effect2[nameKeyorID]] = dis
effectDistance["zeroPos"]["zeroPos"] = 0

bar.close()
print("part 2 done")


print("done")
print("higest dis", highestDis)
print(f"maxdepth is {maxDepth}")
print("if highest dis is at or above maxdepth, then maxdepth should be increased")



with open("./getData/extrapolatedData/mixData.json", "w") as f:
    f.write(json.dumps(mixData, indent=4))

with open("./getData/extrapolatedData/allTransforms.json", "w") as f:
    f.write(json.dumps(allTransforms, indent=4))

with open("./getData/extrapolatedData/effectDistance.json", "w") as f:
    f.write(json.dumps(effectDistance, indent=4))
