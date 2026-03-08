
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

var mainMixerMap = NetworkSingleton<ScheduleOne.Product.ProductManager>.Instance.WeedMixMap;
var effectsList = new List<Dictionary<string, object>>();

for (int i = 0; i < mainMixerMap.Effects.Count; i++)
{
	var mixerMapEffect = mainMixerMap.Effects[i];
	var propEffect = mixerMapEffect.Property;

	var effectDict = new Dictionary<string, object>
			{
				{ "name", propEffect.name },
				{ "Name", propEffect.Name },
				{ "ID", propEffect.ID },
				{ "addBaseValueMultiplier", propEffect.AddBaseValueMultiple },
				{ "addictiveness", propEffect.Addictiveness },
				{ "mixDir", new List<float> { propEffect.MixDirection.x, propEffect.MixDirection.y } },
				{ "mixMagnitude", propEffect.MixMagnitude },
				{ "tier", propEffect.Tier },
				{ "valueChange", propEffect.ValueChange },
				{ "valueMultiplier", propEffect.ValueMultiplier },
				{ "pos", new List<float> { mixerMapEffect.Position.x, mixerMapEffect.Position.y } },
				{ "labelColor", new List<float> { propEffect.LabelColor.r, propEffect.LabelColor.g, propEffect.LabelColor.b, propEffect.LabelColor.a } },
				{ "productColor", new List<float> { propEffect.ProductColor.r, propEffect.ProductColor.g, propEffect.ProductColor.b, propEffect.ProductColor.a } },
				{ "description", propEffect.Description },
				{ "radius", mixerMapEffect.Radius }
			};

	effectsList.Add(effectDict);
}
GUIUtility.systemCopyBuffer = JsonConvert.SerializeObject(effectsList,settings);
// Log(data);

// ScheduleOne.Properties.MixMaps.MixerMap