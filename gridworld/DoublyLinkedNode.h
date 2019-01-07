#ifndef DLL
#define DLL
template <class T>

struct DoublyLinkedNode {
  T info;
  DoublyLinkedNode<T> *next, *prev;
  
  DoublyLinkedNode<T>() {
    next = prev = 0;
  }
  
  DoublyLinkedNode<T>(T nu, DoublyLinkedNode *ne = 0, DoublyLinkedNode *pre = 0) {
    info = nu;
    next = ne;
    prev = pre;
  }
};

template<class T>

class DoublyLinkedList { 
 public: 
  DoublyLinkedNode<T> *head;
  DoublyLinkedNode<T> *tail;
  int size; 
 
  DoublyLinkedList<T>() {
    head = tail = 0;
    size = 0; 
  }
  
  ~DoublyLinkedList<T>(){
    DoublyLinkedNode<T>* p; 
    while(head){ 
      p = head->next;
      delete head;
      head = p;
    }
  }
  
  bool isEmpty() const {
    return this->head == 0;
  }
  /*
  void remove(DoublyLinkedNode<T>* n ){
    if(n == this->head){
      pop_front();
    }
    else{
      
      n->prev->next = n->next; 
      delete n;  
    }
  }
  */

  DoublyLinkedNode<T>* push_back(DoublyLinkedNode<T>* n){
    if(head == 0){
      head = n = tail;
      head->next = nullptr;
      DoublyLinkedNode<T>* t = head;
      return t; 
    }
    else{
      this->tail->next = n;
      this->tail = this->tail->next; 
    }
  }

  
  DoublyLinkedNode<T>*  push_back(T n)
  {
    DoublyLinkedNode<T>* w; 
    if (tail != 0) {
      tail = new DoublyLinkedNode<T>(n, 0, tail);
      tail->prev->next = tail;
      w = tail;
      size++; 
      return w; 
    } else {
      head = tail = new DoublyLinkedNode<T>(n);
      w = tail;
      size++; 
      return w; 
    }
  }
  void pop_front()
  {
    if(head != nullptr) {
      DoublyLinkedNode<T> *tmp = head->next;
      delete head;
      head = tmp;
      head->prev = nullptr; 
      size--;
    }
  }
  /*
    int pop_back()
  {
    T el = tail->info;
    if(head == tail) {
      delete tail;
      head = tail = 0;
    } else {
      tail = tail->prev;
      delete tail->next;
      tail->next = 0;
    }
    return el;
  }
  */
  
  
  
  
};

#endif
