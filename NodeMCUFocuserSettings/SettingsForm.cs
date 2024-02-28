using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.Serialization.Json;
using System.Drawing.Text;
using System.Collections;
using System.Runtime.Remoting.Messaging;

namespace NodeMCUFocuserSettings
{
    public partial class FormSettings : Form
    {
        const String TerminationSign = "#";

        public FormSettings()
        {
            InitializeComponent();
        }

        private void FormSettings_Load(object sender, EventArgs e)
        {
            FillComPorts();
        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {
            Connect();
        }
        private void buttonSave_Click(object sender, EventArgs e)
        {
            SaveSettings();
        }

        private void FillComPorts()
        {
            comboBoxComPort.Items.Clear();
            foreach (var port in SerialPort.GetPortNames())
                comboBoxComPort.Items.Add(port);
        }

        private void Connect()
        {
            serialPort1.PortName = comboBoxComPort.Text;
            try
            {
                serialPort1.Open();
                serialPort1.WriteLine("get_settings");
                var jsonSetttings = ReadSerial(serialPort1);
                LoadSettings(jsonSetttings.Replace(TerminationSign, String.Empty));
            } finally
            {
                serialPort1.Close();
            }
        }

        private void SaveSettings()
        {
            try
            {
                serialPort1.PortName = comboBoxComPort.Text;
                serialPort1.Open();
                var settings = GetSettings();
                if (!ValidateIP(settings.IPAddress))
                {
                    MessageBox.Show("IPAddress is incorrect");
                    return;
                }
                if (!ValidateIP(settings.Gateway))
                {
                    MessageBox.Show("Gateway is incorrect");
                    return;
                }
                if (!ValidateSubnet(settings.Subnet))
                {
                    MessageBox.Show("Subnet is incorrect");
                    return;
                }
                var jsonSettings = JsonSerializer.Serialize(settings);                
                serialPort1.Write("save_settings " + jsonSettings + TerminationSign);
                var answer = ReadSerial(serialPort1);
                if (answer.StartsWith("saved"))
                    MessageBox.Show("Saved successfuly", "Status", MessageBoxButtons.OK, MessageBoxIcon.Information);
                else
                    throw new Exception("Wrong response from focuser");
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed:" + Environment.NewLine + ex.Message, "Status", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally
            {
                serialPort1.Close();
            }
        }

        private String ReadSerial(SerialPort serialPort)
        {
            var fullString = String.Empty;
            do
                fullString += serialPort1.ReadLine();
            while (!fullString.Contains(TerminationSign));
            return fullString;
        }

        private void LoadSettings(String jsonSettings)
        {
            var settings = JsonSerializer.Deserialize<Settings>(jsonSettings);
            checkBoxServerEnabled.Checked = settings.ServerEnabled;
            checkBoxAccessPointEnable.Checked = settings.AccessPointEnabled;
            textBoxWiFiSSID.Text = settings.WiFiSSID;
            textBoxWiFiPassword.Text = settings.Password;
            maskedTextBoxIPAddress.Text = NormalizeIP(settings.IPAddress);
            maskedTextBoxGateway.Text = NormalizeIP(settings.Gateway);
            maskedTextBoxSubnet.Text = NormalizeIP(settings.Subnet);
        }

        private Settings GetSettings()
        {
            var settings = new Settings();
            settings.ServerEnabled = checkBoxServerEnabled.Checked;
            settings.AccessPointEnabled = checkBoxAccessPointEnable.Checked;
            settings.WiFiSSID = textBoxWiFiSSID.Text;
            settings.Password = textBoxWiFiPassword.Text;
            settings.IPAddress = DenormalizeIP(maskedTextBoxIPAddress.Text);
            settings.Gateway = DenormalizeIP(maskedTextBoxGateway.Text);
            settings.Subnet = DenormalizeIP(maskedTextBoxSubnet.Text);
            return settings;
        }

        private String NormalizeIP(String IPstring)
        {
            return String.Join(".", IPstring.Split('.').Select(x => (new String('0', 3 - x.Length)) + x ));            
        }

        private String DenormalizeIP(String IPstring)
        {
            return String.Join(".", IPstring.Split('.').Select(x => x == "000" ? "0" : x.TrimStart('0')));
        }

        private bool ValidateIP(String IPstring)
        {
            return IPstring.Split('.').All(x => int.Parse(x) < 256);
        }

        private bool ValidateSubnet(String IPstring)
        {            
            var parts = IPstring.Split('.').Select(x => byte.Parse(x)).ToArray();
            //after 1s must be only 0s
            var bits = new BitArray(parts);
            var currentState = true;            
            for (var i = 0; i < bits.Count; i++)
            {
                if (currentState)                
                    currentState = bits[i];                
                else if (bits[i]) return false;
            }
            return true;
        }
    }
}
