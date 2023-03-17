#include <SoftwareSerial.h>
class Bluetooth{
  public:
    int SW_Tx = 10;     
    int SW_RX = 9;  
    String msg = "";
    SoftwareSerial BT = SoftwareSerial(SW_RX, SW_Tx);
    void pinconfig(void){
      Serial.begin(9600);
      pinMode(SW_RX, INPUT);
      pinMode(SW_Tx, OUTPUT);
      BT.begin(9600); 
      BT.println("BT listening");
      Serial.println("Serial listening");
    }
    void read(void){
      while(BT.available()){
        delay(10);
        char c = BT.read();
        
        //Serial.println((int)c);
        msg+=c;  
      }
    }
    void send(String txt){
      BT.println(txt);
    }
    bool is_num(){
      int len = msg.length();
        for (int i = 0; i< len; i++){
          if( msg[i] != '\r' && msg[i] !='\n'){
            if(msg[i] < 48 || msg[i] > 57 ){
              //send("false");
              //BT.println((int)msg[i]);
              return false;
            }
          }
        } 
      return true;
    }
    int numero(){
      int n_digits;
      int num = 0;
      for(int i = 0; i < msg.length(); i++){
        if(msg[i] == '\r'){
        n_digits = i;
        }
      }
      for(int i = 0; i < n_digits;i++){
        num += (msg[i]-48)*power(10,n_digits-i-1);
        //Serial.println(msg[i]-48);
        //Serial.println(power(10,n_digits-i-1));
      //Serial.println(pow(10,(n_digits-i-1)));
      }
      return num;
    }
    String read_msg(){
      String txt;
      int n;
      for(int i = 0; i < msg.length(); i++){
        if(msg[i] == '\r'){
        n= i;
        }
      }
      for(int i = 0; i < n; i++){
        txt+=msg[i];
      }
      return txt;
    }   

  private: 
    
    int power(int num, int y){
      if (y == 0){
        return 1;
      }
      return num*power(num, y-1);
    }



};