using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Threading.Tasks;

namespace NodeMCUFocuserSettings
{
    public static class JsonSerializer
    {
        public static string Serialize<T>(T Obj)
        {
            using (var ms = new MemoryStream())
            {
                DataContractJsonSerializer serialiser = new DataContractJsonSerializer(typeof(T));
                serialiser.WriteObject(ms, Obj);
                byte[] json = ms.ToArray();
                return Encoding.UTF8.GetString(json, 0, json.Length);
            }
        }

        public static T Deserialize<T>(string Json)
        {
            using (var ms = new MemoryStream(Encoding.UTF8.GetBytes(Json)))
            {
                DataContractJsonSerializer serialiser = new DataContractJsonSerializer(typeof(T));
                var deserializedObj = (T)serialiser.ReadObject(ms);
                return deserializedObj;
            }
        }
    }
}
