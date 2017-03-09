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

namespace RAK
{   
    public partial class Form1 : Form
    {
        private SerialPort comm = new SerialPort();
        private int len = 0, length = 0;
        byte[] buf = new byte[5000];                    //声明一个临时数组存储当前来的串口数据

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
            comboBaudrate.SelectedIndex = comboBaudrate.Items.IndexOf("115200");
        }
        //串口接收数据信息
        private void comm_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {          
            System.Threading.Thread.Sleep(100);//延时，以确保数据完全接收
            int n = comm.BytesToRead;             //先记录下来，避免某种原因，人为的原因，操作几次之间时间长，缓存不一致
            
            comm.Read(buf, length, n);            //读取缓冲数据

            //因为要访问ui资源，所以需要使用invoke方式同步ui。
            this.Invoke((EventHandler)(delegate
            {
                {

                }
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
                this.textBoxPlay.AppendText("串口已关闭\r\n");
            }
            else
            {
                //this.textBoxPlay.AppendText("Serial Connected.\r\n");
                if (comboPortName.Text != "")
                {
                    //关闭时点击，则设置好端口，波特率后打开
                    comm.PortName = comboPortName.Text;                           //端口名
                    comm.BaudRate = int.Parse(comboBaudrate.Text);                //波特率
                    this.textBoxPlay.AppendText("串口已打开\r\n");
                    try
                    {
                        comm.Open();
                        if (comm.IsOpen == true)
                            //添加事件注册
                            comm.DataReceived += comm_DataReceived;
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
                    MessageBox.Show("未检测到有串口连接");
                }
            }
            buttonOpenserial.Text = comm.IsOpen ? "关闭" : "打开";

        }
        //串口发送数据
        //串口发送，蓝色字体显示
        void Com_Write(string data)
        {
            if (comm.IsOpen)
            {
                comm.Write(data);
                this.textBoxPlay.Select(textBoxPlay.TextLength, 0);//将光标始终指向最末尾
                this.textBoxPlay.SelectionColor = Color.Blue;
                this.textBoxPlay.AppendText("发送 => " + data+"\r\n");
            }
            else
            {
                MessageBox.Show("请先打开串口 !!!");
            }
        }
        //设置flash更新位置
        private void buttonsetflash_Click(object sender, System.EventArgs e)
        {
            string senddata="";
            if (checkBoxAll.Checked == true)
            {
                senddata = "set block all";
                Com_Write(senddata);
            }
            else
            {
                if (checkBoxmcu.Checked == true)
                {                    
                    senddata += "mcu";
                }
                if (checkBoxwlan.Checked == true)
                {
                    if (senddata != "")
                        senddata += "&";
                    senddata += "wlan";
                }
                if (checkBoxgdbin.Checked == true)
                {
                    if (senddata != "")
                        senddata += "&"; 
                    senddata += "gdbin";
                }
                if (checkBoxrftest.Checked == true)
                {
                    if (senddata != "")
                        senddata += "&"; 
                    senddata += "rftest";
                }
                if (checkBox415_web_u_ch.Checked == true)
                {
                    if (senddata != "")
                        senddata += "&"; 
                    senddata += "415_web_u_ch";
                }
                if (checkBox415_web_u_en.Checked == true)
                {
                    if (senddata != "")
                        senddata += "&"; 
                    senddata += "415_web_u_en";
                }
                if (checkBox415_config.Checked == true)
                {
                    if (senddata != "")
                        senddata += "&"; 
                    senddata += "415_config";
                }
                if (checkBox415_web_d_ch.Checked == true)
                {
                    if (senddata != "")
                        senddata += "&"; 
                    senddata += "415_web_d_ch";
                }
                if (checkBox415_web_d_en.Checked == true)
                {
                    if (senddata != "")
                        senddata += "&"; 
                    senddata += "415_web_d_en";
                }
                if (checkBox411_web_u_ch.Checked == true)
                {
                    if (senddata != "")
                        senddata += "&"; 
                    senddata += "411_web_u_ch";
                }
                if (checkBox411_web_u_en.Checked == true)
                {
                    if (senddata != "")
                        senddata += "&"; 
                    senddata += "411_web_u_en";
                }
                if (checkBox411_config.Checked == true)
                {
                    if (senddata != "")
                        senddata += "&"; 
                    senddata += "411_config";
                }
                if (senddata != "")
                {
                    senddata = "set block " + senddata;
                    Com_Write(senddata);
                }
                else 
                {
                    MessageBox.Show("请选择Flash更新位置");
                }
                
            }
        }
        //选择了all，就不能选择其他的
        private void checkBoxAll_CheckedChanged(object sender, System.EventArgs e)
        {
            if (checkBoxAll.Checked == true)
            {
                checkBoxwlan.Enabled = false;
                checkBoxmcu.Enabled = false;
                checkBoxrftest.Enabled = false;
                checkBoxgdbin.Enabled = false;
                checkBox415_web_u_ch.Enabled = false;
                checkBox415_web_u_en.Enabled = false;
                checkBox415_config.Enabled = false;
                checkBox415_web_d_ch.Enabled = false;
                checkBox415_web_d_en.Enabled = false;
                checkBox411_web_u_ch.Enabled = false;
                checkBox411_web_u_en.Enabled = false;
                checkBox411_config.Enabled = false;
            }
            else
            {
                checkBoxwlan.Enabled = true;
                checkBoxmcu.Enabled = true;
                checkBoxrftest.Enabled = true;
                checkBoxgdbin.Enabled = true;
                checkBox415_web_u_ch.Enabled = true;
                checkBox415_web_u_en.Enabled = true;
                checkBox415_config.Enabled = true;
                checkBox415_web_d_ch.Enabled = true;
                checkBox415_web_d_en.Enabled = true;
                checkBox411_web_u_ch.Enabled = true;
                checkBox411_web_u_en.Enabled = true;
                checkBox411_config.Enabled = true;
            }
        }
        //设置产品类型
        private void buttonsetstyle_Click(object sender, System.EventArgs e)
        {
            if (comboBoxmodulestyle.Text == "RAK415")
            {
                Com_Write("set product 1");
            }
            else if (comboBoxmodulestyle.Text == "RAK411 UART")
            {
                Com_Write("set product 2");
            }
            else if (comboBoxmodulestyle.Text == "RAK411 SPI")
            {
                Com_Write("set product 3");

            }
        }
        //清空显示栏
        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            textBoxPlay.Text = "";
        }

        

    }
}
