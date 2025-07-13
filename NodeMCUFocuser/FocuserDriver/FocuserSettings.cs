using ASCOM.NodeMCUFocuser.Device;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ASCOM.NodeMCUFocuser.Focuser
{
    public class FocuserSettings : DeviceSettings
    {
        public bool Absolute { get; set; }
        public int MaxIncrement { get; set; }
        public int MaxStepsCount { get; set; }

        public FocuserSettings()
        {
            MaxIncrement = 100000;
            MaxStepsCount = 100000;
        }
    }
}
