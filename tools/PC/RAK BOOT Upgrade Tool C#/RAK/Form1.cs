using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Text.RegularExpressions;
using System.Windows;
using System.IO;
using System.Threading;
using System.Globalization;
using System.Xml;
using System.Configuration;

namespace RAK
{   
    public partial class Form1 : Form
    {
        public static SerialPort comm = new SerialPort();
        private int len = 0, length = 0;
        byte[] buf = new byte[5000];                    //声明一个临时数组存储当前来的串口数据
        string Mytext = "";
        int file_len;
        int file_send;
        int read_len;
        byte[] binchar = new byte[128];
        bool check = false;
        string[] filepaths;

        int send_current_line = 0;
        public Form1()
        {
            InitializeComponent();           
        }

        private void Form1_Load(object sender, System.EventArgs e)
        {
            //初始化下拉串口名称列表框
            string[] ports = SerialPort.GetPortNames();
            Array.Sort(ports);
            comboPortName.Items.AddRange(ports);
            comboPortName.SelectedIndex = comboPortName.Items.Count > 0 ? 0 : -1;
            //comboBaudrate.SelectedIndex = comboBaudrate.Items.IndexOf("115200");
        }
        
        byte[] current_data = new byte[133];
        byte[] ss = {0x04,0x00};
        bool first_time = true;
        //串口接收数据信息
        private void comm_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {          
            System.Threading.Thread.Sleep(10);//延时，以确保数据完全接收
            int n = comm.BytesToRead;             //先记录下来，避免某种原因，人为的原因，操作几次之间时间长，缓存不一致            
            comm.Read(buf, length, n);            //读取缓冲数据

            //因为要访问ui资源，所以需要使用invoke方式同步ui。
            this.Invoke((EventHandler)(delegate
            {
                string versionstring = Encoding.GetEncoding("gb2312").GetString(buf);
                if (versionstring.Contains("BOOT"))
                {
                    this.textBoxPlay.Select(textBoxPlay.TextLength, 0);//将光标始终指向最末尾
                    this.textBoxPlay.SelectionColor = Color.Green;
                    this.textBoxPlay.AppendText(versionstring);
                    
                    comm.Write("1");
                    Thread.Sleep(500);
                    comm.Write("u");
                }
                if (upgrade_473_476 || upgrade_475_477_3)
                {
                    string bufstring = Encoding.GetEncoding("gb2312").GetString(buf) + "\r\n";
                    if ((buf[0] == 0x4F) && (buf[1] == 0x4B))
                    {
                        comm.Write("u");
                        this.textBoxPlay.Select(textBoxPlay.TextLength, 0);//将光标始终指向最末尾
                        this.textBoxPlay.SelectionColor = Color.Green;
                        this.textBoxPlay.AppendText(bufstring);
                    }
                    else 
                    {
                        this.textBoxPlay.Select(textBoxPlay.TextLength, 0);//将光标始终指向最末尾
                        this.textBoxPlay.SelectionColor = Color.Red;
                        this.textBoxPlay.AppendText(bufstring);
                    }

                    upgrade_473_476 = false;
                    upgrade_475_477_3 = false;
                }
                if (upgrade_475_477_2)
                {
                    string bufstring = Encoding.GetEncoding("gb2312").GetString(buf) + "\r\n";
                    if ((buf[0] == 0x4F) && (buf[1] == 0x4B))
                    {
                        upgrade_475_477_3 = true;
                        comm.Write("at+upgrade\r\n");
                        this.textBoxPlay.Select(textBoxPlay.TextLength, 0);//将光标始终指向最末尾
                        this.textBoxPlay.SelectionColor = Color.Green;
                        this.textBoxPlay.AppendText(bufstring);
                    }
                    else
                    {
                        this.textBoxPlay.Select(textBoxPlay.TextLength, 0);//将光标始终指向最末尾
                        this.textBoxPlay.SelectionColor = Color.Red;
                        this.textBoxPlay.AppendText(bufstring);
                    }
                    upgrade_475_477_2 = false;
                }
                if (upgrade_475_477)
                {
                    string bufstring = Encoding.GetEncoding("gb2312").GetString(buf) + "\r\n";
                    if (buf[0] == 0x55)
                    {
                        upgrade_475_477_2 = true;
                        comm.Write("U");
                        this.textBoxPlay.Select(textBoxPlay.TextLength, 0);//将光标始终指向最末尾
                        this.textBoxPlay.SelectionColor = Color.Green;
                        this.textBoxPlay.AppendText(bufstring);
                    }
                    else
                    {
                        this.textBoxPlay.Select(textBoxPlay.TextLength, 0);//将光标始终指向最末尾
                        this.textBoxPlay.SelectionColor = Color.Red;
                        this.textBoxPlay.AppendText(bufstring);
                    }
                    upgrade_475_477 = false;
                }

                if (buf[0] == 0x43)//接收到'C'，等待升级固件
                {
                    check = true;
                    string bufstring = Encoding.GetEncoding("gb2312").GetString(buf) + "\r\n";
                    this.textBoxPlay.Select(textBoxPlay.TextLength, 0);//将光标始终指向最末尾
                    this.textBoxPlay.SelectionColor = Color.Green;
                    this.textBoxPlay.AppendText(bufstring);
                    if (!first_time)
                    {
                        if (begin_upgrade() == -1)
                        {
                            stop_upgrade_state();
                        }
                    }
                    if (module_type.Equals("RAK473") || module_type.Equals("RAK476") || module_type.Equals("RAK475") || module_type.Equals("RAK477"))
                    {
                        if (begin_upgrade() == -1)
                        {
                            stop_upgrade_state();
                        }
                    }
                }
                if (buf[0] == CAN)//CAN 无条件停止
                {
                    stop_upgrade_state();
                    this.dataGVscan.Rows[send_current_line].Cells[3].Style.BackColor = Color.LightSkyBlue;
                    this.dataGVscan.Rows[send_current_line].Cells[3].Value = "Upgrade failed\r\n";
                    this.textBoxPlay.Select(textBoxPlay.TextLength, 0);//将光标始终指向最末尾
                    this.textBoxPlay.SelectionColor = Color.Red;
                    this.textBoxPlay.AppendText("Upgrade " + this.dataGVscan.Rows[send_current_line].Cells[1].Value + " failed!\r\n");
                }
                if ((buf[0] == ACK)&&(xmodem_send))//ACK
                {                     
                    if (Last_eot)//收到EOT的ACK
                    {
                        this.dataGVscan.Rows[send_current_line].Cells[3].Style.BackColor = Color.PaleGreen;
                        this.dataGVscan.Rows[send_current_line].Cells[3].Value = "Upgrade success";
                        Last_eot = false;
                        packetno = 1;
                        file_len = 0;
                        file_send = 0;
                        Last_one = false;
                        packetno = 1;
                        file_len = 0;
                        if (binreader != null)
                        {
                            binreader.Close();
                        }
                        this.textBoxPlay.Select(textBoxPlay.TextLength, 0);//将光标始终指向最末尾
                        this.textBoxPlay.SelectionColor = Color.Blue;
                        this.textBoxPlay.AppendText("Upgrade " + this.dataGVscan.Rows[send_current_line].Cells[1].Value + " success!\r\n");
                        first_time = false;
                        send_current_line = get_send_path(send_current_line+1);
                        if (send_current_line != -1)
                        {
                            comm.Write("u");
                        }
                        else
                        {
                            stop_upgrade_state();
                            this.textBoxPlay.Select(textBoxPlay.TextLength, 0);//将光标始终指向最末尾
                            this.textBoxPlay.SelectionColor = Color.Black;
                            this.textBoxPlay.AppendText("Upgrade success!\r\n");
/*                            for (int i = 0; i < filepaths.Length; i++)
                            {
                                this.dataGVscan.Rows[i].Cells[3].Style.BackColor = Color.Transparent;
                                this.dataGVscan.Rows[i].Cells[3].Value = "wait for upgrade";//信号强度
                            }
*/                        }
                    }
                    else if (Last_one)//收到最后一包ACK
                    {
                        this.dataGVscan.Rows[send_current_line].Cells[3].Value = "Upgrading...  " + (int)(file_send * 100 / file_len) + "%";
                        Last_one = false;                           
                        Last_eot = true;
                        current_data = ss;
                        Com_Write_Byte(ss,1);//发送EOT                           
                    }
                    else
                    {
                        current_data = Encode_Xmodem(Read_bin());
                        Com_Write_Byte(current_data, current_data.Length);//发送下一包
                        packetno++;
                        if (packetno == 256)
                            packetno = 0;
                        this.dataGVscan.Rows[send_current_line].Cells[3].Value = "Upgrading...  " + (int)(file_send * 100 / file_len) + "%";
                    }
                }
                if ((buf[0] == NAK)&&(xmodem_send))//NAK
                {
                    this.textBoxPlay.Select(textBoxPlay.TextLength, 0);//将光标始终指向最末尾
                    this.textBoxPlay.SelectionColor = Color.Red;
                    this.textBoxPlay.AppendText("Send last packet again\r\n");
                    Com_Write_Byte(current_data, current_data.Length);//重发上一包
                    this.dataGVscan.Rows[send_current_line].Cells[3].Value = "Upgrading...  " + (int)(file_send * 100 / file_len) + "%";
                }
                //清空buf
                Array.Clear(buf,0,5000);
            }));
        }

