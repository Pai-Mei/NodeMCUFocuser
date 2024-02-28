using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace NodeMCUFocuserSettings
{
    [DataContract]
    public class Settings
    {
        [DataMember(Name = "serverEnabled")]
        public bool ServerEnabled { get; set; }
        [DataMember(Name = "createAccessPoint")]
        public bool AccessPointEnabled { get; set; }
        [DataMember(Name = "ssid")]
        public String WiFiSSID { get; set; }
        [DataMember(Name = "pass")]
        public String Password { get; set; }
        [DataMember(Name = "localIP")]
        public String IPAddress { get; set; }
        [DataMember(Name = "gateway")]
        public String Gateway { get; set; }
        [DataMember(Name = "subnet")]
        public String Subnet { get; set; }

    }
}
