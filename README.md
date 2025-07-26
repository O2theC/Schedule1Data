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

## Files
### python scripts
we got some python scripts in root that do a few things
- avgCustomerData.py
this was to compute some min,max and avg for customer data, i doubt most will have use of it
- extrapolateData.py
this uses data in ./data to make json files for how items change effects and other things
- genEffectImg.py
this makes a png to show the effect graph 
- pyLib.py
this is mainly a python library, meant to centralize getting and using the data in ./data across python scripts
- sortedCustomer.py
another custom script that just sorts and prints customers based on their spending limits, not something i expect most to find value in
