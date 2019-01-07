#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <fstream>

using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;

#define UNDISCOVERED 'u'
#define DISCOVERED   'd'
#define ACTIVE       'a'
#define FINISHED     'f'

/*
 * function:  pvec
 * description:  utility function that prints the elements of
 *   a vector: one per line.
 * 
 * Note that this is a templated function; only works if the type
 *   T is acceptable with:
 *
 *     cout << var_of_type_T
 */
template <typename T>
void pvec(const std::vector<T> & vec) {

  for(const T &x : vec) {
    std::cout << x << "\n";;
  }
}


/*
 * class:  graph
 *
 * desc:   class for representing directed graphs.  Uses the
 *   adjacency list representation.
 */
class graph {

  private:

    // note:  this struct does not store both
    //   vertices in the edge -- just one.  This
    //   is because of the overall structure of
    //   the adjacency list organization:  an
    //   edge struct is stored in a vector associated
    //   with the other vertex.
    struct edge {
      int vertex_id;
      double weight;
      edge ( int vtx_id=0, double _weight=1.0) 
        : vertex_id { vtx_id}, weight { _weight} 
      { }
    };

    // a vertex struct stores all info about a particular
    //    vertex:  name, ID, incoming and outgoing edges.
    struct vertex {
      int id;
      vector<edge> outgoing;
      vector<edge> incoming;
      string name;

      vertex ( int _id=0, string _name="") 
        : id { _id }, name { _name } 
      { }
    };

    /**************************************************
    *   Data members of the Graph class here!
    *
    *   Everything about a graph is accessible via
    *    these three data structures!
    ***************************************************/

    // _name2id:
    // Each vertex in a graph is identified in two ways:
    //      - by its unique 'name' which is a string (so things are
    //        friendly to the outside world).
    //      - by its unique integer ID which is more convenient 
    //        internally.  If a graph has N vertices, the 
    //        corresponding IDs are ALWAYS 0..N-1.
    // _name2id is an unordered_map (hash map) which lets us easily
    //       retrieve the vertex ID associated with a given vertex
    //       name (it maps from strings to integers).
    unordered_map<string, int> _name2id;

    // vertices:
    //   vertices is the primary data structure:  it is an  implementation
    //      of a pretty standard adjacency list.
    //   It is indexed by vertex ID.
    //   vertices[u] contains everything we need to know about vertex u:
    //       - name (string)
    //       - ID (int).  Somewhat redundant since vertices[u].id == u
    //       - outgoing edges (as a vector of edge structures)
    //       - incoming edges (as a vector of edge structures)
    //       
    //   See struct vertex above
    vector<vertex> vertices;

    // the unordered set edges isn't going to be of much interest
    //   to you.  Its main purpose is to detect duplicate edges
    //   while building a graph (see add_edge)..
    // Notes:  the data structure is an unordered_set which is
    //   really a hash table.  It stores a unique string 
    //   representation of already added edges; it allows add_edge
    //   to detect if an edge already exists efficiently.
    unordered_set<string> edges;

  public:

    // this struct is used for capturing the results of an operation.
    // typically a "report" will be a vector of vertex_labels indexed
    // by vertex-id.
    struct vertex_label {
      double dist;
      int pred;
      char state;
      int npaths;
      
      vertex_label( double _dist=0.0, int _pred=-1, char _state='?',
          int _npaths=0) 
        : dist { _dist }, pred { _pred }, state { _state}, npaths { 0 }
      { }

    };


    graph() {}

    ~graph() {}

  private:

    int add_vertex(const string &name) {
      int id = vertices.size();
        vertices.push_back(vertex(id, name));
        _name2id[name] = id;
        return id;
    }

    /*
     * function:  edge_string
     *
     * returns concatenation of src and dest vertex strings with
     * a single space between
     *
     * Purpose:  gives a unique string representing the edge
     * -- data member edges stores sets of such strings to
     * quickly detect if an edge has already been created.
     *
     */
    static
    string edge_string(const string &src, const string &dest) {
      return src + " " + dest;
    }


    /*
     * function: p_edge
     * desc:  simple function for printing an edge
     */
    void p_edge(edge &e) {
      std::cout << "(" << id2name(e.vertex_id) 
        << ", " << e.weight << ") ";
    }

  public:

    /*
     * func:  id2name
     * desc:  returns vertex name (a string) associated with given 
     *         vertex id.
     *
     *         If id not valid for given graph, the string "$NONE$"
     *         is returned.
     */
    string  id2name(int id) {
      if(id<0 || id>=vertices.size())
        return "$NONE$";
      return vertices[id].name;
    }

