Mixing in Schedule 1 is both complex and simple, first let us define some terms so we know what we are talking about

- Strain
this is the drug we can sell in game, this can be just the base drug like OG Kush or a base drug with something mixed with it
it has, effects and addiction.
A Strain can have up to 8 effects and maxes out at 100% addiction, while addiction is from 0 to 100% in game, for this, it will be referred to using a 0 to 1 scale, this is just mainly for how it's represented in the code, you can either divide or multiply by 100 to get between the two if wanted
- Item/Ingredient
the item(s) we can mix with a Strain to transform it's effects, such as cuke, donut, etc, each item has a price/cost ,this is what it costs to buy from gas mart, each item has an Effect tied to it
- Effect
There are a variety of effects in Schedule, currently there are 35 effects in the game, but only 34 can be gotten , for some reason, the effect Lethal exists but can not be gotten too
- Base
refers to the base drugs, there are currently 6 in game, the 4 weeds , meth and coke


-----

Mixing will be explained in an advanced and simple version, the advanced is for more details of the inner workings, 

### How Mixing Works (advanced)
the following makes sense if you imagine it on a 2d graph.
Each effect has a position on a 2d plane, along with a circle with radius of .4 centered on it's position, none of the effect's circle overlaps with any other effect.
Each effect also has a vector but this is technically it's position too, so the 2 will be referred to interchangeably 

when mixing an Item with a Strain, the game does the following

1. it grabs the effect tied to that item, along with that Effect's vector
2. for each effect on the Strain, it tests if applying the vector on that effects position would make the resulting point be in the circle of another effect, if so , it saves that as a possible conversion
3. after all possible conversions are found, it goes through them, checking if the resulting effect already exists, if it does then it doesn't do the conversion, if the resulting effect doesn't exist then original effect is turned to the resulting effect

though because all of these values are static and finite, this is often simplified to ...

### How Mixing Works (simple)

Each Item has a map of what effects turn into what other effect when the item is applied

For example, Cuke has a mapping of munchies to athletic , meaning if Cuke is mixed with a Strain that has the effect Munchies, then Munchies would be replaced with Athletic , but there is something important to know of, this replacement doesn't take place if the resulting effect already exists, so for our example, if Athletic already existed in the Strain then Munchies would stay and not change, along with that, an Effect can not change more than once per mix operation 

---- 


Now on to how pricing and addiction works

Each drug type has a base value, for weed it's \$35, for meth it's \$70 and \$150 for cocaine.
Each Effect has a multiplier and addiction value tied to it

to calculate a Strains sell price and addiction, take the effects that the Strain has, add up their multipliers, add 1, then multiply that against the Strain's base value using the type of drug it uses, for addiction just add up all the effect's addiction values, and there you go, though remember that the addiction values are on the 0 to 1 scale, so a total of 1 is max addiction, and unfortunately, no, an addiction higher than 1 doesn't do anything, it gets capped at 1


and those are the basics of mixing in schedule 1, related data should be found in the repo regarding Effects,Bases and Items