        //打开或关闭串口
        private void buttonOpenserial_Click(object sender, EventArgs e)
        {
            //根据当前串口对象，来判断操作
            if (comm.IsOpen)
            {
                //打开时点击，则关闭串口
                //this.textBoxPlay.AppendText("Serial Disconnected.\r\n");
                comm.Close();
                this.textBoxPlay.Select(textBoxPlay.TextLength, 0);//将光标始终指向最末尾
                this.textBoxPlay.SelectionColor = Color.Black;
                this.textBoxPlay.AppendText("Serial closed\r\n");
            }
            else
            {
                //this.textBoxPlay.AppendText("Serial Connected.\r\n");
                if (comboPortName.Text != "")
                {
                    //关闭时点击，则设置好端口，波特率后打开
                    comm.PortName = comboPortName.Text;                           //端口名
                    comm.BaudRate = 115200;                //波特率
                    
                    try
                    {
                        comm.Open();

                        if (comm.IsOpen == true)                        
                        {
                            this.textBoxPlay.Select(textBoxPlay.TextLength, 0);//将光标始终指向最末尾
                            this.textBoxPlay.SelectionColor = Color.Black;
                            this.textBoxPlay.AppendText("Serial opened\r\n");
                            comm.DataReceived += comm_DataReceived;//添加事件注册
                        }
                    }
                    catch (Exception ex)
                    {
                        //捕获到异常信息，创建一个新的comm对象，之前的不能用了。
                        comm = new SerialPort();
                        //初始化SerialPort对象
                        comm.NewLine = "\r\n";
                        //添加事件注册
                        comm.DataReceived += comm_DataReceived;
                        //显示异常信息给客户。                  
                        MessageBox.Show(ex.Message);
                    }
                }
                else 
                {
                    MessageBox.Show("Serial connect failed");
                }
            }
            buttonOpenserial.Text = comm.IsOpen ? "Close" : "Open";

        }
        //串口发送数据
        //串口发送字符串，蓝色字体显示
        void Com_Write(string data)
        {
            if (comm.IsOpen)
            {
                comm.Write(data);
                //this.textBoxPlay.Select(textBoxPlay.TextLength, 0);//将光标始终指向最末尾
                //this.textBoxPlay.SelectionColor = Color.Blue;
                //this.textBoxPlay.AppendText("发送 => " + data);
            }
            else
            {
                MessageBox.Show("Please open serial first!!!");
            }
        }