    /*
     * func: name2id
     * desc: returns integer vertex id of given vertex name.
     *       If there is no such vertex in the graph, -1 is returned.
     */
    int name2id(const string &vtx_name) {
      if(_name2id.count(vtx_name)==0)
        return -1;
      return _name2id[vtx_name];
    }

    /*
     * func: name_vec2string
     * desc: utility function - if you have a bunch of
     *   vertex names (as strings) stored in a vector, this
     *   function puts the names in a single string with
     *   nodes separated by single spaces.
     *
     *   Might be handy for things like getting an easy to
     *   print representation of a path for example.
     */
    string name_vec2string(const vector<string> &vec) {
      string s = "";
      int i;

      if(vec.size()==0)
        return s;

      s = s + vec[0];
      for(i = 1; i<vec.size(); i++) {
        s = s + " " + vec[i];
      }
      return s;
    }

    /*
     * func: id_vec2string
     * desc: utility function - if you have a bunch of
     *   vertex ids (ints) stored in a vector, this
     *   function connverts them to names and builds a in a 
     *   single string with nodes-names separated by single spaces.
     *
     *   Might be handy for things like getting an easy to
     *   print representation of a path for example.
     */
    string id_vec2string(const vector<int> &vec) {
      string s = "";
      int i;

      if(vec.size()==0)
        return s;

      s = s + id2name(vec[0]);
      for(i = 1; i<vec.size(); i++) {
        s = s + " " + id2name(vec[i]);
      }
      return s;
    }




    /*
     * func: add_edge
     * desc: adds edge (src,dest) with given weight to graph if
     *   possible.
     *
     *       If edge (src,dest) is already in graph, the graph is
     *       unchanged and false is returned.
     *
     *       Otherwise the edge is added and true is returned.
     *
     *       Note:  if src and/or dest are not currently vertices
     *         in the graph, they will be added.
     */
    bool add_edge(const string &src, const string &dest, 
        double weight=1.0) {

      int s_id, d_id;

      string estring = edge_string(src, dest);

      if(edges.count(estring)==1) {
        std::cerr << "warning: duplicate edge '"
          << estring << "'\n";
        return false;
      }

      edges.insert(estring);

      // get id for source vertex
      if(_name2id.count(src)==0) 
        s_id = add_vertex(src);
      else
        s_id = _name2id[src];

      // get id for destination vertex
      if(_name2id.count(dest)==0) 
        d_id = add_vertex(dest);
      else
        d_id = _name2id[dest];

      vertices[s_id].outgoing.push_back(edge(d_id, weight));
      vertices[d_id].incoming.push_back(edge(s_id, weight));

      return true;
    }


    /*
     * func: add_edge(string &)
     * desc: takes an edge specification as a single string, 
     *   parses the string into src vertex, dest vertex and
     *   weight (optional).
     *
     *   If parsing is successful, add_edge(string, string, double) above
     *   is called to do the "real work".
     *
     * returns true on success; false on failure (parse error or
     *   call to add_edge failed).
     *
     * expected format:
     *
     *   the given string must have either two or three tokens (exactly).
     *
     *   If it has three tokens, the third token must be parseable as
     *   a double.
     */
    bool add_edge(const string &str) {
      std::stringstream ss(str);
      string src, dest, junk, weight_str;
      double weight;

      if(!(ss >> src))
        return false;
      if(!(ss >> dest))
        return false;
      if(!(ss >> weight_str)){
        // only two tokens: use default weight
        weight = 1.0;
      }
      else {
        if(!(std::stringstream(weight_str) >> weight)){
          // couldn't parse weight
          return false;
        }

        if(ss >> junk){
          // extratoken?  format error
          return false;
        }
      }

      add_edge(src, dest, weight);

      return true;
    }

    void _add_edge(const string &str) {

      if(!add_edge(str))
        std::cout << "add_edge failed; str='" <<
          str << "'\n";
    }

    void display(){
      int u;

      for(u=0; u<vertices.size(); u++) {
        std::cout << vertices[u].name << " : ";

        for(edge &e : vertices[u].outgoing) 
          p_edge(e);
        std::cout << "\n";
      }
    }

    /*
     * func: ids2names
     * desc: utility function which takes a vector of vertex IDs
     *   and populates another vector of strings with the corresponding
     *   vertex names.
     */
    void ids2names(std::vector<int> &  ids, std::vector<string> & names) {
      names.clear();

      for(int &u : ids) {
        names.push_back(id2name(u));
      }
    }

