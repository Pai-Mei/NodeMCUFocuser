namespace ASCOM.NodeMCUFocuser.Focuser
{
    partial class SetupDialogForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SetupDialogForm));
            this.cmdOK = new System.Windows.Forms.Button();
            this.cmdCancel = new System.Windows.Forms.Button();
            this.picASCOM = new System.Windows.Forms.PictureBox();
            this.chkTrace = new System.Windows.Forms.CheckBox();
            this.comboBoxComPort = new System.Windows.Forms.ComboBox();
            this.numMaxOffset = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.numMaxStep = new System.Windows.Forms.NumericUpDown();
            this.textBoxServerAddress = new System.Windows.Forms.TextBox();
            this.rbComPort = new System.Windows.Forms.RadioButton();
            this.rbServer = new System.Windows.Forms.RadioButton();
            this.checkBoxAbsolute = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.picASCOM)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMaxOffset)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMaxStep)).BeginInit();
            this.SuspendLayout();
            // 
            // cmdOK
            // 
            this.cmdOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cmdOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.cmdOK.Location = new System.Drawing.Point(281, 108);
            this.cmdOK.Name = "cmdOK";
            this.cmdOK.Size = new System.Drawing.Size(59, 24);
            this.cmdOK.TabIndex = 0;
            this.cmdOK.Text = "OK";
            this.cmdOK.UseVisualStyleBackColor = true;
            this.cmdOK.Click += new System.EventHandler(this.CmdOK_Click);
            // 
            // cmdCancel
            // 
            this.cmdCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cmdCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cmdCancel.Location = new System.Drawing.Point(281, 138);
            this.cmdCancel.Name = "cmdCancel";
            this.cmdCancel.Size = new System.Drawing.Size(59, 25);
            this.cmdCancel.TabIndex = 1;
            this.cmdCancel.Text = "Cancel";
            this.cmdCancel.UseVisualStyleBackColor = true;
            this.cmdCancel.Click += new System.EventHandler(this.CmdCancel_Click);
            // 
            // picASCOM
            // 
            this.picASCOM.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.picASCOM.Cursor = System.Windows.Forms.Cursors.Hand;
            this.picASCOM.Image = ((System.Drawing.Image)(resources.GetObject("picASCOM.Image")));
            this.picASCOM.Location = new System.Drawing.Point(292, 9);
            this.picASCOM.Name = "picASCOM";
            this.picASCOM.Size = new System.Drawing.Size(48, 56);
            this.picASCOM.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.picASCOM.TabIndex = 3;
            this.picASCOM.TabStop = false;
            this.picASCOM.Click += new System.EventHandler(this.BrowseToAscom);
            this.picASCOM.DoubleClick += new System.EventHandler(this.BrowseToAscom);
            // 
            // chkTrace
            // 
            this.chkTrace.AutoSize = true;
            this.chkTrace.Location = new System.Drawing.Point(93, 146);
            this.chkTrace.Name = "chkTrace";
            this.chkTrace.Size = new System.Drawing.Size(69, 17);
            this.chkTrace.TabIndex = 6;
            this.chkTrace.Text = "Trace on";
            this.chkTrace.UseVisualStyleBackColor = true;
            // 
            // comboBoxComPort
            // 
            this.comboBoxComPort.FormattingEnabled = true;
            this.comboBoxComPort.Location = new System.Drawing.Point(93, 87);
            this.comboBoxComPort.Name = "comboBoxComPort";
            this.comboBoxComPort.Size = new System.Drawing.Size(117, 21);
            this.comboBoxComPort.TabIndex = 7;
            // 
            // numMaxOffset
            // 
            this.numMaxOffset.Increment = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numMaxOffset.Location = new System.Drawing.Point(93, 38);
            this.numMaxOffset.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.numMaxOffset.Name = "numMaxOffset";
            this.numMaxOffset.Size = new System.Drawing.Size(117, 20);
            this.numMaxOffset.TabIndex = 8;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 40);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(56, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "Max offset";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 14);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(50, 13);
            this.label1.TabIndex = 11;
            this.label1.Text = "Max step";
            // 
            // numMaxStep
            // 
            this.numMaxStep.Increment = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numMaxStep.Location = new System.Drawing.Point(93, 12);
            this.numMaxStep.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.numMaxStep.Name = "numMaxStep";
            this.numMaxStep.Size = new System.Drawing.Size(117, 20);
            this.numMaxStep.TabIndex = 10;
            // 
            // textBoxServerAddress
            // 
            this.textBoxServerAddress.Location = new System.Drawing.Point(93, 115);
            this.textBoxServerAddress.Name = "textBoxServerAddress";
            this.textBoxServerAddress.Size = new System.Drawing.Size(117, 20);
            this.textBoxServerAddress.TabIndex = 15;
            // 
            // rbComPort
            // 
            this.rbComPort.AutoSize = true;
            this.rbComPort.Location = new System.Drawing.Point(15, 88);
            this.rbComPort.Name = "rbComPort";
            this.rbComPort.Size = new System.Drawing.Size(65, 17);
            this.rbComPort.TabIndex = 16;
            this.rbComPort.TabStop = true;
            this.rbComPort.Text = "ComPort";
            this.rbComPort.UseVisualStyleBackColor = true;
            // 
            // rbServer
            // 
            this.rbServer.AutoSize = true;
            this.rbServer.Location = new System.Drawing.Point(15, 116);
            this.rbServer.Name = "rbServer";
            this.rbServer.Size = new System.Drawing.Size(56, 17);
            this.rbServer.TabIndex = 17;
            this.rbServer.TabStop = true;
            this.rbServer.Text = "Server";
            this.rbServer.UseVisualStyleBackColor = true;
            // 
            // checkBoxAbsolute
            // 
            this.checkBoxAbsolute.AutoSize = true;
            this.checkBoxAbsolute.Location = new System.Drawing.Point(93, 64);
            this.checkBoxAbsolute.Name = "checkBoxAbsolute";
            this.checkBoxAbsolute.Size = new System.Drawing.Size(67, 17);
            this.checkBoxAbsolute.TabIndex = 18;
            this.checkBoxAbsolute.Text = "Absolute";
            this.checkBoxAbsolute.UseVisualStyleBackColor = true;
            // 
            // SetupDialogForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(350, 171);
            this.Controls.Add(this.checkBoxAbsolute);
            this.Controls.Add(this.rbServer);
            this.Controls.Add(this.rbComPort);
            this.Controls.Add(this.textBoxServerAddress);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.numMaxStep);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.numMaxOffset);
            this.Controls.Add(this.comboBoxComPort);
            this.Controls.Add(this.chkTrace);
            this.Controls.Add(this.picASCOM);
            this.Controls.Add(this.cmdCancel);
            this.Controls.Add(this.cmdOK);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "SetupDialogForm";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "NodeMCUFocuser Setup";
            this.Load += new System.EventHandler(this.SetupDialogForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.picASCOM)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMaxOffset)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMaxStep)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button cmdOK;
        private System.Windows.Forms.Button cmdCancel;
        private System.Windows.Forms.PictureBox picASCOM;
        private System.Windows.Forms.CheckBox chkTrace;
        private System.Windows.Forms.ComboBox comboBoxComPort;
        private System.Windows.Forms.NumericUpDown numMaxOffset;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown numMaxStep;
        private System.Windows.Forms.TextBox textBoxServerAddress;
        private System.Windows.Forms.RadioButton rbComPort;
        private System.Windows.Forms.RadioButton rbServer;
        private System.Windows.Forms.CheckBox checkBoxAbsolute;
    }
}