using ASCOM.NodeMCUFocuser.Device;
using ASCOM.NodeMCUFocuser.Focuser;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ASCOM.NodeMCUFocuser.CoverCalibratorDriver
{
    internal class CoverFactory
    {
        public static ICustomCover GetCover(CoverSettings settings)
        {
            if (settings.UseComPort)
            {
                return new ComDevice(settings.PortName, 0);
            }
            else if (settings.UseServer)
            {
                return new ServerDevice(settings.ServerAddress,0);
            }
            else
            {
                return null;
            }
        }
    }
}
