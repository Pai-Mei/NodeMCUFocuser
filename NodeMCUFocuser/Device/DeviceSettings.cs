using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ASCOM.NodeMCUFocuser.Device
{
    public class DeviceSettings
    {
        public bool UseComPort { get; set; }
        public String PortName { get; set; }
        public bool UseServer { get; set; }
        public String ServerAddress { get; set; }
    }
}
