using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ASCOM.NodeMCUFocuser.Device
{
    interface ICustomCover : Device.IDevice
    {
        void OpenLid();
        void CloseLid();
        void SetLight(int level);
        int MaxBrightnes { get; }
        int Brightnes { get; }
    }
}