    /* 
     * func: read_file
     * desc: reades given file (if possible) as a 
     *   sequence of edges -- one edge per line.
     *
     *   Each line is expected to be in the form:
   
           <source-vertex> <dest-vertex> {<weight>}
     *
     * where the vertices are given as strings and
     *   the edge weight is a number (read as a double).
     * The edge weight is optional (indicated by {}).
     *
     * Examples:
         an edge from Chicago to NewYork with weight 201.9:

            Chicago NewYork 201.9
 
         an edge from Bob to Alice with no weight:

             Bob Alice

     * if no weight is specified, the edge defaults to a weight
     *   of 1.0
     */
    bool read_file(const string &fname) {
      std::ifstream file;
      string line;

      file.open(fname, std::ios::in);
      if(!file.is_open())
        return false;
      
      while(getline(file, line)) {
        // skip blank lines
        if(line.length() > 0) {
          if(!add_edge(line)) {
            std::cerr << "warning: skipped input line '" 
              << line << "' (ill-formatted)\n";
          }
        }
      }
      file.close();
      return true;
    }


    int num_nodes() {
      return vertices.size();
    }
    int num_edges() {
      return edges.size();
    }

  private:
    void init_report(std::vector<vertex_label> & report) {
      int u;
      report.clear();
      for(u=0; u<vertices.size(); u++) {
        report.push_back(vertex_label(-1, -1, UNDISCOVERED));
      }
    }


  public:
    /*
     * TODO 10 points
     *
     * modify bfs so that vertex labels reflect the NUMBER OF 
     *   SHORTEST PATHS TO THE VERTEX LABELED:
     *
     *     report[u].npaths is assigned the number of shortest 
     *        paths from src to u.
     *
     *   OBSERVATIONS:
     *
     *     report[src].npaths will be 1.
     *
     *     if a vertex u is not reachable from src, then 
     *     report[u].npaths will be assigned 0. 
     *
     * RUNTIME:  bfs must still be O(V+E).
     *
     */   
    bool bfs(int src, std::vector<vertex_label> &report) {
      int u, v;
      std::queue<int> q;

      if(src < 0 || src >= num_nodes())
        return false;

      init_report(report);

      report[src].dist = 0;
      report[src].npaths = 1; 
      // since src is the root of the bfs tree, it has no 
      //   predecessor.
      // By convention, we set the predecessor to itself.
      report[src].pred = src;
      report[src].state = DISCOVERED;
      q.push(src);

      while(!q.empty()) {
        // dequeue front node from queue
        u = q.front();
        q.pop();

        // examine outgoing edges of u
        for(edge &e : vertices[u].outgoing) {
	  
          v = e.vertex_id;
	 
          if(report[v].state == UNDISCOVERED) {
            report[v].dist = report[u].dist + 1;
            report[v].pred = u;
            report[v].state = DISCOVERED;
	    report[v].npaths = 1; 
            // enqueue newly discovered vertex
            q.push(v);
          }
	  else if(report[v].state== DISCOVERED  ){
	    report[v].npaths++;
	  }
        }
      }
      return true;
    }

    bool bfs(const string src, std::vector<vertex_label> &report) {
      int u;

      if((u=name2id(src)) == -1)
          return false;
      bfs(u, report);
      return true;
    }

  private:
    void _dfs(int u, vector<vertex_label> & rpt, bool &cycle) {
      int v;

      rpt[u].state = ACTIVE;
      for(edge &e : vertices[u].outgoing) {
        v = e.vertex_id;
        if(rpt[v].state == UNDISCOVERED) {
          rpt[v].pred = u;
          rpt[v].dist = rpt[u].dist + 1;
          _dfs(v, rpt, cycle);
        }
        if(rpt[v].state == ACTIVE) 
          cycle = true;
      }
      rpt[u].state = FINISHED;
    }

  public:
    bool dfs(int u, vector<vertex_label> & rpt, bool &cycle) {

      if(u < 0 || u >= num_nodes()) 
        return false;

      cycle = false;

      init_report(rpt);
      rpt[u].pred = u;
      rpt[u].dist = 0;
      _dfs(u, rpt, cycle);
      return true;
    }

    bool dfs(const string &src, vector<vertex_label> & rpt, bool &cycle) {
      int u;

      if((u=name2id(src)) == -1)
          return false;
      dfs(u, rpt, cycle);
      return true;
    }

