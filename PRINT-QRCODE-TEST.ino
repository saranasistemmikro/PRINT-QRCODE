
#include <Wire.h>
#include <DS3231.h>

#define INP1    0
#define LED2    3
#define RELAY1 19

DS3231 clock;
RTCDateTime dt;
int mCounter=1;
uint8_t stsInp1=0;
void setup()
{
  char MallName[30];
  char Counter[10];
  char mDateTime[21];
  Serial.begin(115200);
  Serial1.begin(38400);

  pinMode(INP1,INPUT_PULLUP);
  pinMode(RELAY1,OUTPUT);

  // Initialize DS3231
  Serial.println("Initialize DS3231");;
  clock.begin();

  // Set sketch compiling time
  clock.setDateTime(__DATE__, __TIME__);

 
 // dt = clock.getDateTime();
 // strcpy(MallName,"MALL_SAMPLE");
 // sprintf(Counter,"%05d",mCounter);
 // sprintf(mDateTime,"%d/%02d/%02d %02d:%02d:%02d ",dt.year,dt.month,dt.day,dt.hour,dt.minute,dt.second);
 // printing(MallName,Counter,mDateTime);

}

void loop()
{
 
  char MallName[30];
  char Counter[10];
  char mDateTime[21];
  //if stsInp1==0 && digitalRead(INP1);
  //Serial.println(digitalRead(INP1));

  if (stsInp1==0 && !digitalRead(INP1))
    {
       stsInp1=1;  
       dt = clock.getDateTime();
       Serial.println("Printing");
       strcpy(MallName,"MALL_SAMPLE");
       sprintf(Counter,"%05d",mCounter);
       sprintf(mDateTime,"%d/%02d/%02d %02d:%02d:%02d ",dt.year,dt.month,dt.day,dt.hour,dt.minute,dt.second);
       
       Serial.println("PRINT QR CODE, OPEN BARRIER GATE");
       printing(MallName,Counter,mDateTime);
       digitalWrite(LED2,HIGH);
       digitalWrite(RELAY1,HIGH);
       delay(400);
       digitalWrite(LED2,LOW);
       digitalWrite(RELAY1,LOW);
       mCounter++;
    }
  if (stsInp1==1 && digitalRead(INP1))
    {
       stsInp1=0;  
    }  
    
  
  

  // For leading zero look to DS3231_dateformat example

 

  delay(500);
}

