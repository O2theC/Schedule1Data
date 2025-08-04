import json
import math
import os
from scripts.pyLib import EffectData, ItemData, EffectLookup, nameKeyOrID

import time

# load the raw data for effects and items
start = time.time()

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
for effect in EffectData:
    pos: list[float] = effect["pos"]

    for item in ItemData:
        vec = [0, 0]
        for itemEffect in item["effects"]:
            # print(EffectLookup)
            vec[0] += (
                EffectData[EffectLookup[itemEffect]]["mixDir"][0]
                * EffectData[EffectLookup[itemEffect]]["mixMagnitude"]
            )
            vec[1] += (
                EffectData[EffectLookup[itemEffect]]["mixDir"][1]
                * EffectData[EffectLookup[itemEffect]]["mixMagnitude"]
            )

        posCopy = pos.copy()
        posCopy[0] += vec[0]
        posCopy[1] += vec[1]

        for toEffect in EffectData:
            effect2Pos = toEffect["pos"]
            if (
                math.sqrt(
                    pow(posCopy[0] - effect2Pos[0], 2)
                    + pow(posCopy[1] - effect2Pos[1], 2)
                )
                < rad
            ):
                allTransforms[effect[nameKeyOrID]] = allTransforms.get(
                    effect[nameKeyOrID], []
                )
                allTransforms[effect[nameKeyOrID]].append(toEffect[nameKeyOrID])

                mixData[item[nameKeyOrID]] = mixData.get(item[nameKeyOrID], dict())
                # if this fails then that means somehow a vector landed in an area that 2 effects overlap
                # this should not be possible at time of writing, but due to prevoius mistakes in data handleing that made it look like it could
                # well, i just want to make sure
                assert mixData[item[nameKeyOrID]].get(effect[nameKeyOrID], "") == ""
                mixData[item[nameKeyOrID]][effect[nameKeyOrID]] = toEffect[nameKeyOrID]


# below is just for effectDistance.json, feel free to uncomment the exit() to not do it since it takes a min or 2
# exit(0)
print(f"MixData.json and allTransforms.json: {time.time()-start:.2f} seconds")
start2 = time.time()
# import tqdm

global maxDepth, bar

maxDepth = 13


highestDis = 0

if os.path.exists("./extrapolatedData"):
    if os.path.isfile("./extrapolatedData"):
        print("please make extrapolatedData a directory/folder , not a file")
        exit(1)
else:
    print("extrapolatedData folder does not exist, making now")
    os.mkdir("./extrapolatedData")


# 88acf783fb4eec3dea8b009565b1783a7015a8d8a2d9f3949c9786d8577acf015c237cb991f1d56a87ce30f703f90abdd532f68090bb6354fd201dfdbb4e53e3
# a80694f5ce1263f14605ade53e72e94b4c7ab65f778486a44cbbf0d592947229feec06bb8b5b777285cf9aa72bc494cb372f14b31636758a6d2863a84168fafa
cnt = 0

wrapperMemory = dict()


def wrapper(func):
    def wrap(*args, **kwargs):
        # print("meow")
        # print(args)
        key = (args[0][nameKeyOrID], args[1][nameKeyOrID], args[2])
        before = wrapperMemory.get(key, -1)
        if wrapperMemory.get(key, -1) == -1:
            wrapperMemory[key] = func(*args)
        if before != -1 and wrapperMemory[key] != before:
            print(f"{before} for {key} became {wrapperMemory[key]}")
        return wrapperMemory[key]

    return wrap


# hash without wrapper
# 3860eede564e1536c5a384632f6d66fc60e511578a5a63bf058bdf2f5b0fa3fdec18a7c04f0d36c7881a28e105c30d3b10432297c82918480304f4f36c541bd4
# has wtih wrapper
# 3860eede564e1536c5a384632f6d66fc60e511578a5a63bf058bdf2f5b0fa3fdec18a7c04f0d36c7881a28e105c30d3b10432297c82918480304f4f36c541bd4


@wrapper
def findEffectDistance(effectFrom, effectTo, depth):
    global maxDepth, bar, highestDis, cnt
    cnt += 1

    if depth >= maxDepth:
        return 200

    # if(effectDistance.get(effectFrom[nameKeyOrID],dict()).get(effectTo[nameKeyOrID],-1) != -1):
    #     return effectDistance.get(effectFrom[nameKeyOrID],dict()).get(effectTo[nameKeyOrID],-1)

    if effectFrom[nameKeyOrID] == effectTo[nameKeyOrID]:
        effectDistance[effectFrom[nameKeyOrID]] = effectDistance.get(
            effectFrom[nameKeyOrID], dict()
        )
        effectDistance[effectFrom[nameKeyOrID]][effectTo[nameKeyOrID]] = 0
        return 0
    elif effectTo[nameKeyOrID] in allTransforms.get(effectFrom[nameKeyOrID], []):
        effectDistance[effectFrom[nameKeyOrID]] = effectDistance.get(
            effectFrom[nameKeyOrID], dict()
        )
        effectDistance[effectFrom[nameKeyOrID]][effectTo[nameKeyOrID]] = 1
        return 1
    else:
        distance = 200
        for effect3 in allTransforms.get(effectFrom[nameKeyOrID], []):

            distance = min(
                distance,
                findEffectDistance(
                    EffectData[EffectLookup[effect3]], effectTo, depth + 1
                )
                + 1,
            )
        if distance != 200:
            highestDis = max(distance, highestDis)
        return distance


print("working on part 1")
# bar = tqdm.tqdm(total=len(EffectData) ** 2)
for effect in EffectData:
    for toEffect in EffectData:
        # bar.update(1)
        effectDistance[effect[nameKeyOrID]] = effectDistance.get(
            effect[nameKeyOrID], dict()
        )
        if effectDistance[effect[nameKeyOrID]].get(toEffect[nameKeyOrID], None) == None:
            effectDistance[effect[nameKeyOrID]][toEffect[nameKeyOrID]] = (
                findEffectDistance(effect, toEffect, 0)
            )
    effectDistance[effect[nameKeyOrID]]["zeroPos"] = 200
# bar.close()
print("part 1 done")
print("working on part 2")
# bar = tqdm.tqdm(total=len(EffectData))
for toEffect in EffectData:
    dis = 200
    # bar.update(1)
    for effectItem in ItemData:
        fromEffect = EffectData[EffectLookup[effectItem["effects"][0]]]
        dis = min(dis, findEffectDistance(fromEffect, toEffect, 0) + 1)
    effectDistance["zeroPos"] = effectDistance.get("zeroPos", dict())
    effectDistance["zeroPos"][toEffect[nameKeyOrID]] = dis
effectDistance["zeroPos"]["zeroPos"] = 0

# bar.close()
print("part 2 done")
print(f"effectDistance.json : {time.time()-start2:.2f} seconds")

print("done")
print("higest dis", highestDis)
print(f"maxdepth is {maxDepth}")
print("if highest dis is at or above maxdepth, then maxdepth should be increased")
print(f"func was called {cnt} times")

with open("./extrapolatedData/mixData.json", "w") as f:
    f.write(json.dumps(mixData, indent=4))

with open("./extrapolatedData/allTransforms.json", "w") as f:
    f.write(json.dumps(allTransforms, indent=4))

with open("./extrapolatedData/effectDistance.json", "w") as f:
    f.write(json.dumps(effectDistance, indent=4))