    bool has_cycle() {
      int u;
      bool cycle=false;
      vector<vertex_label> rpt;

      init_report(rpt);
      for(u=0; u<num_nodes(); u++) {
        if(rpt[u].state == UNDISCOVERED) {
          _dfs(u, rpt, cycle);
          if(cycle)
            return true;
        }
      }
      return false;
    }

    bool topo_sort(std::vector<int> &order) {
      std::queue<int> q;
      std::vector<int> indegrees;
      int u, v;
      int indeg;

      order.clear();
      if(has_cycle())
        return false;

      for(u=0; u<num_nodes(); u++) {
        indeg = vertices[u].incoming.size();

        indegrees.push_back(indeg);
        if(indeg==0)
          q.push(u);
      }

      while(!q.empty()){
        u = q.front();
        q.pop();
        order.push_back(u);
        for(edge &e : vertices[u].outgoing) {
          v = e.vertex_id;
          indegrees[v]--;
          if(indegrees[v]==0) 
            q.push(v);
        }
      }
      return true;
    }



    void disp_report(const vector<vertex_label> & rpt, 
        bool print_paths=false) {
      int u;
      vector<int> path;

        // THIS if STATEMENT IS NEW
        if(rpt.size() != num_nodes()) {
          std::cerr << "error - disp_report(): report vector has incorrect length\n";
          return;
        }

        for(u=0; u<num_nodes(); u++) {
          std::cout << id2name(u) << " : dist=" <<  rpt[u].dist
            << " ; pred=" <<  id2name(rpt[u].pred) << 
            " ; state='" << rpt[u].state << "'; npaths=" << 
            rpt[u].npaths << "\n";
          if(print_paths) {
            extract_path(rpt, u, path);
            std::cout << "     PATH: <" + id_vec2string(path) + ">\n";
          }
        }
    }

    /******************************************************
     *
     * Vocabulary:  
     *
     *   In a DAG G:
     *   
     *       inputs:  subset of vertices with INDEGREE ZERO
     *
     *       outputs: subset of vertices with OUTDEGREE ZERO
     *
     *       input-path: a path in G STARTING AT AN INPUT VERTEX
     *          (and ending at any vertex).
     *
     *       output-path:  a path in G starting at any vertex and
     *          ENDING AT AN OUTPUT VERTEX.
     *
     *       input-output-path (or io-path):  a path STARTING AT
     *          AN INPUT VERTEX _AND_ ENDING AT AN OUTPUT VERTEX.
     *
     */

    /* TODO 20 points
     * function:  extract_path
     * desc:  extracts the path (if any) encoded by vertex labels
     *        ending at vertex dest (as an int ID).  Resulting path
     *        is stored in the int vector path (sequence of vertex
     *        IDs ENDING WITH dest -- i.e., in "forward order").
     *
     *     parameters:
     *       rpt:  vector of vertex labels associated with given
     *             graph (calling object).  Presumption:  labels
     *             have been previously populated by another function
     *             like bfs, dfs, or critical_paths.
     *
     *       dest: vertex ID of the target/destination vertex.
     *
     *       path: int vector in which the constructed path is stored.
     *
     * returns:  true on success; false otherwise.
     *           failure:  there is no encoded path ending at vertex
     *              dest (see discussion below);
     *              OR, the rpt vector is not of the correct dimension.
     *
     * Notes:  predecessor conventions:
     *
     *      SOURCE VERTICES:
     *
     *         if vertex u is a "source" vertex such as:
     *
     *             the source vertex of BFS or DFS or
     *             an input vertex in a DAG (perhaps analyzed by 
     *                dag_critical_paths).
     *
     *         then the predecessor of u is u itself:
     *
     *              rpt[u].pred==u
     *
     *      UNREACHABLE VERTICES:
     *
     *          if rpt[u].pred == -1, this indicates that THERE IS 
     *          NO PATH ENDING AT VERTEX u.
     *
     *          In this situation, the path vector is made empty and
     *          false is returned.
     *
     *  RUNTIME:  O(|p|) where |p| is the number of edges on 
     *    the path extracted.
     *
     */
    bool extract_path(const vector<vertex_label> & rpt, int dest, vector<int> & path) {
      std::vector<int> t;
      int i = dest; 
      path.clear();
      if(rpt.size() != num_nodes())
        return false;
      
      while(rpt[i].pred != i){
	if(rpt[i].pred == -1){path.clear(); return false;}
	t.push_back(rpt[i].pred);
	i = rpt[i].pred; 
      }      
      for(int k = t.size() - 1; k >= 0; k--){
	path.push_back(t[k]);
      }
      // your code here!
      return true;  // placeholder
    }