        void Com_Write_Byte(byte[] data,int len)
        {
            if (comm.IsOpen)
            {
                comm.Write(data, 0, len);
            }
            else
            {
                MessageBox.Show("Please open serial first!!!");
            }
        }

        //清空显示栏
        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            textBoxPlay.Text = "";
        }


        //读取bin文件内容
        bool Last_one = false;//标记bin文件最后一包数据
        bool Last_eot = false;
        byte[] Read_bin()
        {
            if ((file_len > 0))
            {
                if ((file_len - file_send) / 128 > 0)//一次读取128字节
                {
                    file_send += 128;
                    binchar = binreader.ReadBytes(128);
                }
                else                   //不足128字节按实际长度读取
                {
                    int other_len = file_len % 128;
                    byte[] bin = binreader.ReadBytes(other_len);
                    Array.Copy(bin, 0, binchar, 0, other_len);
                    for (int i = other_len; i < 128 - other_len; i++)
                    {
                        binchar[i] = (byte)CTRLZ;
                    }
                    file_send += other_len;
                    Last_one = true;
                }
            }
            return binchar;
        }

        //CRC16校验
        public byte[] CRC16_C(byte[] data)
        {
            int res = calcrc(data);
            byte[] ReturnData = new byte[2];
            ReturnData[0] = (byte)(((res & 0xFF00) >> 8) & 0xFF);       //CRC高位 
            ReturnData[1] = (byte)((res & 0x00FF) & 0xFF);       //CRC低位 
            return ReturnData;
        } 
        int calcrc(byte[] data)
        {
            int crc;
            int i;
            int count = 128;
            crc = 0;
            for (int j = 0; j < count; j++)
            {
                crc = crc ^ (int)data[j] << 8;
                i = 8;
                do
                {
                    if ((crc & 0x8000) != 0)
                        crc = crc << 1 ^ 0x1021;
                    else
                        crc = crc << 1;
                } while (--i > 0);
            }

            return (crc);
        } 

