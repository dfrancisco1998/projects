
#include <iostream>
#include "Graph.h"


int main(int argc, char *argv[]){
  graph g;
  std::vector<graph::vertex_label> bfs_rpt;
  std::vector<graph::vertex_label> dfs_rpt;

  if(argc != 3) {
    std::cout << "usage:  ./bfs <filename> <source-vertex>\n";
    return 0;
  }
  else {
    if(!g.read_file(argv[1])){
      std::cout << "could not open file '" << argv[1] << "'\n";
      return 0;
    }
  }

  std::cout << "\nADJACENCY-LIST REPRESENTATION:\n\n";
  g.display();
  std::cout << "\nEND ADJACENCY LIST:\n\n";

  if(!g.bfs(argv[2], bfs_rpt)) {
    std::cout << "bfs failed -- bad source vertex name '" <<
      argv[2] << "'\n";
    return 0;
  }
  else {
    std::cout << "\nBFS REPORT:\n\n";
    g.disp_report(bfs_rpt, true);
    std::cout << "\nEND BFS REPORT\n\n";
  }
  //g.dag_critical_paths(bfs_rpt);
  //g.disp_report(bfs_rpt);
  std::vector<int> order1;
  std::vector<int> order2;

  for(int i = 0; i <6; i++){
    order2.push_back(i); 
  }
  order1.push_back(0);
  order1.push_back(1);
  order1.push_back(2);
  order1.push_back(5);
  order1.push_back(3);
  order1.push_back(4);
  std::cout<< "O1 " <<g.valid_topo_order(order1) << std::endl;
  std::cout<< "O2 " << g.valid_topo_order(order2) << std::endl;
  return 0;
}

