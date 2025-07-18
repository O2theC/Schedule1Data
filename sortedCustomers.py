import json




with open("./getData/data/customerData.json","r") as f:
    data: list[dict] = json.loads(f.read())
    
def keyfunc(element):
    return element["MaxWeeklySpend"]    

data.sort(key=keyfunc)

for customer in data:
    print(f"{customer["fullName"]} - {customer["MaxWeeklySpend"]}")

with open("./getData/extrapolatedData/sortedCustomerData.json","w") as f:
    f.write(json.dumps(data,indent=4))