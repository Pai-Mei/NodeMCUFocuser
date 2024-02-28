using ASCOM.LocalServer;
using System;
using System.Collections.Generic;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Web;
using System.Web.Script.Serialization;


namespace ASCOM.NodeMCUFocuser.Focuser
{
    public class ServerFocuser : ICustomFocuser
    {
        public bool IsMoving { get; set; }

        public int Position { get; set; }

        public int MaxPosition { get; set; }

        private String Root { get; set; }

        private System.Timers.Timer Timer { get; set; }
        public ServerFocuser(String serverAddress, int maxStepsCount)
        {
            MaxPosition = maxStepsCount;
            Root = serverAddress;
            Timer = new System.Timers.Timer(500);
            Timer.Enabled = true;
            Timer.AutoReset = true;
            Timer.Elapsed += Timer_Elapsed;
        }

        private void Timer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            CheckConnect();
        }

        public bool Connect()
        {
            if(CheckConnect())
            {
                SetMaxPosition();
                return true;
            }
            else
                return false; 
        }

        public bool IsConnected
        {
            get
            {
                return CheckConnect();
            }
        }

        public void Close()
        {

        }

        public void Dispose()
        {
            Close();
        }

        public void Move(int position, bool absolute)
        {
            if(absolute)
                Send("moveto=" + position.ToString());
            else
                Send("move=" + position.ToString());
        }

        public void Stop()
        {
            Send("stop=1");
        }

        public void SetMaxPosition()
        {
            Send("set_max_position=" + MaxPosition.ToString());
        }

        private void Send(String query = null)
        {
            UriBuilder uriBuilder = new UriBuilder();
            uriBuilder.Scheme = "http";
            uriBuilder.Host = Root;
            uriBuilder.Path = "";            
            uriBuilder.Query = query;
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(uriBuilder.ToString());
            request.Method = "POST";
            request.ContentType = "application/x-www-form-urlencoded";            
            WebResponse response = request.GetResponse();
            var responseString = new StreamReader(response.GetResponseStream()).ReadToEnd();
            var serializer = new JavaScriptSerializer();
            dynamic obj = serializer.DeserializeObject(responseString);
            IsMoving = Convert.ToBoolean(obj["moving"]);
            Position = Convert.ToInt32(obj["position"]);
        }

        private bool CheckConnect()
        {
            try
            {
                Send();
                return true;
            }
            catch
            {
                return false;
            }
        }
    }
}
