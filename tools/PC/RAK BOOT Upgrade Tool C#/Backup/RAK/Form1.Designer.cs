namespace RAK
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.buttonOpenserial = new System.Windows.Forms.Button();
            this.comboBaudrate = new System.Windows.Forms.ComboBox();
            this.comboPortName = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.comboBoxmodulestyle = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.checkBoxAll = new System.Windows.Forms.CheckBox();
            this.checkBoxwlan = new System.Windows.Forms.CheckBox();
            this.checkBoxmcu = new System.Windows.Forms.CheckBox();
            this.checkBox415_web_u_ch = new System.Windows.Forms.CheckBox();
            this.checkBox415_web_u_en = new System.Windows.Forms.CheckBox();
            this.checkBox411_web_u_en = new System.Windows.Forms.CheckBox();
            this.checkBox411_web_u_ch = new System.Windows.Forms.CheckBox();
            this.checkBox415_config = new System.Windows.Forms.CheckBox();
            this.checkBox411_config = new System.Windows.Forms.CheckBox();
            this.checkBox415_web_d_ch = new System.Windows.Forms.CheckBox();
            this.checkBox415_web_d_en = new System.Windows.Forms.CheckBox();
            this.checkBoxrftest = new System.Windows.Forms.CheckBox();
            this.checkBoxgdbin = new System.Windows.Forms.CheckBox();
            this.buttonsetflash = new System.Windows.Forms.Button();
            this.buttonsetstyle = new System.Windows.Forms.Button();
            this.groupBoxPlay = new System.Windows.Forms.GroupBox();
            this.textBoxPlay = new System.Windows.Forms.RichTextBox();
            this.linkLabel1 = new System.Windows.Forms.LinkLabel();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBoxPlay.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.BackColor = System.Drawing.Color.Transparent;
            this.groupBox1.Controls.Add(this.buttonOpenserial);
            this.groupBox1.Controls.Add(this.comboBaudrate);
            this.groupBox1.Controls.Add(this.comboPortName);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(3, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(188, 159);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "串口参数设置";
            // 
            // buttonOpenserial
            // 
            this.buttonOpenserial.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.buttonOpenserial.Location = new System.Drawing.Point(112, 125);
            this.buttonOpenserial.Name = "buttonOpenserial";
            this.buttonOpenserial.Size = new System.Drawing.Size(67, 26);
            this.buttonOpenserial.TabIndex = 4;
            this.buttonOpenserial.Text = "打开";
            this.buttonOpenserial.UseVisualStyleBackColor = true;
            this.buttonOpenserial.Click += new System.EventHandler(this.buttonOpenserial_Click);
            // 
            // comboBaudrate
            // 
            this.comboBaudrate.FormattingEnabled = true;
            this.comboBaudrate.Items.AddRange(new object[] {
            "9600",
            "19200",
            "38400",
            "57600",
            "115200",
            "230400",
            "460800",
            "921600"});
            this.comboBaudrate.Location = new System.Drawing.Point(85, 79);
            this.comboBaudrate.Name = "comboBaudrate";
            this.comboBaudrate.Size = new System.Drawing.Size(94, 20);
            this.comboBaudrate.TabIndex = 3;
            // 
            // comboPortName
            // 
            this.comboPortName.FormattingEnabled = true;
            this.comboPortName.Location = new System.Drawing.Point(85, 30);
            this.comboPortName.Name = "comboPortName";
            this.comboPortName.Size = new System.Drawing.Size(94, 20);
            this.comboPortName.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label1.Location = new System.Drawing.Point(18, 33);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "端口号";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.buttonsetflash);
            this.groupBox2.Controls.Add(this.checkBoxgdbin);
            this.groupBox2.Controls.Add(this.checkBoxrftest);
            this.groupBox2.Controls.Add(this.checkBox415_web_d_en);
            this.groupBox2.Controls.Add(this.checkBox415_web_d_ch);
            this.groupBox2.Controls.Add(this.checkBox411_config);
            this.groupBox2.Controls.Add(this.checkBox415_config);
            this.groupBox2.Controls.Add(this.checkBox411_web_u_ch);
            this.groupBox2.Controls.Add(this.checkBox411_web_u_en);
            this.groupBox2.Controls.Add(this.checkBox415_web_u_en);
            this.groupBox2.Controls.Add(this.checkBox415_web_u_ch);
            this.groupBox2.Controls.Add(this.checkBoxmcu);
            this.groupBox2.Controls.Add(this.checkBoxwlan);
            this.groupBox2.Controls.Add(this.checkBoxAll);
            this.groupBox2.Location = new System.Drawing.Point(213, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(341, 159);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "设置模块Flash更新位置";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.buttonsetstyle);
            this.groupBox3.Controls.Add(this.label3);
            this.groupBox3.Controls.Add(this.comboBoxmodulestyle);
            this.groupBox3.Location = new System.Drawing.Point(213, 186);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(341, 58);
            this.groupBox3.TabIndex = 3;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "设置模块类型";
            // 
            // comboBoxmodulestyle
            // 
            this.comboBoxmodulestyle.FormattingEnabled = true;
            this.comboBoxmodulestyle.Items.AddRange(new object[] {
            "RAK415",
            "RAK411 UART",
            "RAK411 SPI"});
            this.comboBoxmodulestyle.Location = new System.Drawing.Point(116, 25);
            this.comboBoxmodulestyle.Name = "comboBoxmodulestyle";
            this.comboBoxmodulestyle.Size = new System.Drawing.Size(96, 20);
            this.comboBoxmodulestyle.TabIndex = 4;
            this.comboBoxmodulestyle.Text = "RAK415";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label2.Location = new System.Drawing.Point(18, 82);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 12);
            this.label2.TabIndex = 1;
            this.label2.Text = "波特率";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label3.Location = new System.Drawing.Point(13, 28);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 12);
            this.label3.TabIndex = 5;
            this.label3.Text = "模块型号";
            // 
            // checkBoxAll
            // 
            this.checkBoxAll.AutoSize = true;
            this.checkBoxAll.Location = new System.Drawing.Point(15, 29);
            this.checkBoxAll.Name = "checkBoxAll";
            this.checkBoxAll.Size = new System.Drawing.Size(42, 16);
            this.checkBoxAll.TabIndex = 4;
            this.checkBoxAll.Text = "all";
            this.checkBoxAll.UseVisualStyleBackColor = true;
            this.checkBoxAll.CheckedChanged += new System.EventHandler(this.checkBoxAll_CheckedChanged);
            // 
            // checkBoxwlan
            // 
            this.checkBoxwlan.AutoSize = true;
            this.checkBoxwlan.Location = new System.Drawing.Point(15, 55);
            this.checkBoxwlan.Name = "checkBoxwlan";
            this.checkBoxwlan.Size = new System.Drawing.Size(48, 16);
            this.checkBoxwlan.TabIndex = 5;
            this.checkBoxwlan.Text = "wlan";
            this.checkBoxwlan.UseVisualStyleBackColor = true;
            // 
            // checkBoxmcu
            // 
            this.checkBoxmcu.AutoSize = true;
            this.checkBoxmcu.Location = new System.Drawing.Point(15, 83);
            this.checkBoxmcu.Name = "checkBoxmcu";
            this.checkBoxmcu.Size = new System.Drawing.Size(42, 16);
            this.checkBoxmcu.TabIndex = 6;
            this.checkBoxmcu.Text = "mcu";
            this.checkBoxmcu.UseVisualStyleBackColor = true;
            // 
            // checkBox415_web_u_ch
            // 
            this.checkBox415_web_u_ch.AutoSize = true;
            this.checkBox415_web_u_ch.Location = new System.Drawing.Point(116, 29);
            this.checkBox415_web_u_ch.Name = "checkBox415_web_u_ch";
            this.checkBox415_web_u_ch.Size = new System.Drawing.Size(96, 16);
            this.checkBox415_web_u_ch.TabIndex = 7;
            this.checkBox415_web_u_ch.Text = "415_web_u_ch";
            this.checkBox415_web_u_ch.UseVisualStyleBackColor = true;
            // 
            // checkBox415_web_u_en
            // 
            this.checkBox415_web_u_en.AutoSize = true;
            this.checkBox415_web_u_en.Location = new System.Drawing.Point(116, 55);
            this.checkBox415_web_u_en.Name = "checkBox415_web_u_en";
            this.checkBox415_web_u_en.Size = new System.Drawing.Size(96, 16);
            this.checkBox415_web_u_en.TabIndex = 8;
            this.checkBox415_web_u_en.Text = "415_web_u_en";
            this.checkBox415_web_u_en.UseVisualStyleBackColor = true;
            // 
            // checkBox411_web_u_en
            // 
            this.checkBox411_web_u_en.AutoSize = true;
            this.checkBox411_web_u_en.Location = new System.Drawing.Point(239, 55);
            this.checkBox411_web_u_en.Name = "checkBox411_web_u_en";
            this.checkBox411_web_u_en.Size = new System.Drawing.Size(96, 16);
            this.checkBox411_web_u_en.TabIndex = 9;
            this.checkBox411_web_u_en.Text = "411_web_u_en";
            this.checkBox411_web_u_en.UseVisualStyleBackColor = true;
            // 
            // checkBox411_web_u_ch
            // 
            this.checkBox411_web_u_ch.AutoSize = true;
            this.checkBox411_web_u_ch.Location = new System.Drawing.Point(239, 29);
            this.checkBox411_web_u_ch.Name = "checkBox411_web_u_ch";
            this.checkBox411_web_u_ch.Size = new System.Drawing.Size(96, 16);
            this.checkBox411_web_u_ch.TabIndex = 10;
            this.checkBox411_web_u_ch.Text = "411_web_u_ch";
            this.checkBox411_web_u_ch.UseVisualStyleBackColor = true;
            // 
            // checkBox415_config
            // 
            this.checkBox415_config.AutoSize = true;
            this.checkBox415_config.Location = new System.Drawing.Point(116, 83);
            this.checkBox415_config.Name = "checkBox415_config";
            this.checkBox415_config.Size = new System.Drawing.Size(84, 16);
            this.checkBox415_config.TabIndex = 11;
            this.checkBox415_config.Text = "415_config";
            this.checkBox415_config.UseVisualStyleBackColor = true;
            // 
            // checkBox411_config
            // 
            this.checkBox411_config.AutoSize = true;
            this.checkBox411_config.Location = new System.Drawing.Point(239, 83);
            this.checkBox411_config.Name = "checkBox411_config";
            this.checkBox411_config.Size = new System.Drawing.Size(84, 16);
            this.checkBox411_config.TabIndex = 12;
            this.checkBox411_config.Text = "411_config";
            this.checkBox411_config.UseVisualStyleBackColor = true;
            // 
            // checkBox415_web_d_ch
            // 
            this.checkBox415_web_d_ch.AutoSize = true;
            this.checkBox415_web_d_ch.Location = new System.Drawing.Point(116, 109);
            this.checkBox415_web_d_ch.Name = "checkBox415_web_d_ch";
            this.checkBox415_web_d_ch.Size = new System.Drawing.Size(96, 16);
            this.checkBox415_web_d_ch.TabIndex = 13;
            this.checkBox415_web_d_ch.Text = "415_web_d_ch";
            this.checkBox415_web_d_ch.UseVisualStyleBackColor = true;
            // 
            // checkBox415_web_d_en
            // 
            this.checkBox415_web_d_en.AutoSize = true;
            this.checkBox415_web_d_en.Location = new System.Drawing.Point(116, 135);
            this.checkBox415_web_d_en.Name = "checkBox415_web_d_en";
            this.checkBox415_web_d_en.Size = new System.Drawing.Size(96, 16);
            this.checkBox415_web_d_en.TabIndex = 14;
            this.checkBox415_web_d_en.Text = "415_web_d_en";
            this.checkBox415_web_d_en.UseVisualStyleBackColor = true;
            // 
            // checkBoxrftest
            // 
            this.checkBoxrftest.AutoSize = true;
            this.checkBoxrftest.Location = new System.Drawing.Point(15, 109);
            this.checkBoxrftest.Name = "checkBoxrftest";
            this.checkBoxrftest.Size = new System.Drawing.Size(60, 16);
            this.checkBoxrftest.TabIndex = 15;
            this.checkBoxrftest.Text = "rftest";
            this.checkBoxrftest.UseVisualStyleBackColor = true;
            // 
            // checkBoxgdbin
            // 
            this.checkBoxgdbin.AutoSize = true;
            this.checkBoxgdbin.Location = new System.Drawing.Point(15, 135);
            this.checkBoxgdbin.Name = "checkBoxgdbin";
            this.checkBoxgdbin.Size = new System.Drawing.Size(54, 16);
            this.checkBoxgdbin.TabIndex = 16;
            this.checkBoxgdbin.Text = "gdbin";
            this.checkBoxgdbin.UseVisualStyleBackColor = true;
            // 
            // buttonsetflash
            // 
            this.buttonsetflash.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.buttonsetflash.Location = new System.Drawing.Point(268, 125);
            this.buttonsetflash.Name = "buttonsetflash";
            this.buttonsetflash.Size = new System.Drawing.Size(67, 26);
            this.buttonsetflash.TabIndex = 17;
            this.buttonsetflash.Text = "设置";
            this.buttonsetflash.UseVisualStyleBackColor = true;
            this.buttonsetflash.Click += new System.EventHandler(this.buttonsetflash_Click);
            // 
            // buttonsetstyle
            // 
            this.buttonsetstyle.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.buttonsetstyle.Location = new System.Drawing.Point(268, 21);
            this.buttonsetstyle.Name = "buttonsetstyle";
            this.buttonsetstyle.Size = new System.Drawing.Size(67, 26);
            this.buttonsetstyle.TabIndex = 18;
            this.buttonsetstyle.Text = "设置";
            this.buttonsetstyle.UseVisualStyleBackColor = true;
            this.buttonsetstyle.Click += new System.EventHandler(this.buttonsetstyle_Click);
            // 
            // groupBoxPlay
            // 
            this.groupBoxPlay.BackColor = System.Drawing.Color.Transparent;
            this.groupBoxPlay.Controls.Add(this.textBoxPlay);
            this.groupBoxPlay.Controls.Add(this.linkLabel1);
            this.groupBoxPlay.Location = new System.Drawing.Point(12, 250);
            this.groupBoxPlay.Name = "groupBoxPlay";
            this.groupBoxPlay.Size = new System.Drawing.Size(542, 144);
            this.groupBoxPlay.TabIndex = 42;
            this.groupBoxPlay.TabStop = false;
            this.groupBoxPlay.Text = "显示信息";
            // 
            // textBoxPlay
            // 
            this.textBoxPlay.HideSelection = false;
            this.textBoxPlay.Location = new System.Drawing.Point(6, 15);
            this.textBoxPlay.Name = "textBoxPlay";
            this.textBoxPlay.ReadOnly = true;
            this.textBoxPlay.Size = new System.Drawing.Size(530, 123);
            this.textBoxPlay.TabIndex = 7;
            this.textBoxPlay.Text = "";
            // 
            // linkLabel1
            // 
            this.linkLabel1.AutoSize = true;
            this.linkLabel1.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.linkLabel1.Location = new System.Drawing.Point(501, 0);
            this.linkLabel1.Name = "linkLabel1";
            this.linkLabel1.Size = new System.Drawing.Size(35, 12);
            this.linkLabel1.TabIndex = 6;
            this.linkLabel1.TabStop = true;
            this.linkLabel1.Text = "Clear";
            this.linkLabel1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabel1_LinkClicked);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(566, 406);
            this.Controls.Add(this.groupBoxPlay);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "RAK生产命令配置工具";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBoxPlay.ResumeLayout(false);
            this.groupBoxPlay.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button buttonOpenserial;
        private System.Windows.Forms.ComboBox comboBaudrate;
        private System.Windows.Forms.ComboBox comboPortName;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox comboBoxmodulestyle;
        private System.Windows.Forms.CheckBox checkBoxrftest;
        private System.Windows.Forms.CheckBox checkBox415_web_d_en;
        private System.Windows.Forms.CheckBox checkBox415_web_d_ch;
        private System.Windows.Forms.CheckBox checkBox411_config;
        private System.Windows.Forms.CheckBox checkBox415_config;
        private System.Windows.Forms.CheckBox checkBox411_web_u_ch;
        private System.Windows.Forms.CheckBox checkBox411_web_u_en;
        private System.Windows.Forms.CheckBox checkBox415_web_u_en;
        private System.Windows.Forms.CheckBox checkBox415_web_u_ch;
        private System.Windows.Forms.CheckBox checkBoxmcu;
        private System.Windows.Forms.CheckBox checkBoxwlan;
        private System.Windows.Forms.CheckBox checkBoxAll;
        private System.Windows.Forms.CheckBox checkBoxgdbin;
        private System.Windows.Forms.Button buttonsetflash;
        private System.Windows.Forms.Button buttonsetstyle;
        private System.Windows.Forms.GroupBox groupBoxPlay;
        private System.Windows.Forms.RichTextBox textBoxPlay;
        private System.Windows.Forms.LinkLabel linkLabel1;
    }
}

