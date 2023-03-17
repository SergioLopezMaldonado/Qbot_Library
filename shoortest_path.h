
class Graph{
  public:
    const static int n_nodes  = 5;
    const static int n_edges = 5;
    
    int start_node;
    int end_node;
    int ele;
    int length_l;

    int edges[n_edges][4];
    int paths[n_nodes][1];
    int path[n_nodes];
    
    int final_path[n_nodes];
    int vectorial_path[n_nodes][2];

    int Matrix_graph[n_nodes][n_nodes][3];
    bool path_found = false;
    //init_paths(paths);
    //init_vector(path);
    void add_edges(void){
      for(int i = 0; i < n_edges; i++){
        add_edge(edges[i][0],edges[i][1],edges[i][2],edges[i][3],1);
        //add_edge(edges[i][1],edges[i][0],-1*edges[i][2],-1*edges[i][3],1);        
      }
    }
    void init_BFS(void){
      init_graph();
      add_edges();
    }
    void instructions(void){

      //Serial.println("pass");
      /*for(int i = 0; i< n_nodes; i++){
        for(int j = 0; j < n_nodes; j++){
          Serial.print("{");
          Serial.print(Matrix_graph[i][j][0]);
          Serial.print(",");
          Serial.print(Matrix_graph[i][j][1]);
          Serial.print(",");
          Serial.print(Matrix_graph[i][j][2]);
          Serial.print("}");
         
        }
        Serial.println("");
      }*/
      BFS(start_node, end_node);
      //Serial.println("pass");
      const int l = len(path, n_nodes);
      length_l = l;
      s_b_array(final_path,l, path);
      reverse_array(final_path,l);
      
      for (int i = 0; i < l;i++){
        Serial.println(final_path[i]);
      }

      int j = 0;
      
      while(j<l-1){
          int x = final_path[j];
          int y = final_path[j+1];
          vectorial_path[j][0] = Matrix_graph[x-1][y-1][1];
          vectorial_path[j][1] = Matrix_graph[x-1][y-1][2];
          Serial.print( vectorial_path[j][0]);
          Serial.print( ',');
          Serial.println( vectorial_path[j][1]);
          j++;
          
          //length_l = j;
      }



    }
    void clear_arrays(void){
      for (int node = 0; node < n_nodes; node++){
        paths[node][1] = 0;
        path[node] = 0;
        }
      
    }

  private:         
    bool in_array(int element, int arr[]){
      for (int e=0; e< n_nodes; e++){
        if(arr[e] ==  element){
          ele = e;
          return true;
        }
      }
      return false;
    }
    void remove_from_array(int e, int arr[]) {
      if (in_array(e, arr) == true) {
        int *ptr;
        // std::cout<<ele << "\n";
        for (int i = ele; i < n_nodes; i++) {
          if (i != n_nodes - 1) {
            ptr = &arr[i];
            *ptr = arr[i + 1];
          } else {
            ptr = &arr[i];
            *ptr = 0;
          }
        }
      }
    }
    void append_array(int e, int array[]) {
      bool t_ele = false;
      for (int i = 0; i < n_nodes; i++) {
        if (array[i] == 0 && t_ele == false) {
          array[i] = e;
          t_ele = true;
        }
      }
    }
    void reverse_array(int array[], int _size) {
      int *ptr;
      int *ptr_2;
      int temp;
      int loop;
      if (_size % 2 == 0) {
        loop = _size / 2;
      } else {
        loop = _size / 2 + 1;
      }
      for (int i = 0; i < loop; i++) {
        ptr = &array[_size - i - 1];
        ptr_2 = &array[i];
        temp = array[_size - i - 1];
        *ptr = array[i];
        *ptr_2 = temp;
      }
    }
    void init_vector(int vector[]){
      int *ptr;
      for(int i=0; i<n_nodes;i++){
        ptr =  &vector[i];
        *ptr = 0;
      }
    }

    void init_graph(void){
      for (int node = 0; node < n_nodes; node++){
        paths[node][1] = 0;
        path[node] = 0;
        for(int relation = 0; relation < n_nodes;relation ++){
          Matrix_graph[node][relation][0] = 0;
          Matrix_graph[node][relation][1] = 0;
          Matrix_graph[node][relation][2] = 0;
        }
      }
    }
    void add_edge(int start, int end, int vector_i, int vector_j, int value){
      for(int node = 0; node < n_nodes;node ++){
        if(node == end-1){
          Matrix_graph[start-1][end-1][0] = value;
          Matrix_graph[start-1][end-1][1] = vector_i;
          Matrix_graph[start-1][end-1][2] = vector_j ;
        }      
      }
    }

    void BFS(int start, int end){
      int initial_node = start;
      int frontier[n_nodes];
      int explored_set[n_nodes];
      int backtracking[n_nodes];
      int n;
      bool condition = false;
    
      init_vector(frontier);
      init_vector(explored_set);
      init_vector(backtracking);

      while(condition == false){
        if (initial_node == end){
          condition = true;
          path_found= true;
        }
        if(explored_set[n_nodes-1] != 0){
          condition = true;
          path_found = true;
        }    
        if(in_array(start, frontier)== true){
          remove_from_array(start, frontier);
        }
        if(in_array(start, frontier)== false){
          append_array(start, explored_set);
        }
        for(int n = 0; n < n_nodes; n++){
          if( Matrix_graph[start-1][n][0]== 1){
            if(in_array(n+1, frontier) == false && in_array(n+1, explored_set)== false){
              append_array(n+1, frontier);
              if(in_array(start, paths[n]) == false){
                append_array(start, paths[n]);
              }    
            }
          }
        }
        start = frontier[0];
      }
      n = end;
      append_array(n, path);
      while(n != initial_node){
        append_array(paths[n-1][0], path);
        n = paths[n-1][0];
      }
      //append_array(nodo_partida, path); 
    }
  
    int len(int arr[], int _size){
      for (int i = 0; i < _size; i++){
        if(arr[i] == 0){
          return i;
      
        }
      }
    }

    void s_b_array(int arr_1[], int size_1, int arr_2[]){
      int *ptr;
      for (int i = 0; i < size_1; i++){
        ptr = &arr_1[i];
        *ptr = arr_2[i];
      }
    }

};