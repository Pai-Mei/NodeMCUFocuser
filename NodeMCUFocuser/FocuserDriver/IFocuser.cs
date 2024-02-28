using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ASCOM.NodeMCUFocuser.Focuser
{
    public interface ICustomFocuser : IDisposable
    {
        bool Connect();
        bool IsConnected { get; }
        void Close();
        void Move(int steps, bool absolute);
        void Stop();
        bool IsMoving { get; }
        int MaxPosition { get; }
        int Position { get; }
    }
}
