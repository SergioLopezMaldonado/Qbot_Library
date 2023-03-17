#include "bluetooth.h"
#include "shoortest_path.h"


class Qbot{
  public:
    

    const static int n_edges = 8;
    int start_node ;
    int end_node;
    int timer = 0;
    
    int Qbot_vector[2] ={-1,0};
    int edges[n_edges][4];
    char Qbot_instruction;
    int repeat;
    void Qbot_instructions(int i){
      if(i == 0){
        user_interface(0);
      }
      else {
        user_interface(1);
        Serial.println("pass 2");
        Qbot_end_node = false;
        Qbot_intersection = true;
      }
      Qbot_graph.start_node = start_node;
      
      
      for(int i = 0; i < n_edges; i++){
        Qbot_graph.edges[i][0] = edges[i][0];
        Qbot_graph.edges[i][1] = edges[i][1];
        Qbot_graph.edges[i][2] = edges[i][2];
        Qbot_graph.edges[i][3] = edges[i][3];
        /*Serial.print(Qbot_graph.edges[i][0]);
        Serial.print(",");
        Serial.print(Qbot_graph.edges[i][1]);
        Serial.print(",");
        Serial.print(Qbot_graph.edges[i][2]);
        Serial.println(" ");*/
      }
      Qbot_graph.init_BFS();
      Qbot_graph.instructions();
      /*for(int i= 0; i < Qbot_graph.length_l; i ++){
        Serial.print(Qbot_graph.vectorial_path[i][0]);
        Serial.print(",");
        Serial.println(Qbot_graph.vectorial_path[i][1]);
      }*/
           
    }
    void pin_config(void){
      Qbot_BT.pinconfig();
      pinMode(infrarojo_d, INPUT);
      pinMode(infrarojo_i, INPUT);
      pinMode(motor_d, OUTPUT);
      pinMode(motor_i, OUTPUT);
      pinMode(dir_motor_d, OUTPUT);
      pinMode(dir_motor_i, OUTPUT);
      pinMode(trig, OUTPUT);
      pinMode(echo, INPUT); 
      digitalWrite(trig, LOW); 
 
    }   
    void Qbot_move(void){
      Ultrasonic_sensor();
      if(distancia < 8  && objeto_detectado == false){// && ignorar_objeto == false){
        //Serial.println("yes move");){
        user_interface(2);
        
      }
      if(distancia >= 8){ //&& ignorar_objeto == false){
        objeto_detectado == true;
        //Qbot_BT.send("mayor a 20");
      }

      if(Qbot_end_node == false && objeto_detectado == false){
        //Serial.println("yes move");
        if(digitalRead(infrarojo_d) == LOW && digitalRead(infrarojo_i) == LOW) {
          Qbot_adelante();
          Qbot_intersection = true;   
          timer = 0;
        }
        else if (digitalRead(infrarojo_d) == HIGH && digitalRead(infrarojo_i) == LOW){
          Qbot_derecha();
          //timer = 0;
          while(digitalRead(infrarojo_d) == HIGH && digitalRead(infrarojo_i) == LOW){ 
            //Qbot_BT.BT.println(timer);
            if(timer > 15 ){
                rotacion_derecha() ;
                Qbot_BT.send("giro a la derecha");
                /*Serial.print('d');
                print_vector(Q_bot_vector);*/
                // print_qbot_vector();
                Qbot_intersection = true;
                timer = 0;
            }
          }
        }
        else if (digitalRead(infrarojo_d) == LOW && digitalRead(infrarojo_i) == HIGH){
          Qbot_izquierda();
          //timer = 0;
          while(digitalRead(infrarojo_d) == LOW && digitalRead(infrarojo_i) == HIGH){
            //Qbot_BT.BT.println(timer);
            if(timer > 15 ){
                Qbot_BT.send("giro a la izquierda");
                rotacion_izquierda() ;
                /*Serial.print('i');
                print_vector(Q_bot_vector);*/
                //print_qbot_vector();
                Qbot_intersection = true;
                timer = 0;
            }
          } 
        }
        else if(digitalRead(infrarojo_d) == HIGH && digitalRead(infrarojo_i) == HIGH){
          
            //print_vector(Q_bot_vector);
              //Serial.println("yes");
          while(digitalRead(infrarojo_d) == HIGH && digitalRead(infrarojo_i) == HIGH){
            if( Qbot_intersection == true){
              //Serial.println("intersection yes");
              if(n_move < Qbot_graph.length_l-1){
                //Serial.println("node yes");
              
                Qbot_next_move[0] = Qbot_graph.vectorial_path[n_move][0]; 
                Qbot_next_move[1] = Qbot_graph.vectorial_path[n_move][1]; 
                Qbot_instruction = Qbot_v_t_c(rotacion( Qbot_vector, Qbot_next_move));
                //Serial.println(Qbot_instruction);
                Qbot_BT.BT.println(Qbot_instruction);
                //print_vector(camino_vectorial[n_move]);
                
                Qbot_acciones(Qbot_instruction);
                delay_intersection(Qbot_instruction);          

                Qbot_BT.BT.print("Node:");
                Qbot_BT.BT.println(Qbot_graph.final_path[n_move]);
                Qbot_graph.start_node = Qbot_graph.final_path[n_move];
                if(n_move == 0){
                  int cos_beta = angle_between_vectors(Qbot_vector, Qbot_next_move);
                  /*Serial.println("Qbot vector");
                  print_vector(Qbot_vector);*/
                  
                  if(cos_beta == -1){
                    giro_180();
                  }
                }
                Qbot_vector[0] = Qbot_next_move[0];
                Qbot_vector[1] = Qbot_next_move[1];
                n_move++; 
                
              }
              else{
                  Qbot_BT.BT.print("Qbot has arrived to its destination:");
                  Qbot_BT.BT.println(Qbot_graph.end_node);
                  Qbot_acciones('e');
                  //Qbot_BT.send("vector:");
                  start_node = Qbot_graph.end_node;
                  Qbot_end_node = true;
                  repeat = 1;
                  n_move = 0; 
                  /*Qbot_vector[0] = Qbot_graph.vectorial_path[n_move -1][0]; 
                  Qbot_vector[1] = Qbot_graph.vectorial_path[n_move -1][1];*/
                  //Qbot_BT.BT.println(repeat);
                  
              }
              
              //print_qbot_vector();
              
              
              Qbot_intersection = false;
              
            }
          }
          timer = 0;
         
        }
      }
      else{
        //Qbot_BT.send("no move");
        Qbot_detenerse();
        delay(100);
        if ( Qbot_end_node == false){
          timer++;
         /* if(timer > 50){
            ruta_alternativa();
            Qbot_BT.send("nueva ruta");
            objeto_detectado = false;
            ignorar_objeto = true;
            
            timer  = 0;
          }*/
          if(distancia > 8){ //&& timer <= 50){
            objeto_detectado = false;
            ignorar_objeto = true;

            timer  = 0;
            
          } 
        }
      }
    }
    void print_qbot_vector(){
      Qbot_BT.BT.print(Qbot_vector[0]);
      Qbot_BT.BT.print(',');
      Qbot_BT.BT.println(Qbot_vector[1]);

    }
    void print_vector(int vect[]){
      Serial.print('{');
      Serial.print(vect[0]);
      Serial.print(", ");
      Serial.print(vect[1]);
      Serial.println('}');
    }
  private:
    Graph Qbot_graph;
    Bluetooth Qbot_BT;
    int motor_i = 6;
    int motor_d = 5;
    long distancia;
    long tiempo;
    int trig = 3;
    int echo = 4;