        //发送Xmodem协议
        int packetno = 1;
        int SOH = 0x01;
        int STX = 0x02;
        int EOT = 0x04;
        int ACK = 0x06;
        int NAK = 0x15;
        int CAN = 0x18;
        int CTRLZ = 0x1A;  // ^Z for DOS officionados

        //拼凑成Xmodem协议的数据
        byte[] Encode_Xmodem(byte[] data)
        {
            byte[] crc = CRC16_C(data);
            byte[] xmodem_data = new byte[133];
            xmodem_data[0] = (byte)SOH;
            xmodem_data[1] = (byte)packetno;
            xmodem_data[2] = (byte)(~packetno);
            Array.Copy(data, 0, xmodem_data,3,128);            
            Array.Copy(crc, 0, xmodem_data, 131, 2);

            return xmodem_data;
        }
        /*************************************************************************************************************************************************************************
        **导入要升级的bin文件
        *************************************************************************************************************************************************************************/
        private void buttonimport_Click(object sender, System.EventArgs e)
        {
            string str = ConfigurationManager.AppSettings["Directory"];

            OpenFileDialog op = new OpenFileDialog();//弹出浏览框
            if (str == "")
                op.InitialDirectory = System.Environment.CurrentDirectory;//打开当前路径
            else
                op.InitialDirectory = str;//打开上一次路径
            op.Multiselect = true;
            op.RestoreDirectory = false;//还原当前路径
            op.Filter = "BIN文件(*.bin)|*.bin";//还原当前路径
            DialogResult result = op.ShowDialog();
            if (result == DialogResult.OK)
            {
                this.dataGVscan.Rows.Clear();
                filepaths = op.FileNames;//获取文件路径
                //textBoximport.Text=filename;
                if (filepaths != null)
                {
                    //保存这次的路径
                    Configuration configuration = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                    configuration.AppSettings.Settings.Clear();
                    configuration.AppSettings.Settings.Add("Directory", filepaths[0]);
                    configuration.Save();
                    ConfigurationManager.RefreshSection("appSettings");

                    for (int i = 0; i < filepaths.Length; i++)
                    {
                        if (i != filepaths.Length-1)
                            dataGVscan.Rows.Add();
                        //填充列表
                        this.dataGVscan.Rows[i].Cells[0].Value = i + 1;//序号
                        int index = filepaths[i].LastIndexOf("\\");
                        string name;
                        if (index == -1)
                            name = filepaths[i];
                        else
                            name = filepaths[i].Substring(index+1);
                        this.dataGVscan.Rows[i].Cells[1].Value =
                            name;//模块名称
                        FileInfo fileInfo = new FileInfo(filepaths[i]);
                        this.dataGVscan.Rows[i].Cells[2].Value = Math.Ceiling(fileInfo.Length / 1024.0) + "KB";
                        this.dataGVscan.Rows[i].Cells[3].Value = "wait for upgrade";
                        this.dataGVscan.Rows[i].Cells[4].Value = true;
                    }
                }
            }
        }

