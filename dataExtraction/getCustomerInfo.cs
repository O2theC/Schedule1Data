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

//do above before below

var settings = new JsonSerializerSettings
{
    Converters = { new StringEnumConverter() }
};

var reg = ScheduleOne.NPCs.NPCManager.NPCRegistry;
// Log(reg);
var dict = new List<Dictionary<string, object>>();
for (var i = 0; i < reg.Count; i++)
{
    Customer customer = reg[i].GetComponent<Customer>();
    if (customer == null)
    {
        continue;
    }
    // kim was c1b11ce1-2d06-4b9a-a563-21bbd3dacfa8
    // lets see if it stays the same 
    CustomerData data = customer.customerData;
    var jsonData = new Dictionary<string, object>();
    jsonData["fullName"] = reg[i].fullName;
    jsonData["name"] = reg[i].name;
    jsonData["FirstName"] = reg[i].FirstName;
    jsonData["LastName"] = reg[i].LastName;
    jsonData["ID"] = reg[i].ID;
    jsonData["GUID"] = reg[i].GUID;

    jsonData["BaseAddiction"] = data.BaseAddiction;
    jsonData["CallPoliceChance"] = data.CallPoliceChance;
    jsonData["CanBeDirectlyApproached"] = data.CanBeDirectlyApproached;
    jsonData["DependenceMultiplier"] = data.DependenceMultiplier;
    jsonData["GuaranteeFirstSampleSuccess"] = data.GuaranteeFirstSampleSuccess;
    jsonData["MaxMutualRelationRequirement"] = data.MaxMutualRelationRequirement;
    jsonData["MaxOrdersPerWeek"] = data.MaxOrdersPerWeek;
    jsonData["MaxWeeklySpend"] = data.MaxWeeklySpend;
    jsonData["MinMutualRelationRequirement"] = data.MinMutualRelationRequirement;
    jsonData["MinOrdersPerWeek"] = data.MinOrdersPerWeek;
    jsonData["MinWeeklySpend"] = data.MinWeeklySpend;
    jsonData["OrderTime"] = data.OrderTime;
    jsonData["PreferredOrderDay"] = data.PreferredOrderDay;
    var effectList = new List<string>();
    for (int ii = 0; ii < data.PreferredProperties.Count; ii++)
    {
        effectList.Add(data.PreferredProperties[ii].name);
    }
    jsonData["PreferredProperties"] = effectList;
    jsonData["Standards"] = data.Standards.ToString();
    jsonData["CurrentAddiction"] = customer.CurrentAddiction;
    jsonData["Relationship"] = customer.NPC.RelationData.RelationDelta;
    jsonData["Unlocked"] = customer.NPC.RelationData.Unlocked;
    var connections = new List<string>();
    for (int ii = 0; ii < customer.NPC.RelationData.Connections.Count; ii++)
    {
        connections.Add(customer.NPC.RelationData.Connections[ii].fullName);
    }
    jsonData["Connections"] = connections;
    var affinities = new Dictionary<string, float>();
    for (int ii = 0; ii < customer.currentAffinityData.ProductAffinities.Count; ii++)
    {
        affinities[customer.currentAffinityData.ProductAffinities[ii].DrugType.ToString()] = customer.currentAffinityData.ProductAffinities[ii].Affinity;
    }
    jsonData["ProductAffinities"] = affinities;
    jsonData["Region"] = reg[i].Region;




    dict.Add(jsonData);
    // Log(data.name);
}


GUIUtility.systemCopyBuffer = JsonConvert.SerializeObject(dict, settings);
// Log(JsonConvert.SerializeObject(dict,settings));