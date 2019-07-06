using System;
using System.Drawing;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.IO;

namespace DSPNetwork.DGRAM.WinForm
{
    public partial class MainForm : Form
    {
        private List<string> strFileNames = new List<string>();
        private int index = 0;
        private int count = 0;
        private Bitmap bmp = null;
        private int dataSize = 0;
        private IPEndPoint targetEP = null;
        private Socket cltSock = null;
        DateTime dt = new DateTime();
        int taskIdx = 0;
        int[] job_types = new int[] { 2, 1 };

        public MainForm()
        {
            InitializeComponent();

            this.StartPosition = FormStartPosition.Manual;

            int[] pu = new int[] { 1024, 1500, 2048 };
            foreach (int u in pu)
            {
                comboBox_PackUnit.Items.Add(u);
            }

            // 作业选择
            string[] tss = new string[] { "Sobel边缘检测", "反相(颜色取补)" };
            foreach (string s in tss)
            {
                comboBox_Task.Items.Add(s);
            }
            comboBox_Task.SelectedIndex = taskIdx;

            comboBox_PackUnit.SelectedIndex = comboBox_PackUnit.Items.Count - 1;

            textBox_IPAddr.Text = "192.168.2.100";
            textBox_PortNum.Text = "7";

            textBox_Info.Text = "##请确认远端就绪后再[启动作业]##\n";
            ScrollToTextEnd();
        }

        private void button_ImageDataFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "选择图像数据(可多选)";
            ofd.Multiselect = true;
            ofd.Filter = "DAT|*.dat";
            if (DialogResult.OK == ofd.ShowDialog(this))
            {
                strFileNames.Clear();
                count = 0;
                index = 0;
                foreach (string s in ofd.FileNames)
                {
                    strFileNames.Add(s);
                    ++count;
                }

                textBox_Info.Text += "*********************************\n已选择" + count.ToString() + "个文件\n";
                ScrollToTextEnd();
            }
        }

