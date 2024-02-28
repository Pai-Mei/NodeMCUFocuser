namespace NodeMCUFocuserSettings
{
    partial class FormSettings
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.buttonConnect = new System.Windows.Forms.Button();
            this.comboBoxComPort = new System.Windows.Forms.ComboBox();
            this.labelComPort = new System.Windows.Forms.Label();
            this.groupBoxSettings = new System.Windows.Forms.GroupBox();
            this.buttonSave = new System.Windows.Forms.Button();
            this.labelSubnet = new System.Windows.Forms.Label();
            this.labelGateway = new System.Windows.Forms.Label();
            this.labelIPAddress = new System.Windows.Forms.Label();
            this.labelWiFiPassword = new System.Windows.Forms.Label();
            this.textBoxWiFiPassword = new System.Windows.Forms.TextBox();
            this.textBoxWiFiSSID = new System.Windows.Forms.TextBox();
            this.labelWiFiSSID = new System.Windows.Forms.Label();
            this.checkBoxAccessPointEnable = new System.Windows.Forms.CheckBox();
            this.checkBoxServerEnabled = new System.Windows.Forms.CheckBox();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.maskedTextBoxIPAddress = new System.Windows.Forms.MaskedTextBox();
            this.maskedTextBoxGateway = new System.Windows.Forms.MaskedTextBox();
            this.maskedTextBoxSubnet = new System.Windows.Forms.MaskedTextBox();
            this.groupBoxSettings.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonConnect
            // 
            this.buttonConnect.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonConnect.Location = new System.Drawing.Point(195, 12);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(75, 23);
            this.buttonConnect.TabIndex = 0;
            this.buttonConnect.Text = "Connect";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // comboBoxComPort
            // 
            this.comboBoxComPort.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxComPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxComPort.FormattingEnabled = true;
            this.comboBoxComPort.Location = new System.Drawing.Point(73, 14);
            this.comboBoxComPort.Name = "comboBoxComPort";
            this.comboBoxComPort.Size = new System.Drawing.Size(116, 21);
            this.comboBoxComPort.TabIndex = 1;
            // 
            // labelComPort
            // 
            this.labelComPort.AutoSize = true;
            this.labelComPort.Location = new System.Drawing.Point(12, 17);
            this.labelComPort.Name = "labelComPort";
            this.labelComPort.Size = new System.Drawing.Size(55, 13);
            this.labelComPort.TabIndex = 2;
            this.labelComPort.Text = "COM port:";
            // 
            // groupBoxSettings
            // 
            this.groupBoxSettings.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBoxSettings.Controls.Add(this.maskedTextBoxSubnet);
            this.groupBoxSettings.Controls.Add(this.maskedTextBoxGateway);
            this.groupBoxSettings.Controls.Add(this.maskedTextBoxIPAddress);
            this.groupBoxSettings.Controls.Add(this.buttonSave);
            this.groupBoxSettings.Controls.Add(this.labelSubnet);
            this.groupBoxSettings.Controls.Add(this.labelGateway);
            this.groupBoxSettings.Controls.Add(this.labelIPAddress);
            this.groupBoxSettings.Controls.Add(this.labelWiFiPassword);
            this.groupBoxSettings.Controls.Add(this.textBoxWiFiPassword);
            this.groupBoxSettings.Controls.Add(this.textBoxWiFiSSID);
            this.groupBoxSettings.Controls.Add(this.labelWiFiSSID);
            this.groupBoxSettings.Controls.Add(this.checkBoxAccessPointEnable);
            this.groupBoxSettings.Controls.Add(this.checkBoxServerEnabled);
            this.groupBoxSettings.Location = new System.Drawing.Point(13, 41);
            this.groupBoxSettings.Name = "groupBoxSettings";
            this.groupBoxSettings.Size = new System.Drawing.Size(257, 224);
            this.groupBoxSettings.TabIndex = 3;
            this.groupBoxSettings.TabStop = false;
            this.groupBoxSettings.Text = "Settings";
            // 
            // buttonSave
            // 
            this.buttonSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonSave.Location = new System.Drawing.Point(151, 195);
            this.buttonSave.Name = "buttonSave";
            this.buttonSave.Size = new System.Drawing.Size(100, 23);
            this.buttonSave.TabIndex = 6;
            this.buttonSave.Text = "Save settings";
            this.buttonSave.UseVisualStyleBackColor = true;
            this.buttonSave.Click += new System.EventHandler(this.buttonSave_Click);
            // 
            // labelSubnet
            // 
            this.labelSubnet.AutoSize = true;
            this.labelSubnet.Location = new System.Drawing.Point(6, 166);
            this.labelSubnet.Name = "labelSubnet";
            this.labelSubnet.Size = new System.Drawing.Size(44, 13);
            this.labelSubnet.TabIndex = 4;
            this.labelSubnet.Text = "Subnet:";
            // 
            // labelGateway
            // 
            this.labelGateway.AutoSize = true;
            this.labelGateway.Location = new System.Drawing.Point(6, 140);
            this.labelGateway.Name = "labelGateway";
            this.labelGateway.Size = new System.Drawing.Size(52, 13);
            this.labelGateway.TabIndex = 4;
            this.labelGateway.Text = "Gateway:";
            // 
            // labelIPAddress
            // 
            this.labelIPAddress.AutoSize = true;
            this.labelIPAddress.Location = new System.Drawing.Point(6, 114);
            this.labelIPAddress.Name = "labelIPAddress";
            this.labelIPAddress.Size = new System.Drawing.Size(60, 13);
            this.labelIPAddress.TabIndex = 4;
            this.labelIPAddress.Text = "IP address:";
            // 
            // labelWiFiPassword
            // 
            this.labelWiFiPassword.AutoSize = true;
            this.labelWiFiPassword.Location = new System.Drawing.Point(6, 94);
            this.labelWiFiPassword.Name = "labelWiFiPassword";
            this.labelWiFiPassword.Size = new System.Drawing.Size(80, 13);
            this.labelWiFiPassword.TabIndex = 4;
            this.labelWiFiPassword.Text = "WiFi Password:";
            // 
            // textBoxWiFiPassword
            // 
            this.textBoxWiFiPassword.Location = new System.Drawing.Point(151, 91);
            this.textBoxWiFiPassword.MaxLength = 64;
            this.textBoxWiFiPassword.Name = "textBoxWiFiPassword";
            this.textBoxWiFiPassword.Size = new System.Drawing.Size(100, 20);
            this.textBoxWiFiPassword.TabIndex = 3;
            // 
            // textBoxWiFiSSID
            // 
            this.textBoxWiFiSSID.Location = new System.Drawing.Point(151, 65);
            this.textBoxWiFiSSID.MaxLength = 64;
            this.textBoxWiFiSSID.Name = "textBoxWiFiSSID";
            this.textBoxWiFiSSID.Size = new System.Drawing.Size(100, 20);
            this.textBoxWiFiSSID.TabIndex = 3;
            // 
            // labelWiFiSSID
            // 
            this.labelWiFiSSID.AutoSize = true;
            this.labelWiFiSSID.Location = new System.Drawing.Point(6, 68);
            this.labelWiFiSSID.Name = "labelWiFiSSID";
            this.labelWiFiSSID.Size = new System.Drawing.Size(59, 13);
            this.labelWiFiSSID.TabIndex = 2;
            this.labelWiFiSSID.Text = "WiFi SSID:";
            // 
            // checkBoxAccessPointEnable
            // 
            this.checkBoxAccessPointEnable.AutoSize = true;
            this.checkBoxAccessPointEnable.Location = new System.Drawing.Point(6, 42);
            this.checkBoxAccessPointEnable.Name = "checkBoxAccessPointEnable";
            this.checkBoxAccessPointEnable.Size = new System.Drawing.Size(122, 17);
            this.checkBoxAccessPointEnable.TabIndex = 1;
            this.checkBoxAccessPointEnable.Text = "Enable access point";
            this.checkBoxAccessPointEnable.UseVisualStyleBackColor = true;
            // 
            // checkBoxServerEnabled
            // 
            this.checkBoxServerEnabled.AutoSize = true;
            this.checkBoxServerEnabled.Location = new System.Drawing.Point(6, 19);
            this.checkBoxServerEnabled.Name = "checkBoxServerEnabled";
            this.checkBoxServerEnabled.Size = new System.Drawing.Size(88, 17);
            this.checkBoxServerEnabled.TabIndex = 0;
            this.checkBoxServerEnabled.Text = "Enable sever";
            this.checkBoxServerEnabled.UseVisualStyleBackColor = true;
            // 
            // maskedTextBoxIPAddress
            // 
            this.maskedTextBoxIPAddress.Culture = new System.Globalization.CultureInfo("en-US");
            this.maskedTextBoxIPAddress.Location = new System.Drawing.Point(151, 117);
            this.maskedTextBoxIPAddress.Mask = "000.000.000.000";
            this.maskedTextBoxIPAddress.Name = "maskedTextBoxIPAddress";
            this.maskedTextBoxIPAddress.Size = new System.Drawing.Size(100, 20);
            this.maskedTextBoxIPAddress.TabIndex = 7;
            // 
            // maskedTextBoxGateway
            // 
            this.maskedTextBoxGateway.Culture = new System.Globalization.CultureInfo("en-US");
            this.maskedTextBoxGateway.Location = new System.Drawing.Point(151, 143);
            this.maskedTextBoxGateway.Mask = "000.000.000.000";
            this.maskedTextBoxGateway.Name = "maskedTextBoxGateway";
            this.maskedTextBoxGateway.Size = new System.Drawing.Size(100, 20);
            this.maskedTextBoxGateway.TabIndex = 7;
            // 
            // maskedTextBoxSubnet
            // 
            this.maskedTextBoxSubnet.Culture = new System.Globalization.CultureInfo("en-US");
            this.maskedTextBoxSubnet.Location = new System.Drawing.Point(151, 169);
            this.maskedTextBoxSubnet.Mask = "000.000.000.000";
            this.maskedTextBoxSubnet.Name = "maskedTextBoxSubnet";
            this.maskedTextBoxSubnet.Size = new System.Drawing.Size(100, 20);
            this.maskedTextBoxSubnet.TabIndex = 7;
            // 
            // FormSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(282, 277);
            this.Controls.Add(this.groupBoxSettings);
            this.Controls.Add(this.labelComPort);
            this.Controls.Add(this.comboBoxComPort);
            this.Controls.Add(this.buttonConnect);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "FormSettings";
            this.Text = "NodeMCU Focuser Settings";
            this.Load += new System.EventHandler(this.FormSettings_Load);
            this.groupBoxSettings.ResumeLayout(false);
            this.groupBoxSettings.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.ComboBox comboBoxComPort;
        private System.Windows.Forms.Label labelComPort;
        private System.Windows.Forms.GroupBox groupBoxSettings;
        private System.Windows.Forms.CheckBox checkBoxServerEnabled;
        private System.Windows.Forms.Label labelWiFiPassword;
        private System.Windows.Forms.TextBox textBoxWiFiPassword;
        private System.Windows.Forms.TextBox textBoxWiFiSSID;
        private System.Windows.Forms.Label labelWiFiSSID;
        private System.Windows.Forms.CheckBox checkBoxAccessPointEnable;
        private System.Windows.Forms.Label labelSubnet;
        private System.Windows.Forms.Label labelGateway;
        private System.Windows.Forms.Label labelIPAddress;
        private System.Windows.Forms.Button buttonSave;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.MaskedTextBox maskedTextBoxIPAddress;
        private System.Windows.Forms.MaskedTextBox maskedTextBoxSubnet;
        private System.Windows.Forms.MaskedTextBox maskedTextBoxGateway;
    }
}