    int dir_motor_i = 8;
    int dir_motor_d = 7;
    int infrarojo_d = 11;
    int infrarojo_i = 12;
    int Qbot_next_move[2];
    int n_move=0;

    bool once = true;
    bool Qbot_end_node = false;
    bool objeto_detectado = false;
    bool ignorar_objeto = false;
    

    bool Qbot_intersection = true;
    int length_l;
    void Ultrasonic_sensor(){
      digitalWrite(trig, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig, LOW);
      tiempo = pulseIn(echo, HIGH);
      distancia = tiempo/59;
      //Serial.println(distancia);
      
    }
    void rotacion_izquierda(void) {
      int *ptr_1;
      ptr_1 = &Qbot_vector[0];
      int temp = Qbot_vector[0];
      int *ptr_2;
      ptr_2 = &Qbot_vector[1];
      *ptr_1 = 0 * Qbot_vector[0] - 1 * Qbot_vector[1];
      *ptr_2 = 1 * temp + 0 * Qbot_vector[1];
    }
    void rotacion_derecha(void) {
      int *ptr_1;
      ptr_1 = &Qbot_vector[0];
      int *ptr_2;
      ptr_2 = &Qbot_vector[1];
      int temp = Qbot_vector[0];
      *ptr_1 = 0 * Qbot_vector[0] + 1 * Qbot_vector[1];
      *ptr_2 = -1 * temp + 0 * Qbot_vector[1];
    }
    int rotacion(int vect1[], int vect2[]) {
      int cross_product = vect1[0] * vect2[1] - vect1[1] * vect2[0];
      if (cross_product > 0) {
        rotacion_izquierda();
      } else if (cross_product < 0) {
        rotacion_derecha();
      }
      return cross_product;
    }
    void Qbot_adelante(void){
      digitalWrite(dir_motor_i,LOW);
      analogWrite(motor_i,85);
      digitalWrite(dir_motor_d,LOW);
      analogWrite(motor_d,85);
    }
    void Qbot_derecha(void){
      digitalWrite(dir_motor_i,HIGH);
      analogWrite(motor_i,0);
      digitalWrite(dir_motor_d,LOW);
      analogWrite(motor_d,155);
    }
    void Qbot_izquierda(void){
      digitalWrite(dir_motor_i,LOW);
      analogWrite(motor_i,155);
      digitalWrite(dir_motor_d,HIGH);
      analogWrite(motor_d,0);
    }
    void Qbot_detenerse(void){
      digitalWrite(dir_motor_i,LOW);
      analogWrite(motor_i,0);
      digitalWrite(dir_motor_d,LOW);
      analogWrite(motor_d,0);
    }
    void Qbot_acciones(char c){
      switch(c)
      {
        case 'a' : //adelante
          Qbot_adelante();
          break;
        case 'd': //derecha
          Qbot_izquierda();
          break;
        case 'i':
          Qbot_derecha();
          break;
        case 'e':
          Qbot_detenerse();
          break;  
      }
    }
    char Qbot_v_t_c(int v){

      if (v > 0){
        rotacion_izquierda();
        return 'i';
              
        //adelante
      }

      else if(v < 0){
        rotacion_derecha();
        return 'd';
        //derecha
      }
      else  {
        return 'a';
      }
        
    } 
    void giro_180(void){
      Qbot_BT.send("180° turn"); 
      Qbot_acciones('i');
      delay(900);

      while(digitalRead(infrarojo_d) == LOW && digitalRead(infrarojo_i) == LOW);
      Qbot_acciones('d');
      delay(200);

      while(digitalRead(infrarojo_d) == HIGH && digitalRead(infrarojo_i) == HIGH);
      Qbot_acciones(Qbot_instruction);

    }
    void user_interface(int i){
      bool flag_1 = false;
      bool flag_2 = false;

      switch(i){
        case 0:
          Qbot_BT.send("Insert a destination (1-10)");
          while(flag_1 == false ){
           
            if (Qbot_BT.msg.length() > 0){
              if(Qbot_BT.is_num() == true){
                int destino = Qbot_BT.numero();
                  /*Serial.println(destino);
                  Serial.println(Qbot_BT.msg );*/
                if (destino <= Qbot_graph.n_nodes ){
                    flag_1 = true;
                    Qbot_graph.end_node  = destino;
                    /*Qbot_BT.send("Q:");
                    Qbot_BT.BT.println(destino);*/
                }
                
                else{
                  Qbot_BT.send("Insert a valid value (1-10)");
                }
              }
              else{
                  Qbot_BT.send("Insert a number (1-10)");
              }    
            }
            Qbot_BT.msg = "";
            Qbot_BT.read();
          }
          break;
        case 1:
          Qbot_BT.send("Would you like to add a new destination?(yes/no)");
          Qbot_BT.read();
          while(flag_2 == false ){
            if (Qbot_BT.msg.length() > 0){
              String user_txt = Qbot_BT.read_msg();
              if(user_txt == "yes"){
                 flag_2 == true;
                 repeat = 0;
                 Qbot_BT.msg = "";
                 return user_interface(0);
              }
              else if(user_txt == "no"){
                 Qbot_BT.send("Ok, Have a nice day!");
                 repeat = 0;
                 flag_2 == true;
              }
              else{
                Qbot_BT.send("Insert a valid answer (yes/no)");
              }
            }
            Qbot_BT.msg = "";
            Qbot_BT.read();
          }

          break;
        case 2:
          Qbot_BT.send("Qbot has found an object");
      
       
          while(distancia > 10 && distancia < 25){
            Ultrasonic_sensor();    
          }
          if (distancia < 10){
            Qbot_BT.send("Qbot will stop");
            objeto_detectado = true;

          }
          
          break;

      }
    }