        private void button_TaskBegin_Click(object sender, EventArgs e)
        {
            if (count == 0)
            {
                MessageBox.Show("请至少选择一个文件并打开", "提示");
                return;
            }

            // pack_info
            // {
            //    int imgWidth;
            //    int imgHeight;
            //    int imgDepth;
            //    int packUnit;
			//   int jobType;
            // }

            int unit1 = int.Parse(comboBox_PackUnit.SelectedItem.ToString());
            int unit2;
            int cycle, residue;
            int width, height, depth;
            int j_idx=comboBox_Task.SelectedIndex;
            if(j_idx<0) j_idx=0;
            int job_type = job_types[j_idx];

            int sz_int = sizeof(int);
            byte[] packi = new byte[5 * sz_int];
            byte[] tmp = new byte[sz_int];
            byte[] rbuf = new byte[2048];
            byte[] rbuf2 = new byte[] { (byte)'H', (byte)'E', (byte)'L', (byte)'L', (byte)'O', 0 };

            int i, j, k;
            for (k = 2; k < sz_int; ++k)
            {
                rbuf[k] = (byte)0;
            }
            tmp = BitConverter.GetBytes(unit1);
            k = 0;
            for (i = 3 * sz_int; i < 4 * sz_int; ++i)
            {
                packi[i] = tmp[k];
                ++k;
            }
            tmp = BitConverter.GetBytes(job_type);
            k = 0;
            for (i = 4 * sz_int; i < 5 * sz_int; ++i)
            {
                packi[i] = tmp[k];
                ++k;
            }


            try
            {
                #region INIT_SOCK
                if (cltSock == null)
                {
                    UpdateNetworkSettings();
                }
                if (!cltSock.Connected)
                {
                    cltSock.Connect(targetEP);
                    //cltSock.SendTimeout = 1000; //1 second
                    //cltSock.ReceiveTimeout = 1000; //1 second
                    if (!cltSock.Connected)
                    {
                        throw new Exception("连接失败！\n");
                    }
                }
                #endregion INIT_SOCK

                #region WHILE_LOOP
                while (index < count)
                {
                    FileStream fs = new FileStream(strFileNames[index], FileMode.Open);
                    fs.Read(packi, 0, 3 * sz_int);
                    width = BitConverter.ToInt32(packi, 0);
                    height = BitConverter.ToInt32(packi, sz_int);
                    depth = BitConverter.ToInt32(packi, 2 * sz_int);
                    dataSize = width * height * depth / 8;
                    if (fs.Length != (dataSize + 3 * sz_int))
                    {
                        throw new Exception("文件内容有误/格式不符");
                    }
                    byte[] storage1 = new byte[dataSize];
                    fs.Read(storage1, 0, dataSize);
                    fs.Close();

                    cycle = dataSize / unit1;
                    residue = dataSize - cycle * unit1;

                    dt = DateTime.Now;
                    textBox_Info.Text +="["+ (index+1).ToString()+"/"+count.ToString()+
                        "]文件{"+strFileNames[index]+"}\n数据大小(WF/Byte): " + dataSize.ToString() +
                        ", " + unit1.ToString() + "\n开始发送数据 " + 
                        dt.ToString("G") + "." + dt.Millisecond.ToString("D3") + "\n";
                    ScrollToTextEnd();
                    textBox_Info.Refresh();
                    progressBar1.Value = 0;

                    cltSock.Send(packi);
                    cltSock.Receive(rbuf);
                    if (rbuf[0] != 'H') throw new Exception("数据传输出错");

                    k = 0;
                    for (i = 1; i <= cycle; ++i)
                    {
                        byte[] sendBuf = new byte[unit1];
                        for (j = 0; j < unit1; ++j)
                        {
                            sendBuf[j] = storage1[k];
                            ++k;
                        }
                        cltSock.Send(sendBuf);
                        cltSock.Receive(rbuf);
                        if (rbuf[0] != 'H') throw new Exception("数据传输出错");

                        progressBar1.Value = (int)(100.0 * i / cycle);
                    }
                    if (residue > 0)
                    {
                        byte[] sendBuf = new byte[residue];
                        for (j = 0; j < residue; ++j)
                        {
                            sendBuf[j] = storage1[k];
                            ++k;
                        }
                        cltSock.Send(sendBuf);
                        cltSock.Receive(rbuf);
                        if (rbuf[0] != 'H') throw new Exception("数据传输出错");
                    }

                    progressBar1.Value = 100;

                    dt = DateTime.Now;
                    textBox_Info.Text += "数据发送完成 " + dt.ToString("G") + "." + dt.Millisecond.ToString("D3") + "\n";
                    ScrollToTextEnd();
                    textBox_Info.Refresh();

                    ConstructImageFromData(storage1, width, height, depth);
                    pictureBox1.Image = bmp;
                    pictureBox1.Refresh();
                    /////////////////////////////////////////////////////////////////


                    ///// BACKWARD
                    cltSock.Receive(packi);
                    cltSock.Send(rbuf);
                    if (rbuf[0] != 'H') throw new Exception("数据传输出错");

                    width = BitConverter.ToInt32(packi, 0);
                    height = BitConverter.ToInt32(packi, sz_int);
                    depth = BitConverter.ToInt32(packi, 2 * sz_int);
                    unit2 = BitConverter.ToInt32(packi, 3 * sz_int);
                    dataSize = width * height * depth / 8;
                    cycle = dataSize / unit2;
                    residue = dataSize - unit2 * cycle;

                    dt = DateTime.Now;
                    textBox_Info.Text += "正在接收数据 " + dt.ToString("G") + "." + dt.Millisecond.ToString("D3") + "\n";
                    ScrollToTextEnd();
                    textBox_Info.Refresh();

                    byte[] storage2 = new byte[dataSize];
                    byte[] recvBuf = new byte[unit2];

                    k = 0;
                    for (i = 1; i <= cycle; ++i)
                    {
                        cltSock.Receive(recvBuf);
                        for (j = 0; j < unit2; ++j)
                        {
                            storage2[k] = recvBuf[j];
                            ++k;
                        }
                        cltSock.Send(rbuf2);

                        progressBar1.Value = 100 * (cycle - i) / cycle;
                    }
                    if (residue > 0)
                    {
                        cltSock.Receive(recvBuf);
                        for (j = 0; j < residue; ++j)
                        {
                            storage2[k] = recvBuf[j];
                            ++k;
                        }
                        cltSock.Send(rbuf2);
                    }
                    progressBar1.Value = 0;

                    dt = DateTime.Now;
                    textBox_Info.Text += "数据接收完毕 " + dt.ToString("G") + "." + dt.Millisecond.ToString("D3") + 
                        "\n数据大小(WF/Byte): " + dataSize.ToString() + ", " + unit2.ToString() + "\n";
                    ScrollToTextEnd();
                    textBox_Info.Refresh();

                    ConstructImageFromData(storage2, width, height, depth);

                    string ss=strFileNames[index];
                    ss = ss.Substring(0, ss.Length - 4) + "_r_" + job_type.ToString() + ".dat";
                    FileStream fsv = new FileStream(ss, FileMode.Create);
                    fsv.Write(packi, 0, 3 * sz_int);
                    fsv.Write(storage2, 0, dataSize);
                    fsv.Close();
                    textBox_Info.Text += "数据已保存为{"+ss+"}\n\n";
                    ScrollToTextEnd();
                    textBox_Info.Refresh();

                    pictureBox1.Image = bmp;
                    pictureBox1.Refresh();

                    System.Threading.Thread.Sleep(1000);
                    ++index;
                }
                #endregion WHILE_LLOP
            }
            catch (Exception ex)
            {
                textBox_Info.Text += "*****EXCEPTION*****\n" + ex.Message + "\n";
                ScrollToTextEnd();
                textBox_Info.Refresh();
            }
        }

        private void MainForm_FormClosing(object sender, EventArgs e)
        {
            cltSock.Shutdown(SocketShutdown.Both);
            cltSock.Close();
        }

