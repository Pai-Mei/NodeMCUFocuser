using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ASCOM.NodeMCUFocuser.Focuser
{
    public class FocuserSettings
    {
        public bool Absolute { get; set; }
        public int MaxIncrement { get; set; }
        public int MaxStepsCount { get; set; }
        public bool UseComPort { get; set; }
        public String PortName { get; set; }
        public bool UseServer { get; set; }
        public String ServerAddress { get; set; }

        public FocuserSettings()
        {
            MaxIncrement = 100000;
            MaxStepsCount = 100000;
        }
    }
}
