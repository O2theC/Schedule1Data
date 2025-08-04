import json
from scripts.pyLib import CustomerData


def keyfunc(element):
    return element["MaxWeeklySpend"]


CustomerData.sort(key=keyfunc)

for customer in CustomerData:
    print(f"{customer["fullName"]} - {customer["MaxWeeklySpend"]}")

with open("./extrapolatedData/sortedCustomerData.json", "w") as f:
    f.write(json.dumps(CustomerData, indent=4))
