import json
from scripts.pyLib import CustomerData


avg = {}

for customer in CustomerData:
    for key in customer.keys():
        if isinstance(customer[key], str):
            continue
        elif isinstance(customer[key], float) or isinstance(customer[key], int):
            avg[key] = avg.get(
                key, {"min": 99999999, "max": -99999999, "avg": 0, "amt": 0}
            )
            avg[key]["min"] = min(avg[key]["min"], customer[key])
            avg[key]["max"] = max(avg[key]["max"], customer[key])
            avg[key]["amt"] += 1
            avg[key]["avg"] += customer[key]
            if customer == CustomerData[-1]:
                avg[key]["avg"] /= avg[key]["amt"]
        elif isinstance(customer[key], dict):
            for key2 in customer[key].keys():
                if isinstance(customer[key][key2], str):
                    continue
                elif isinstance(customer[key][key2], float) or isinstance(
                    customer[key2], int
                ):
                    avg[key] = avg.get(key, {})
                    avg[key][key2] = avg[key].get(
                        key2, {"min": 99999999, "max": -99999999, "avg": 0, "amt": 0}
                    )
                    avg[key][key2]["min"] = min(
                        avg[key][key2]["min"], customer[key][key2]
                    )
                    avg[key][key2]["max"] = max(
                        avg[key][key2]["max"], customer[key][key2]
                    )
                    avg[key][key2]["amt"] += 1
                    avg[key][key2]["avg"] += customer[key][key2]
                    if customer == CustomerData[-1]:
                        avg[key][key2]["avg"] /= avg[key][key2]["amt"]


print(json.dumps(avg, indent=4))
