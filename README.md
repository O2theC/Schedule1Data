# Schedule 1 Mixing

This contains information related to Schedule 1 Mixing.

It includes:  

- Bases
- Effects  
- Items  
- Customers  

This repo is a bit janky — it started as a personal project, and at some point I decided to turn it into a local repo. Now I've decided to throw it on GitHub.

I'm probably not following most (or any) best practices, but the bulk of the functional work is done, so I’m starting to focus on cleaning things up. Since I have little to no experience, I’d appreciate any feedback I can get.

## Folders

- data
this contains the json files for Effects, Bases, Items and Customers
- dataExtraction
this has the C# scripts that extract game data into json format, meant to be run in game using unity explorer
- dataFile
holds the cpp constant file for use in cpp programs such as the calculator i made, to make it flexible, it uses a template file and a python file to fill in values using the data in ./data
- extrapolatedData
this holds data that is extrapolated from the data in ./data, such as what effects turn into what other effects for item and other such things, along with a programmatically made png of the effect graph
- scripts
holds a bunch of scripts for extrapolating data and other cool things

## Updates
in general this repo is for use by mixing calcs and related programs, as such this isn't often updated, as so far mixing hasn't meaningfully changed
we have somewhat recently gotten shrooms, but unfortunely they are (in the eyes of a mixing calc) just a reskin of meth or coke, this is because they have no base effects like weed does, the update also did not change effects in any way nor added any new effects, the same is for items, as such, nothing really changed for mixing, for shrooms, you can just use meth or coke, switch out the base price and that's it
as such, this is not oftenly updated, if an update comes out that changes things, then this will be updated

## Game Data
the scripts in dataExtraction are used in a program like UnityExplorer that provides a REPL, in theory any method that allows executing arbitrary scripts during runtime would work, this could include a mod
the scripts put their output in your clipboard as minified json, so just paste it and format it (or not), and there is the data
the scripts are designed to grab almost every bit of data from the objects/classes of Bases, Customers, Effects, and Items. As such some of this data is not as relevent as other bits, as they are not persistent between saves, meaning they can change depending on who and on which save the scripts are run