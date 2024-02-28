﻿using ASCOM.NodeMCUFocuser.Focuser;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ASCOM.NodeMCUFocuser.Focuser
{
    public static class FocuserFactory
    {
        public static ICustomFocuser GetFocuser(FocuserSettings settings)
        {
            if(settings.UseComPort)
            {
                return new ComFocuser(settings.PortName, settings.MaxStepsCount);
            } else if (settings.UseServer) {
                return new ServerFocuser(settings.ServerAddress, settings.MaxStepsCount);
            } else
            {
                return null;
            }
        }
    }
}