    /*
     *  TODO 30 points
     *
     *  func: dag_critical_paths
     *  desc: for each vertex u, the length of the critical (LONGEST)
     *        input-path ENDING AT u.
     *
     *        The "length" of a path is the SUM OF THE WEIGHTS OF THE
     *        EDGES ON THE PATH.
     *
     *        On completion, the results are stored in the vector rpt.
     *        For each vertex u (as an intID),
     *
     *          rpt[u].dist  stores the length of the longest (critical)
     *            input-path ending at vertex u.
     *
     *          rpt[u].pred  stores the predecessor vertex of u on a 
     *            critical/longest input path ending at u.  If there
     *            are multiple such paths (having equal maximum length)
     *            there may be multiple correct predecessors.
     *
     *  returns:  true on success (as long as graph is a DAG).
     *            false if graph is not a DAG.
     *
     *  runtime:  O(V+E)
     */
    bool dag_critical_paths(vector<vertex_label> & rpt) {

      if(has_cycle())
        return false;
      // your code here...
      vector<int> t; 
      for(int i = 0; i < num_nodes(); i++){
	t.push_back(i); 
      }
      topo_sort(t);
       
      for(int i = 0; i < t.size(); i++){
	int max = 0; 
	if(vertices[t[i]].incoming.size() == 0){
	  rpt[t[i]].dist = 0; 
	}
	
	else{
	  for(int j = 0; j < vertices[t[i]].incoming.size(); j++){
	    if(max < vertices[t[i]].incoming[j].weight){max =  vertices[t[i]].incoming[j].weight; }
	  }
	  rpt[t[i]].dist = max +  rpt[rpt[t[i]].pred].dist; 
	}
      }
      
      return true;
    }
    int findMax(vector<edge> in){
      //std::cout<<"in max" <<std::endl;

      //if(in.size() == 0){return 0;}
      //if(in.size() == 1){return 1; }
      double max = in[0].weight;
      int maxIndex = 0; 
      for(int i = 1; i < in.size(); i++){
	//std::cout<<"in max for" <<std::endl;

	if(in[i].weight > max){
	  max = in[i].weight;
	  maxIndex = in[i].vertex_id;
	}
      }
      return maxIndex; 
    }
    /*
     *  TODO 30 points
     *  function:  dag_num_paths
     *  desc:  if given graph (calling object) is a DAG, the vector
     *         rpt is populated such that:
     *
     *           rpt[u].npaths = number of io-paths passing through
     *                            vertex u.
     *
     *           Recall: an IO path starts at an input vertex and
     *             ends at an output vertex.
     *
     *           This value is defined for all vertices u in the
     *             graph (inputs, outputs and "intermediate" nodes).
     *
     *  NOTES:  rpt[u].pred, and rpt[u].dist have no partiular 
     *          meaning after this operation.
     *
     *  EXAMPLE:

                         a  b  c
                         \  |  /
                          \ | /
                            d
                           / \
                          e   f
                           \ /
                            g
                           / \
                          h   i
                           \ /
                            j

            There are 3 input nodes (a,b,c) and one output node (j)
            in this graph.

            There are 12 distinct io-paths passing through vertex d in 
            this dag (note:  edges are pointed downward)

            Can you enumerate them?

     *
     *  returns true if graph is a DAG; false otherwise.        
     *
     *  RUNTIME:  O(V+E)  -- Note: in general, the number of paths
     *                       in a graph may be exponential in the
     *                       number of vertices.
     *
     *                       This means that you cannot explicitly
     *                       enumerate all of the paths and count them!
     *                       (The enum_paths function below which DOES
     *                       enumerate a set of paths MAY take exponential
     *                       time).
     *
     * General Hint:  an io-path passing through a vertex u is 
     *   composed of an input-path ending at u, followed by an
     *   output path starting at u.  
     *
     *   Now, if you could figure out the number of input-paths
     *   ending at u and the number of output paths starting at u, 
     *   could you determine the number of io-paths passing through
     *   u?
     * 
     */
    bool dag_num_paths(vector<vertex_label> & rpt) {
      if(has_cycle())
        return false;
      // your code here...
      int count = 2; 
      for(int i = 0; i < rpt.size(); i++){
	if(rpt[i].pred == i){
	  rpt[i].npaths = 1;
	  //continue; 
	}
	//else{return false; }
	std::cout<<"dag num"<<std::endl; 
	count = rpt[rpt[i].pred].npaths; 
	count *= vertices[i].incoming.size();
	rpt[i].npaths = count; 
      }
      return true; 
    }

