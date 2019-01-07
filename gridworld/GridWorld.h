#include "DoublyLinkedNode.h"
#include "GWInterface.h"
#include "List.h"
#include <vector>
#include <iostream>

using std::vector;


class GridWorld : public GWInterface {

 public:
  GridWorld(unsigned nrows, unsigned ncols)   {
    this->rows = nrows;
    this->cols = ncols;
    id_counter = 0;
    this->count = 0; // our population 
    districts = new District*[rows];//get the districts up
    for(int i = 0; i < rows; i++) {
      districts[i] = new District[cols]; 
    }
    for(int i = 0 ; i < rows; i++){
      for(int j = 0; j < cols; j++){
	districts[i][j].pop = 0; // set up pop and polis
	districts[i][j].polis = new DoublyLinkedList<Person*>(); 
      }
    }
    dead = new List<Person*>; 

  }
  
  ~GridWorld(){
    for(int i = 0; i < alive.size(); i++){
      delete alive[i];
    }
    for(int i = 0; i < this->rows; i++){
      for(int j = 0; j < this->cols; j++){
	DoublyLinkedNode<Person*>* u = districts[i][j].polis->head;
	while(districts[i][j].polis->head){
	  u =  districts[i][j].polis->head->next;
	  delete  districts[i][j].polis->head->info;
	  delete  districts[i][j].polis->head;
	  districts[i][j].polis->head = u; 
	}
	//delete u; 
      }
    }
    for(int i = 0; i < cols; i++){
      delete[] districts[i];
    }
    delete districts;
    districts = nullptr;
    /*while(dead->front){
      Node<Person*>* u = dead->front->next;
      delete dead->front->data;
      delete dead->front;
      dead->front = u; 
    }
    */
    delete dead; 
    
     
  }
  
  bool birth(int row, int col, int &id){
    if(row < this->rows && col< this->cols){
      if(dead->length() == 0){
	Person* p = new Person;
	p->alive = true;
	p->row = row;
	p->col = col;
	p->id = id_counter;
        id_counter++;
	id = p->id;
	p->point = districts[row][col].polis->push_back(p);
	alive.push_back(p);
      }
      else{
	alive[dead->front->data->id] ->alive = true; 
       
	alive[dead->front->data->id]->row = row;
	alive[dead->front->data->id]->col = col;
	id =  dead->front->data->id; 
	alive[dead->front->data->id]->point =  districts[row][col].polis->push_back(dead->front->data);
	//alive.push_back(dead->front->data); 
	dead->pop_front(); 
      }
      
      districts[row][col].pop++;
      count++;
      return true;
    }
    return false;
    
  }
  
  bool death(int personID){
    int r, c;
    r = alive[personID]->row;
    c = alive[personID]->col; 
    if(alive[personID]->alive){
      alive[personID]->alive = false;
      //std::cout<<"after making false"<<std::endl;
      if(alive[personID]->point->next == nullptr){
	districts[r][c].polis->head = nullptr; 
      }else{
	alive[personID]->point->prev->next = alive[personID]->point->next;
      }
      //std::cout<<" after rem,oving form polis"<<std::endl; 
      districts[r][c].pop--;
      //std::cout<<"after down pop"<<std::endl; 
      Person* p = alive[personID]; 
      dead->push_back(p);
      count--;
      //std::cout<<"death end"<<std::endl;  
      return true; 
    }
    
    return false;
  }
  
  bool whereis(int id, int &row, int &col)const{
    if(alive[id]->alive){
	
      row = alive[id]->row;
      col = alive[id]->col;
      
      return true;
    }
    return false; 
  }
  
  bool move(int id, int targetRow, int targetCol){
    int r, c;
    r = alive[id]->row; 
    c = alive[id]->col; 
    int tc, tr;
    tc = targetCol;
    tr = targetRow; 
    if(alive[id]->alive && (targetRow <  this->rows) && (targetCol < this->cols)){
      alive[id]->row = targetRow;
      alive[id]->col = targetCol;
      alive[id]->point = districts[tr][tc].polis->push_back(alive[id]);       
      return true; 
    }
    return false;
  }
  
  std::vector<int> * members(int row, int col)const{
    vector<int>* ids = new vector<int>;
    DoublyLinkedNode<Person*>* t = districts[row][col].polis->head; 

    while(t){
      ids->push_back(t->info->id);
      t = t->next; 
    }
    return ids;
  }

  int population()const{
    return this->count;
  }
  
  int population(int row, int col)const{
    return districts[row][col].pop;
  }
  
  int num_rows()const {
    
    return this->rows;
  }
  int num_cols()const {
    return this->cols;
  }
  
  
 private:
  struct Person{
    int col;
    int row;
    int id;
    bool alive;
    DoublyLinkedNode<Person*>* point; 
  };
  struct District{
    int pop; 
    DoublyLinkedList<Person*>* polis; 
  };
  int rows;
  int cols;
  vector<Person*> alive; 
  District** districts;
  List<Person*>* dead; 
  int count;
  int id_counter; 
  
};
