using System;
using System.Collections.Generic;
using EasyButtons;
using FishNet.Object;
using ScheduleOne.ConstructableScripts;
using ScheduleOne.DevUtilities;
using ScheduleOne.Growing;
using ScheduleOne.ItemFramework;
using ScheduleOne.Levelling;
using ScheduleOne.Persistence;
using UnityEngine;
using UnityEngine.Events;





//do above and below seperately 





var registry = Singleton<ScheduleOne.Registry>.Instance;
var items = registry.ItemRegistry;
var dict = new List<Dictionary<string, object>>();
for (var i = 0; i < items.Count; i++)
{
    var name = items[i].name;
    if (name == null) {
        continue;
    }
    var itemName = name.Substring(0, name.IndexOf(" ("));
    var itemCat = name.Substring(name.IndexOf(" (") + 2, name.Length - (name.IndexOf(" (") + 3));
    if (itemCat == "Ingredient")
    {
        if (items[i].Definition is ScheduleOne.Product.PropertyItemDefinition)
        {
            //log(itemName);
            var item = items[i].Definition as ScheduleOne.Product.PropertyItemDefinition;
            var data = new Dictionary<string, object>();
            data["name"] = item.name;
            data["Name"] = item.Name;
            data["ID"] = item.ID;
            data["effects"] = new List<string>();
            for (var ii = 0; ii < item.Properties.Count; ii++)
            {
                (data["effects"] as List<string>).Add(item.Properties[ii].Name);
            }
            data["price"] = item.BasePurchasePrice;
            data["rank"] = item.RequiredRank.Tier;
            data["resellMul"] = item.ResellMultiplier;
            data["description"] = item.Description;
            data["labelColor"] = new List<float> { item.LabelDisplayColor.r, item.LabelDisplayColor.g, item.LabelDisplayColor.b, item.LabelDisplayColor.a };
            data["legalStatus"] = item.legalStatus;
            data["inDemo"] = item.AvailableInDemo;
            data["stackSize"] = item.StackLimit;
			dict.Add(data);
        }
    }
}




// Log(JsonConvert.SerializeObject(dict));
GUIUtility.systemCopyBuffer = JsonConvert.SerializeObject(dict);