void printing(char *MallName, char *Counter, char *mDateTime)
{      
           char mytext[20];
           char qrcodedata[60];
           memset(mytext,'\0',20);
           mytext[0]=0x1d; // select character size (GS ! n)
           mytext[1]='!';
           mytext[2]=0x0;          
           Serial1.write(mytext,3);

           mytext[0]=0x1b; // cancel emphasize
           mytext[1]='!';
           mytext[2]=0x0;          
           Serial1.write(mytext,3);

           
           
           mytext[0]=0x1d; // LEFT MARGIN
           mytext[1]=0x4c; 
           mytext[2]=0x70;
           mytext[3]=0x0;
           Serial1.write(mytext,4);
           
          
         
          Serial1.write(MallName,strlen(MallName));
          Serial1.write("\r\n",2);
          Serial1.write(mDateTime,strlen(mDateTime));
          Serial1.write("\r\n",2);

         
      
        
          mytext[0]=0x1b; // ESC ! 0x38 // DOUBLE WIDE,EMPHASIZED
           mytext[1]='!';
           mytext[2]=0x38;         
           Serial1.write(mytext,3);

          
       
        
           mytext[0]=' '; // PRINT SPACE
           mytext[1]=0xd;
           mytext[2]=0xa;
          // mytext[3]=0xa;
           Serial1.write(mytext,3);
           

       
         memset(mytext,'\0',20);
         mytext[0]=0x1d; //LEFT MARGIN
         mytext[1]=0x4c; 
         mytext[2]=0x90;
         mytext[3]=0x0;
         Serial1.write(mytext,4);
     
         mytext[0]=0x1d;
         mytext[1]=0x28;
         mytext[2]=0x6b;
         mytext[3]=0x4;
         mytext[4]=0x0;
         mytext[5]=0x31;
         mytext[6]=0x41;
         mytext[7]=0x32;// type qrcode 2 0x32; = (50)
         mytext[8]=0x0;
         Serial1.write(mytext,9);

         //strcpy(mytext,"\x1d\x28\x6b\x3\x0\x31\x43\x8");//("\x1d\x28\x6b\x3\x01C\xd"); //    x8  lebar qr code   167       //myStr = myStr & Chr(&H1D) & Chr(&H28) & Chr(&H6B) & Chr(&H3) & Chr(&H0) & Chr(&H31) & Chr(&H43) & Chr(&H8) ' Chr(1) S/D CHR(16) F 167
         mytext[0]=0x1d;
         mytext[1]=0x28;
         mytext[2]=0x6b;
         mytext[3]=0x3;
         mytext[4]=0x0;
         mytext[5]=0x31;
         mytext[6]=0x43;
         mytext[7]=0x6;// 0xa;//0x8;  // lebar barcode
         Serial1.write(mytext,8);

         //strcpy(mytext,"\x1d\x28\x6b\x3\x0\x31\x45\x33"); //("\x1d\x28\x6b\x3\x01E3 ");  //180       //myStr = myStr & Chr(&H1D) & Chr(&H28) & Chr(&H6B) & Chr(&H3) & Chr(&H0) & Chr(&H31) & Chr(&H45) & Chr(&H33)
         mytext[0]=0x1d;
         mytext[1]=0x28;
         mytext[2]=0x6b;
         mytext[3]=0x3;
         mytext[4]=0x0;
         mytext[5]=0x31;
         mytext[6]=0x45;
         mytext[7]=0x33;
         Serial1.write(mytext,8);



         strcpy(qrcodedata,MallName);
         strcat(qrcodedata,".");
         strcat(qrcodedata,Counter);
         strcat(qrcodedata,"\r\n");
         strcat(qrcodedata,mDateTime);
         
         mytext[0]=0x1d;
         mytext[1]=0x28;
         mytext[2]=0x6b;
         mytext[3]= strlen(qrcodedata) +  3;// 0xd;
         mytext[4]=0x0;
         mytext[5]=0x31;
         mytext[6]=0x50;
         mytext[7]=0x30;
         Serial1.write(mytext,8);

       
         
         
         Serial1.write(qrcodedata,strlen(qrcodedata));
         
         mytext[0]=0x1d;
         mytext[1]=0x28;
         mytext[2]=0x6b;
         mytext[3]=0x4;
         mytext[4]=0x0;
         mytext[5]=0x31;
         mytext[6]=0x51;
         mytext[7]=0x30;
         mytext[8]=0x0;
         Serial1.write(mytext,9);
         
         mytext[0]=0x1b;
         mytext[1]=0x33;
         mytext[2]=0x0;
         mytext[3]=0x0;
         mytext[4]=0xa;
         Serial1.write(mytext,5);

           mytext[0]=' ';
           mytext[1]=0xd;
           mytext[2]=0xa;
         
           Serial1.write(mytext,3);


         //POTONG
         mytext[0]=0x1d;
         mytext[1]=0x56;
         mytext[2]=0x42; 
         mytext[3]=0x0;
         Serial1.write(mytext,4);  

           mytext[0]=' ';
           mytext[1]=0xd;
           mytext[2]=0xa;
          // mytext[3]=0xa;
           Serial1.write(mytext,3);
           
           mytext[0]=' ';
           mytext[1]=0xd;
           mytext[2]=0xa;
           Serial1.write(mytext,3);

//================================
        strcpy(mytext,"\x1b\x32");    
         mytext[0]=0x1b;
         mytext[1]=0x32;
         Serial1.write(mytext,2);   

}         