        private void ConstructImageFromData(byte[] storage, int width, int height, int depth)
        {
            if (bmp != null)
            {
                bmp.Dispose();
            }
            bmp = new Bitmap(width, height, PixelFormat.Format24bppRgb);
            BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.WriteOnly, PixelFormat.Format24bppRgb);
            int iStride = bmpData.Stride;
            int sz_ushort = sizeof(ushort);
            unsafe
            {
                byte* ptr = (byte*)(bmpData.Scan0.ToPointer());

                int i, j, k, t;

                k = 0;
                if (depth == 8)
                {
                    for (i = 0; i < height; ++i)
                    {
                        for (j = 0; j < width; ++j)
                        {
                            t = iStride * i + 3 * j;
                            ptr[t] = ptr[t + 1] = ptr[t + 2] = storage[k];
                            ++k;
                        }
                    }
                }
                else
                {
                    ushort uvar;
                    for (i = 0; i < height; ++i)
                    {
                        for (j = 0; j < width; ++j)
                        {
                            t = iStride * i + 3 * j;
                            uvar = BitConverter.ToUInt16(storage, k);
                            k += sz_ushort;
                            ptr[t] = ptr[t + 1] = ptr[t + 2] = (byte)(uvar >> 2);
                        }
                    }
                }
            }

            bmp.UnlockBits(bmpData);
        }

        private void ContextMenuSaveAs_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image == null) return;
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "JPEG|*.jpg";
            if (DialogResult.OK == sfd.ShowDialog(this))
            {
                pictureBox1.Image.Save(sfd.FileName, ImageFormat.Jpeg);
            }
        }

        private void buttonAddUnit_Click(object sender, EventArgs e)
        {
            string txt = textBoxUnit.Text.Trim();
            textBoxUnit.Clear();
            if (string.IsNullOrEmpty(txt)) return;
            int unit;
            bool b = int.TryParse(txt, out unit);
            if (!b || unit < 1024 || unit > 2048)
            {
                MessageBox.Show("请输入[1024,2048]之间的数字", "提示");
                return;
            }
            if (comboBox_PackUnit.Items.Contains(unit))
            {
                MessageBox.Show(txt + ": 已经存在!", "提示");
                return;
            }

            comboBox_PackUnit.Items.Add(unit);
            int idx = comboBox_PackUnit.Items.IndexOf(unit);
            comboBox_PackUnit.SelectedIndex = idx;
        }

        private void button_Update_Click(object sender, EventArgs e)
        {
            UpdateNetworkSettings();
        }

        private void UpdateNetworkSettings()
        {
            if (cltSock != null && cltSock.Connected)
            {
                MessageBox.Show("现在无法更新设置，请关闭后重试", "提示");
                return;
            }

            string IPAddrStr = textBox_IPAddr.Text;
            int port;
            bool b = int.TryParse(textBox_PortNum.Text, out port);
            if (!b || port < 1 || port > 65535)
            {
                MessageBox.Show("请输入合法的端口号!", "提示");
                return;
            }
            textBox_Info.Text += "[网络设置已更新] " + IPAddrStr + ":" + port.ToString() + "\n";
            ScrollToTextEnd();

            if (cltSock != null)
            {
                cltSock.Dispose();
            }

            targetEP = new IPEndPoint(IPAddress.Parse(IPAddrStr), port);
            cltSock = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
        }

        private void button_FileList_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "选择列表文件";
            ofd.Multiselect = false;
            ofd.Filter = "Text|*.txt";
            if (DialogResult.OK == ofd.ShowDialog(this))
            {
                StreamReader sr = new StreamReader(ofd.FileName);
                strFileNames.Clear();
                count = 0;
                index = 0;
                while (!sr.EndOfStream)
                {
                    string line = sr.ReadLine();
                    if (line.Length >= 5)
                    {
                        string ext = line.Substring(line.Length - 4);
                        ext = ext.ToUpper();
                        if (ext == ".DAT")
                        {
                            strFileNames.Add(line);
                            ++count;
                        }
                    }
                }
                sr.Close();
                textBox_Info.Text += "*********************************\n已找到" + count.ToString() + "个条目\n";
                ScrollToTextEnd();
            }
        }

        private bool IsGoodEcho(byte[] recvBuf, string goodEchoStr)
        {
            if (recvBuf[0] != (byte)(goodEchoStr[0]))
            {
                textBox_Info.Text += "ERROR:来自远端的错误回应\n";
                ScrollToTextEnd();
                return false;
            }
            return true;
        }

        private void ScrollToTextEnd()
        {
            textBox_Info.Focus();
            textBox_Info.Select(textBox_Info.Text.Length - 1, 0);
            textBox_Info.ScrollToCaret();
        }

        private delegate void UpdateImage();

        private void ShowImage()
        {
            if (bmp != null)
            {
                pictureBox1.Image = bmp;
            }
        }

    }
}