    /*
     * TODO 20 points
     * function:  valid_topo_order
     * desc:  determines if vertex sequence in the given vector
     *        (parameter order) is a valid topological ordering of
     *        the given graph (calling object).
     *
     *        returns true if it is; false otherwise.
     *
     * details:  returns false if graph is not a DAG.
     *
     *           Note that vertices are given as their integer IDs.
     *
     * RUNTIME:  O(V+E)
     */
    bool valid_topo_order(const vector<int> & order) {
      if(has_cycle())
        return false;
      bool visited[num_nodes()];
      for(int i = 0; i < order.size(); i++){
	visited[order[i]] = true;
	for(int j = 0; j < vertices[order[i]].incoming.size(); j++){
	  if(visited[vertices[order[i]].incoming[j].vertex_id] == false){
	    return false; 
	  }
	}							   
      }
      
      return true;

    }

    /*
     * TODO 30 points
     *
     * function:  enum_paths
     * desc:  enumerates all input-paths ending at target vertes in
     *        a DAG.
     * details:  Given a DAG and vertex target in the graph, the
     *   vector paths is populated with ALL input paths ending at
     *   vertex target.
     *
     * [NOTE:  target vertex is passed as its integer ID; however,
     *   vertices in paths constructed are represented by their 
     *   name -- as a string]
     *
     * A path is represented as a string containing the names of
     * each vertex (NOT intger vertex IDs) on the path in sequence; 
     * vertex names are separated by a single space.
     *
     * returns:  true on success; 
     *           false on failure (graph is not a DAG or target vertex ID
     *           is out of range).
     *
     * RUNTIME:  this one may be unavoidably exponential!
     *
     * EXAMPLES:
     *
     *   Chicago
     *   NewYork
     *   LosAngeles
     *
     * and there are edges:
     *
     *   LosAngeles Chicago
     *   Chicago NewYork
     *
     * The path LosAngeles to Chicago to NewYork is represented by the
     * string:
     *
     *   "LosAngeles Chicago NewYork"
     *
     * Another example:  the input file ex1A is a DAG.  Using vertex g
     * as the target (integer ID: 6), will result in the paths vector
     * containing the following strings:
     *

           "a d g"
           "a b d g"
           "a c d g"
           "a d e g"
           "a b d e g"
           "a c d e g"
           "a d f g"
           "a b d f g"
           "a c d f g"
     *
     * NOTE:  the concatenation operator '+' on strings might
     *   make some of your work pretty easy to code!
     *
     * COMMENT:  this function can be implemented with about
     *   20 lines of code.
     */
    bool enum_paths(int target, vector<string> &paths) {
      paths.clear();
      if(has_cycle() || target < 0 || target >= num_nodes())
        return false;

      // your code here!
      printAllPaths(0, target, paths ); 
      return true;
    }

    void printAllPaths(int s, int d, vector<string> &paths){
      bool *visited = new bool[num_nodes()];
      int *path = new int[num_nodes()];
      int path_index = 0;
      
      for (int i = 0; i < num_nodes(); i++){
        visited[i] = false;
      }
      printAllPathsUtil(s, d, visited, path, path_index,  paths);
    }
    void printAllPathsUtil(int u, int d, bool visited[], int path[], int &path_index,  vector<string> &paths){
      visited[u] = true;
      path[path_index] = u;
      path_index++;
      string s = " " ; 
      if (u == d){
	for (int i = 0; i<path_index; i++){
	  s +=  id2name(path[i]) +  " ";
	}
	paths.push_back( s); 
	
      }
      else{
	for (edge &e : vertices[u].outgoing){
	  int v = e.vertex_id; 
	    if (!visited[v])
	      printAllPathsUtil(v, d, visited, path, path_index,  paths);
	}
      }
      path_index--;
      visited[u] = false;
    }
  
    
    /*
     * (DONE)
     * func: enum_paths(string, vector<string> &)
     * desc: same as enum_paths(int, vector<string> &) above except
     *       target vertex is taken as its name (as a string).
     *
     *       Simply translates target vertex name to its integer id
     *       and calls enum_paths(int, vector<string> &) above.
     */
    bool enum_paths(const string &target,  vector<string> &paths) {
      int tgt;
      if((tgt=name2id(target)) == -1)
          return false;

      return enum_paths(tgt, paths);
    }
    



};
