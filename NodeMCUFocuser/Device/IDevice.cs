using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ASCOM.NodeMCUFocuser.Device
{ 
    public interface IDevice : IDisposable
    {
        bool Connect();
        bool IsConnected { get; }
        void Close();
    }
}
