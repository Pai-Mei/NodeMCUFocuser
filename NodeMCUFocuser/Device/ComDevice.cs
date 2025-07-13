using ASCOM.DeviceInterface;
using ASCOM.LocalServer;
using ASCOM.LocalServer.Device;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ASCOM.NodeMCUFocuser.Device
{
    public class ComDevice : IDisposable, ICustomFocuser, ICustomCover, IFilterWheel
    {       

        public bool IsMoving { get; set; }
        public int Position { get; set; }
        public int MaxPosition { get; set; }


        public ComDevice(String comPort, int maxStepsCount)
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

        public int MaxBrightnes { get; set; } = 1024;

        public int Brightnes { get; set; }

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
            {
                WaitWhileMoving();
                var answer = SharedResources.SendMessage($"{(absolute ? "moveto" : "move")} {position}");
                IsMoving = true;
                WaitWhileMoving();
            }
        }

        private void WaitWhileMoving()
        {
            while (IsMoving)
            {
                var stoppedString = SharedResources.SharedSerial.ReceiveTerminated("#");
                IsMoving = !stoppedString.Contains("stoped");
            }
        }

        public void Stop()
        {
            SharedResources.SendMessage($"stop");
        }

        public void OpenLid()
        {
            SharedResources.SendMessage($"open_cover");
        }

        public void CloseLid()
        {
            SharedResources.SendMessage($"close_cover");
        }

        public void SetLight(int level)
        {
            SharedResources.SendMessage($"set_light {level}");
            Brightnes = level;
        }
    }
}