        private void dataGVscan_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            if (this.dataGVscan.CurrentRow.Cells[4].EditedFormattedValue.ToString() == "True")
            {
                this.dataGVscan.CurrentRow.Cells[4].Value = false;
            }
            else
            {
                this.dataGVscan.CurrentRow.Cells[4].Value = true;
            }
        }
        /*************************************************************************************************************************************************************************
         **发送Xmodem协议
         *************************************************************************************************************************************************************************/
        int get_send_path(int current_line)
        {
            int line=-1;
            for (int i = current_line; i < dataGVscan.Rows.Count; i++)
            {
                if (this.dataGVscan.Rows[i].Cells[4].EditedFormattedValue.ToString() == "True")
                {
                    line = i;
                    break;
                }
            }
            return line;
        }

        bool xmodem_send=false;
        long progress = 0;
        BinaryReader binreader=null;
        bool upgrade_473_476 = false;
        bool upgrade_475_477 = false;
        bool upgrade_475_477_2 = false;
        bool upgrade_475_477_3 = false;
        //发送Xmodem协议
        private void buttonsend_Click(object sender, System.EventArgs e)
        {
            if (!comm.IsOpen)
            {
                MessageBox.Show("Please open serial first");
                return;
            }
            bool choosed = false;

            for (int i = 0; i < dataGVscan.Rows.Count; i++)
            {
                if (this.dataGVscan.Rows[i].Cells[4].EditedFormattedValue.ToString() == "True")
                {
                    choosed = true;
                    break;
                }
            }
            if (!choosed)
            {
                MessageBox.Show("Please select the file to upgrade first");
                return;
            }
            if (module_type.Equals("RAK473") || module_type.Equals("RAK476"))
            {
                comm.Write("at+upgrade\r\n");
                upgrade_473_476 = true;
                xmodem_send = !xmodem_send;
            }
            else if (module_type.Equals("RAK475") || module_type.Equals("RAK477"))
            {
                comm.Write("+++");
                upgrade_475_477 = true;
                xmodem_send = !xmodem_send;
            }
            else
            {
                if (!check)
                {
                    MessageBox.Show("Please let the module into boot mode first");
                    return;
                }
                progress = 0;
                xmodem_send = !xmodem_send;

                if (xmodem_send)
                {
                    for (int i = 0; i < filepaths.Length; i++)
                    {
                        this.dataGVscan.Rows[i].Cells[3].Style.BackColor = Color.Transparent;
                        this.dataGVscan.Rows[i].Cells[3].Value = "wait for upgrade";//信号强度
                    }
                    send_current_line = 0;
                    start_upgrade_state();
                    send_current_line = get_send_path(send_current_line);
                    begin_upgrade();
                }
            }

            if (xmodem_send)
            {

            }
            else
            {
                stop_upgrade_state();
            }
        }

        void start_upgrade_state()
        {
            buttonsend.Text = "Stop Upgrade";
            buttonOpenserial.Enabled = false;
            button1.Enabled = false;
            dataGVscan.Enabled = false;
            file_send = 0;
            send_current_line = 0;
        }

        void stop_upgrade_state()
        {
            xmodem_send = false;
            first_time = true;
            check = false;
            Last_one = false;
            packetno = 1;
            file_len = 0;
            buttonsend.Text = "Start Upgrade";
            buttonOpenserial.Enabled = true;
            button1.Enabled = true;
            dataGVscan.Enabled = true;
            if (binreader != null)
            {
                binreader.Close();
            }
            send_current_line = 0;
        }

        int begin_upgrade()
        {
                try
                {
                    FileStream Myfile = new FileStream(filepaths[send_current_line], FileMode.Open, FileAccess.Read);
                    binreader = new BinaryReader(Myfile);
                    file_len = (int)Myfile.Length;//获取bin文件长度
                    file_send = 0;
                    this.dataGVscan.Rows[send_current_line].Cells[3].Style.BackColor = Color.LightSkyBlue;
                    this.dataGVscan.Rows[send_current_line].Cells[3].Value = "Upgrading...  " + (int)(file_send * 100 / file_len) + "%";
                    current_data = Encode_Xmodem(Read_bin());
                    Com_Write_Byte(current_data, current_data.Length);//发送第一包
                    this.textBoxPlay.Select(textBoxPlay.TextLength, 0);//将光标始终指向最末尾
                    this.textBoxPlay.SelectionColor = Color.Blue;
                    this.textBoxPlay.AppendText("\r\nUpgrading " + this.dataGVscan.Rows[send_current_line].Cells[1].Value + ",please wait...\r\n");
                    packetno++;
                    if (packetno == 256)
                        packetno = 0; 
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.ToString());
                }
                return send_current_line;
        }
       
        //清空显示
        private void linkLabelclear_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            textBoxPlay.Text = "";
        }
        string module_type = "";
        private void button2_Click(object sender, System.EventArgs e)
        {
            module_type = comboBox1.Text;
            this.textBoxPlay.Select(textBoxPlay.TextLength, 0);//将光标始终指向最末尾
            this.textBoxPlay.SelectionColor = Color.Green;
            this.textBoxPlay.AppendText("You have choosed "+ module_type+"\r\n");
        }

    }
}
