import json
import os

if(os.path.exists("./getData") and os.path.isdir("./getData")):
    from getData.pyLib import *
else:
    from pyLib import *




data: list[dict] = CustomerData
    
def keyfunc(element):
    return element["MaxWeeklySpend"]    

data.sort(key=keyfunc)

for customer in data:
    print(f"{customer["fullName"]} - {customer["MaxWeeklySpend"]}")

with open(pathPrefix+"./extrapolatedData/sortedCustomerData.json","w") as f:
    f.write(json.dumps(data,indent=4))