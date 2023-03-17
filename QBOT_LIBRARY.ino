#include "Qbot.h"
#include <TimerOne.h>



Qbot qbot1;

int edges[5][4] ={
{1,2,-1,0},
{2,3,-1,0},
{3,4,1,0},
{4,5,0,-1},
{5,1,0,1},
  
};


void setup() {
  // put your setup code here, to run once:

  

  Timer1.initialize(100000);  
  Timer1.attachInterrupt(callback);
  qbot1.start_node = 1;
  qbot1.pin_config();
  
  for(int i = 0; i < 5; i++){
   qbot1.edges[i][0] = edges[i][0];
   qbot1.edges[i][1] = edges[i][1];
   qbot1.edges[i][2] = edges[i][2];
   qbot1.edges[i][3] = edges[i][3];
    }
  qbot1.Qbot_instructions(0);
  }
  void callback(){
    qbot1.timer++;
    
  }


void loop() {

  

  
  
  if(qbot1.repeat == 1){
    qbot1.Qbot_instructions(1);
    
  }
  qbot1.Qbot_move();




  

}
