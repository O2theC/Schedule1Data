import json

nameKeyOrID = "Name"

with open("./data/BaseData.json") as f:
    BaseData = json.load(f)
with open("./data/CustomerData.json") as f:
    CustomerData = json.load(f)
with open("./data/EffectData.json") as f:
    EffectData = json.load(f)
with open("./data/ItemData.json") as f:
    ItemData = json.load(f)
with open("./extrapolatedData/mixData.json") as f:
    MixData = json.load(f)
    
    
BasesLookup = {}
EffectLookup = {}
ItemLookup = {}
CustomerLookup = {}

for i in range(len(BaseData)):
    BasesLookup[BaseData[i][nameKeyOrID]] = i

for i in range(len(EffectData)):
    EffectLookup[EffectData[i][nameKeyOrID]] = i

for i in range(len(ItemData)):
    ItemLookup[EffectData[i][nameKeyOrID]] = i
    
for i in range(len(CustomerData)):
    CustomerLookup[CustomerData[i]["fullName"]] = i
    if(ItemLookup.get(CustomerData[i]["FirstName"],False)):
        if(CustomerData[i]["FirstName"] != "Cranky Frank"):
            print(CustomerData[i]["FirstName"])
            print("DUPE - ",CustomerData[i])
        
    CustomerLookup[CustomerData[i]["FirstName"]] = i
    CustomerLookup[CustomerData[i]["GUID"]] = i
    CustomerLookup[CustomerData[i]["ID"]] = i
    
    