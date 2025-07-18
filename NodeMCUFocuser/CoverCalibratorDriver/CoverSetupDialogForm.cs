using ASCOM.NodeMCUFocuser.CoverCalibrator;
using ASCOM.NodeMCUFocuser.Focuser;
using ASCOM.Utilities;
using System;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace ASCOM.NodeMCUFocuser.Device
{
    [ComVisible(false)] // Form not registered for COM!
    public partial class CoverSetupDialogForm : Form
    {
        const string NO_PORTS_MESSAGE = "No COM ports found";
        TraceLogger tl; // Holder for a reference to the driver's trace logger

        public CoverSetupDialogForm(TraceLogger tlDriver)
        {
            InitializeComponent();

            // Save the provided trace logger for use within the setup dialogue
            tl = tlDriver;

            // Initialise current values of user settings from the ASCOM Profile
            InitUI();
        }

        private void CmdOK_Click(object sender, EventArgs e) // OK button event handler
        {
            // Place any validation constraint checks here and update the state variables with results from the dialogue
            tl.Enabled = chkTrace.Checked;
            CoverCalibratorHardware.Settings.UseComPort = rbComPort.Checked;
            if (!(comboBoxComPort.SelectedItem is null) && comboBoxComPort.SelectedItem.ToString() != NO_PORTS_MESSAGE)
                CoverCalibratorHardware.Settings.PortName = comboBoxComPort.SelectedItem.ToString();
            CoverCalibratorHardware.Settings.UseServer = rbServer.Checked;
            CoverCalibratorHardware.Settings.ServerAddress = textBoxServerAddress.Text;
            CoverCalibratorHardware.WriteProfile();
        }

        private void CmdCancel_Click(object sender, EventArgs e) // Cancel button event handler
        {
            Close();
        }

        private void BrowseToAscom(object sender, EventArgs e) // Click on ASCOM logo event handler
        {
            try
            {
                System.Diagnostics.Process.Start("https://ascom-standards.org/");
            }
            catch (Win32Exception noBrowser)
            {
                if (noBrowser.ErrorCode == -2147467259)
                    MessageBox.Show(noBrowser.Message);
            }
            catch (Exception other)
            {
                MessageBox.Show(other.Message);
            }
        }

        private void InitUI()
        {

            // Set the trace checkbox
            chkTrace.Checked = tl.Enabled;

            // set the list of COM ports to those that are currently available
            comboBoxComPort.Items.Clear(); // Clear any existing entries
            using (Serial serial = new Serial()) // User the Serial component to get an extended list of COM ports
            {
                comboBoxComPort.Items.AddRange(serial.AvailableCOMPorts);
            }
            // If no ports are found include a message to this effect
            if (comboBoxComPort.Items.Count == 0)
            {
                comboBoxComPort.Items.Add(NO_PORTS_MESSAGE);
                comboBoxComPort.SelectedItem = NO_PORTS_MESSAGE;
            }
            // select the current port if possible
            if (comboBoxComPort.Items.Contains(CoverCalibratorHardware.Settings.PortName))
            {
                comboBoxComPort.SelectedItem = CoverCalibratorHardware.Settings.PortName;
            }

            rbComPort.Checked = CoverCalibratorHardware.Settings.UseComPort;
            rbServer.Checked = CoverCalibratorHardware.Settings.UseServer;
            textBoxServerAddress.Text = CoverCalibratorHardware.Settings.ServerAddress;

            tl.LogMessage("InitUI", $"Set UI controls to Trace: {chkTrace.Checked}");
        }

        private void SetupDialogForm_Load(object sender, EventArgs e)
        {
            // Bring the setup dialogue to the front of the screen
            if (WindowState == FormWindowState.Minimized)
                WindowState = FormWindowState.Normal;
            else
            {
                TopMost = true;
                Focus();
                BringToFront();
                TopMost = false;
            }
        }
    }
}