using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RAK
{
    class Xmodem
    {
        int SOH = 0x01;
        int STX = 0x02;
        int EOT = 0x04;
        int ACK = 0x06;
        int BSP = 0x08;
        int NAK = 0x15;
        int CAN = 0x18;
        int CTRLZ = 0x1A;  // ^Z for DOS officionados

        int DLY_1S = 1000;
        int MAXRETRANS = 25;
        static int last_error = 0;
        ///////
        //串口发送字符'C'
        void charport_outbyte(char trychar)
        {
            byte[] buf=new byte[2];
            buf[0] = Convert.ToByte(trychar);
            Form1.comm.Write(buf, 0, 1);
        }

        void intport_outbyte(int trychar)
        {
            byte[] buf = new byte[2];
            buf[0] = (byte)(trychar&0xFF);
            Form1.comm.Write(buf, 0, 1);
        }

        char port_inbyte(int time_out)
        {
            char[] ch=new char[2];
            last_error = 0;
            if (Form1.comm.Read(ch,0, 1) == 1)
                return ch[0];
            last_error = 1;
            return ch[0];
        }
        //Xmodem接收处理
/*        int XmodemReceive(char dest, int destsz)
        {
             byte[] xbuff = new byte[1030];
             char p;
             int bufsz, crc = 0;
             char trychar = 'C';
             int packetno = 1;
             int i, c, len = 0;
             int retry, retrans = MAXRETRANS;

            for(;;)
             {
              for( retry = 0; retry < 16; ++retry)
              {
               if (trychar=='C')
                  charport_outbyte(trychar);
               c = port_inbyte((DLY_1S)<<1);
               if (last_error == 0)
               {
                switch (c)
                {
                 case 0x01://SOH
                  bufsz = 128;
                  goto start_recv;
                 case 0x02://STX
                  bufsz = 1024;
                  goto start_recv;
                 case 0x04://EOT
                  //flushinput();
                  intport_outbyte(ACK);
                  return len;
                 case 0x18://CAN
                  c = port_inbyte(DLY_1S);

                  if (c == CAN)
                  {
                   //flushinput();
                   intport_outbyte(ACK);
                   return -1;
                  }
                  break;
                 default:
                  break;
                }
               }
              }
              if (trychar == 'C')
              {
               trychar = Convert.ToChar(NAK);
               continue;
              }
              //flushinput();
              intport_outbyte(CAN);
              intport_outbyte(CAN);
              intport_outbyte(CAN);
              return -2;

             start_recv:
              if (trychar == 'C') 
                  crc = 1;
              trychar = Convert.ToChar(0);
              p = xbuff;
              *p++ = c;
              for (i = 0;  i < (bufsz+(crc?1:0)+3); ++i)
              {
               c = port_inbyte(DLY_1S);
               if (last_error != 0)
                goto reject;
               *p++ = c;
              }

              if (xbuff[1] == (unsigned char)(~xbuff[2]) &&
               (xbuff[1] == packetno || xbuff[1] == (unsigned char)packetno-1) &&
               check(crc, &xbuff[3], bufsz))
              {
               if (xbuff[1] == packetno) 
               {
                int count = destsz - len;
                if (count > bufsz)
                 count = bufsz;
                if (count > 0)
                {
                 memcpy (&dest[len], &xbuff[3], count);
                 len += count;
                }
                ++packetno;
                retrans = MAXRETRANS+1;
               }
               if (--retrans <= 0)
               {
                //flushinput();
                intport_outbyte(CAN);
                intport_outbyte(CAN);
                intport_outbyte(CAN);
                return -3;
               }
               intport_outbyte(ACK);
               continue;
              }
             reject:
              //flushinput();
              intport_outbyte(NAK);
             }
            }
 */       ///////
    }
}
