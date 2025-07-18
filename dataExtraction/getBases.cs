using System;
using System.Collections.Generic;
using System.Linq;
using ScheduleOne.DevUtilities;
using ScheduleOne.Map;
using ScheduleOne.Persistence;
using ScheduleOne.Persistence.Loaders;
using UnityEngine;
using ScheduleOne.Economy;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;



//do above and below seperately 



var settings = new JsonSerializerSettings
{
    Converters = { new StringEnumConverter() }
};

var productManager = NetworkSingleton<ScheduleOne.Product.ProductManager>.Instance;
var bases = productManager.AllProducts;
var dict = new List<Dictionary<string, object>>();

for (int i = 0; i < bases.Count; i++)
{
    var drugBase = new Dictionary<string, object>();
    var drugObj = bases[i];
    if (drugObj.Recipes.Count != 0)
    { 
        continue;
    }

    drugBase["Name"] = drugObj.Name;
    drugBase["name"] = drugObj.name;
    drugBase["ID"] = drugObj.ID;
    var Effects = new List<string>();
    for (int ii = 0; ii < drugObj.Properties.Count; ii++)
    {
        Effects.Add(drugObj.Properties[ii].Name);
    }
    drugBase["effects"] = Effects;
    drugBase["drugType"] = drugObj.DrugType;
    drugBase["price"] = drugObj.Price;
    drugBase["description"] = drugObj.Description;
    drugBase["legalStatus"] = drugObj.legalStatus;
    drugBase["inDemo"] = drugObj.AvailableInDemo;
    drugBase["baseAddictiveness"] = drugObj.BaseAddictiveness;
    drugBase["basePrice"] = drugObj.BasePrice;
    dict.Add(drugBase);
}


GUIUtility.systemCopyBuffer = JsonConvert.SerializeObject(dict,settings);