    int angle_between_vectors(int vector1[], int vector2[]){

      return (vector1[0]*vector2[0]+vector1[1]*vector2[1]);
    }
    void ruta_alternativa(void){
      Qbot_BT.send("Another path was found");

      int start = Qbot_graph.final_path[n_move - 1];
      /*Qbot_BT.BT.println("nodo incio");
      Qbot_BT.BT.println(start);*/
      int end = Qbot_graph.final_path[n_move];
      //Qbot_BT.BT.println(end);
      Qbot_graph.Matrix_graph[start][end][0] = 0;
      Qbot_graph.Matrix_graph[end][start][0] = 0;
      Qbot_graph.clear_arrays();
      Qbot_graph.instructions();
      Qbot_BT.send("180° turn"); 
      Qbot_BT.BT.println(Qbot_graph.start_node); 
      Qbot_acciones('i');
      delay(900);

      while(digitalRead(infrarojo_d) == LOW && digitalRead(infrarojo_i) == LOW);
      Qbot_acciones('d');
      delay(200);
      while(digitalRead(infrarojo_d) == HIGH && digitalRead(infrarojo_i) == HIGH);
      Qbot_acciones('a');


      Qbot_vector[0] = -1* Qbot_vector[0];
      Qbot_vector[1] = -1* Qbot_vector[1];
      n_move = 0;
    }
    void delay_intersection(char c){
      switch(c){
        case 'a':
          delay(100);
          break;
        case 'i':
          delay(70);
          break;
        case 'd':
          delay(70);
          break;
        default:
          delay(100);
      }
    }
};