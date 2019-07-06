namespace DSPNetwork.DGRAM.WinForm
{
    partial class MainForm
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.ContextMenuSaveAs = new System.Windows.Forms.ToolStripMenuItem();
            this.button_ImageDataFile = new System.Windows.Forms.Button();
            this.button_TaskBegin = new System.Windows.Forms.Button();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.textBox_Info = new System.Windows.Forms.RichTextBox();
            this.buttonAddUnit = new System.Windows.Forms.Button();
            this.textBoxUnit = new System.Windows.Forms.TextBox();
            this.comboBox_PackUnit = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label5 = new System.Windows.Forms.Label();
            this.comboBox_Task = new System.Windows.Forms.ComboBox();
            this.button_FileList = new System.Windows.Forms.Button();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.textBox_IPAddr = new System.Windows.Forms.TextBox();
            this.textBox_PortNum = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.button_Update = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.contextMenuStrip1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.SystemColors.ControlLight;
            this.pictureBox1.ContextMenuStrip = this.contextMenuStrip1;
            this.pictureBox1.Location = new System.Drawing.Point(4, 4);
            this.pictureBox1.Margin = new System.Windows.Forms.Padding(4);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(767, 606);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ContextMenuSaveAs});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(118, 26);
            // 
            // ContextMenuSaveAs
            // 
            this.ContextMenuSaveAs.Name = "ContextMenuSaveAs";
            this.ContextMenuSaveAs.Size = new System.Drawing.Size(117, 22);
            this.ContextMenuSaveAs.Text = "SaveAs";
            this.ContextMenuSaveAs.Click += new System.EventHandler(this.ContextMenuSaveAs_Click);
            // 
            // button_ImageDataFile
            // 
            this.button_ImageDataFile.Location = new System.Drawing.Point(7, 26);
            this.button_ImageDataFile.Margin = new System.Windows.Forms.Padding(4);
            this.button_ImageDataFile.Name = "button_ImageDataFile";
            this.button_ImageDataFile.Size = new System.Drawing.Size(116, 40);
            this.button_ImageDataFile.TabIndex = 0;
            this.button_ImageDataFile.Text = "选择数据";
            this.button_ImageDataFile.UseVisualStyleBackColor = true;
            this.button_ImageDataFile.Click += new System.EventHandler(this.button_ImageDataFile_Click);
            // 
            // button_TaskBegin
            // 
            this.button_TaskBegin.Location = new System.Drawing.Point(170, 66);
            this.button_TaskBegin.Margin = new System.Windows.Forms.Padding(4);
            this.button_TaskBegin.Name = "button_TaskBegin";
            this.button_TaskBegin.Size = new System.Drawing.Size(194, 58);
            this.button_TaskBegin.TabIndex = 1;
            this.button_TaskBegin.Text = "启动作业";
            this.button_TaskBegin.UseVisualStyleBackColor = true;
            this.button_TaskBegin.Click += new System.EventHandler(this.button_TaskBegin_Click);
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(6, 247);
            this.progressBar1.Margin = new System.Windows.Forms.Padding(4);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(383, 14);
            this.progressBar1.TabIndex = 4;
            // 
            // textBox_Info
            // 
            this.textBox_Info.BackColor = System.Drawing.SystemColors.ControlLight;
            this.textBox_Info.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox_Info.Location = new System.Drawing.Point(8, 26);
            this.textBox_Info.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_Info.Name = "textBox_Info";
            this.textBox_Info.ReadOnly = true;
            this.textBox_Info.Size = new System.Drawing.Size(380, 213);
            this.textBox_Info.TabIndex = 9;
            this.textBox_Info.TabStop = false;
            this.textBox_Info.Text = "";
            // 
            // buttonAddUnit
            // 
            this.buttonAddUnit.Location = new System.Drawing.Point(215, 27);
            this.buttonAddUnit.Name = "buttonAddUnit";
            this.buttonAddUnit.Size = new System.Drawing.Size(100, 28);
            this.buttonAddUnit.TabIndex = 8;
            this.buttonAddUnit.Text = "<自定义<";
            this.buttonAddUnit.UseVisualStyleBackColor = true;
            this.buttonAddUnit.Click += new System.EventHandler(this.buttonAddUnit_Click);
            // 
            // textBoxUnit
            // 
            this.textBoxUnit.Location = new System.Drawing.Point(329, 28);
            this.textBoxUnit.Name = "textBoxUnit";
            this.textBoxUnit.Size = new System.Drawing.Size(60, 26);
            this.textBoxUnit.TabIndex = 7;
            this.textBoxUnit.Text = "2000";
            // 
            // comboBox_PackUnit
            // 
            this.comboBox_PackUnit.FormattingEnabled = true;
            this.comboBox_PackUnit.Location = new System.Drawing.Point(136, 30);
            this.comboBox_PackUnit.Margin = new System.Windows.Forms.Padding(4);
            this.comboBox_PackUnit.Name = "comboBox_PackUnit";
            this.comboBox_PackUnit.Size = new System.Drawing.Size(67, 24);
            this.comboBox_PackUnit.Sorted = true;
            this.comboBox_PackUnit.TabIndex = 6;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(8, 34);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(104, 16);
            this.label3.TabIndex = 4;
            this.label3.Text = "数据分片大小";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.comboBox_PackUnit);
            this.groupBox2.Controls.Add(this.textBoxUnit);
            this.groupBox2.Controls.Add(this.buttonAddUnit);
            this.groupBox2.Location = new System.Drawing.Point(779, 123);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(395, 74);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "数据发送设置";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Controls.Add(this.comboBox_Task);
            this.groupBox3.Controls.Add(this.button_FileList);
            this.groupBox3.Controls.Add(this.button_ImageDataFile);
            this.groupBox3.Controls.Add(this.button_TaskBegin);
            this.groupBox3.Location = new System.Drawing.Point(779, 203);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(395, 133);
            this.groupBox3.TabIndex = 0;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "作业控制";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(142, 29);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(72, 16);
            this.label5.TabIndex = 14;
            this.label5.Text = "作业选择";
            // 
            // comboBox_Task
            // 
            this.comboBox_Task.FormattingEnabled = true;
            this.comboBox_Task.Location = new System.Drawing.Point(235, 26);
            this.comboBox_Task.Name = "comboBox_Task";
            this.comboBox_Task.Size = new System.Drawing.Size(150, 24);
            this.comboBox_Task.TabIndex = 13;
            // 
            // button_FileList
            // 
            this.button_FileList.Location = new System.Drawing.Point(7, 84);
            this.button_FileList.Name = "button_FileList";
            this.button_FileList.Size = new System.Drawing.Size(116, 40);
            this.button_FileList.TabIndex = 10;
            this.button_FileList.Text = "列表指定";
            this.button_FileList.UseVisualStyleBackColor = true;
            this.button_FileList.Click += new System.EventHandler(this.button_FileList_Click);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.progressBar1);
            this.groupBox4.Controls.Add(this.textBox_Info);
            this.groupBox4.Location = new System.Drawing.Point(779, 342);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(395, 268);
            this.groupBox4.TabIndex = 3;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "作业进度";
            // 
            // textBox_IPAddr
            // 
            this.textBox_IPAddr.Location = new System.Drawing.Point(136, 27);
            this.textBox_IPAddr.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_IPAddr.Name = "textBox_IPAddr";
            this.textBox_IPAddr.Size = new System.Drawing.Size(174, 26);
            this.textBox_IPAddr.TabIndex = 2;
            // 
            // textBox_PortNum
            // 
            this.textBox_PortNum.Location = new System.Drawing.Point(136, 65);
            this.textBox_PortNum.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_PortNum.Name = "textBox_PortNum";
            this.textBox_PortNum.Size = new System.Drawing.Size(67, 26);
            this.textBox_PortNum.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(8, 31);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(88, 16);
            this.label1.TabIndex = 2;
            this.label1.Text = "远程IP地址";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(8, 75);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(104, 16);
            this.label2.TabIndex = 8;
            this.label2.Text = "网络通信端口";
            // 
            // button_Update
            // 
            this.button_Update.Location = new System.Drawing.Point(328, 31);
            this.button_Update.Name = "button_Update";
            this.button_Update.Size = new System.Drawing.Size(59, 57);
            this.button_Update.TabIndex = 4;
            this.button_Update.Text = "更新设置";
            this.button_Update.UseVisualStyleBackColor = true;
            this.button_Update.Click += new System.EventHandler(this.button_Update_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(206, 68);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(80, 16);
            this.label4.TabIndex = 9;
            this.label4.Text = "[1,65535]";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.button_Update);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.textBox_PortNum);
            this.groupBox1.Controls.Add(this.textBox_IPAddr);
            this.groupBox1.Location = new System.Drawing.Point(779, 4);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox1.Size = new System.Drawing.Size(395, 110);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "网络设置";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1179, 616);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.pictureBox1);
            this.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Location = new System.Drawing.Point(50, 50);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.Text = "控制台 (LAN+UDP)";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.contextMenuStrip1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Button button_ImageDataFile;
        private System.Windows.Forms.Button button_TaskBegin;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.RichTextBox textBox_Info;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox comboBox_PackUnit;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem ContextMenuSaveAs;
        private System.Windows.Forms.Button buttonAddUnit;
        private System.Windows.Forms.TextBox textBoxUnit;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button button_FileList;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox comboBox_Task;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.TextBox textBox_IPAddr;
        private System.Windows.Forms.TextBox textBox_PortNum;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button button_Update;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.GroupBox groupBox1;
    }
}

