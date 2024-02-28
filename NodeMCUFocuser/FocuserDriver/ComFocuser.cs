using ASCOM.DeviceInterface;
using ASCOM.LocalServer;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ASCOM.NodeMCUFocuser.Focuser
{
    public class ComFocuser : IDisposable, ICustomFocuser
    {       

        public bool IsMoving { get; set; }

        public int Position { get; set; }
        public int MaxPosition { get; set; }

        public ComFocuser(String comPort, int maxStepsCount)
        {
            MaxPosition = maxStepsCount;
            SharedResources.SharedSerial.PortName = comPort;
            SharedResources.SharedSerial.StopBits = Utilities.SerialStopBits.One;
            SharedResources.SharedSerial.Parity = Utilities.SerialParity.None;
            SharedResources.SharedSerial.DataBits = 8;
            SharedResources.SharedSerial.Speed = Utilities.SerialSpeed.ps9600;
            SharedResources.SharedSerial.ReceiveTimeout = 500;
            SharedResources.SharedSerial.Handshake = Utilities.SerialHandshake.None;            
        }

        public bool Connect()
        {
            SharedResources.Connected = true;
            var answer = SharedResources.SendMessage($"set_max_position {MaxPosition}");
            return answer == "yes#";
        }

        public bool IsConnected
        {
            get 
            {
                return SharedResources.Connected;
            }
        }

        public void Close()
        {
            SharedResources.Connected = false;
        }

        public void Dispose()
        {            
            Close();   
        }

        public void Move(int position, bool absolute)
        {
            if (SharedResources.Connected)
                SharedResources.SendMessage($"{(absolute ? "moveto" : "move")} {position}");             
        }

        public void Stop()
        {
            SharedResources.SendMessage($"stop");
        }
    }
}
