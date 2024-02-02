#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Delaunay;

typedef K::Point_2 P;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

typedef struct{
  int from;
  int to;
  long sq_len;
} edge_t;

struct compEdges{
  bool operator()(edge_t &e1, edge_t &e2){
    return e1.sq_len < e2.sq_len;
  }
};

typedef struct{
  P point;
  long sq_d;
} bone_t;

struct compBones{
  bool operator()(bone_t &b1, bone_t &b2){
    return b1.sq_d < b2.sq_d;
  }
};

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, m; long s; int k;  //n = trees, m = bones, s = 4*r^2, k = min bones
    std::cin>>n>>m>>s>>k;

    //Save the location of the trees
    std::vector<std::pair<P, int>> trees(n);
    for(int i = 0; i < n; i++){
      int x, y;
      std::cin>>x>>y;

      trees[i] = {P(x, y), i};
    }

    Delaunay tri;
    tri.insert(trees.begin(), trees.end());

    //Create a vector containing all the edges that are covered by a shadow
    //And a vector contain all the edges
    std::vector<edge_t> covered_edges;
    std::vector<edge_t> all_edges;

    for (auto e = tri.finite_edges_begin(); e != tri.finite_edges_end(); e++) {
      int i1 = e->first->vertex((e->second+1)%3)->info();
      int i2 = e->first->vertex((e->second+2)%3)->info();

      long sq_len = tri.segment(e).squared_length();

      //Covered if l/2 <= r, l^2 <= s
      if(sq_len <= s){
        covered_edges.push_back({i1, i2, sq_len});
      }
      all_edges.push_back({i1, i2, sq_len});
    }

    //TASK 1

    //Find how many bones are covered by each tree
    //If a bone is covered by multiple trees, it's not a problem to consider only the
    //closest tree because, if a bone is covered by two shadows, it's possible to move from one tree to the other

    std::vector<int> bones_per_tree(n, 0);
    std::vector<bone_t> bones(m);  //Each bone also contains the distance to its closest tree
    for(int i = 0; i < m; i++){
      int x, y;
      std::cin>>x>>y;

      auto nearest = tri.nearest_vertex(P(x, y));
      long sq_d = CGAL::squared_distance(P(x, y), nearest->point());

      //It may be possible that the bone is not covered by the shadow of the nearest tree
      if(sq_d <= s/4){
        bones_per_tree[nearest->info()]++;
      }
      bones[i] = {P(x, y), sq_d};
    }

    //Build the graph with edges convered by a shadow
    graph G(n);
    for(auto edge: covered_edges){
      boost::add_edge(edge.from, edge.to, G);
    }

    //Find the sub-forests that can be walked under a shadow (connected components)
    std::vector<int> cc_map(n);
    int ncc = boost::connected_components(G, boost::make_iterator_property_map(cc_map.begin(),
      boost::get(boost::vertex_index, G)));

    //Find how many bones are covered by each connected component
    std::vector<int> bones_per_cc(ncc, 0);
    for(int i = 0; i < n; i++){
      int tree_cc = cc_map[i];

      bones_per_cc[tree_cc] += bones_per_tree[i];
    }

    //Find the connected component with the maximum amount of trees
    int max_bones_covered = *std::max_element(bones_per_cc.begin(), bones_per_cc.end());

    //SECOND TASK
    std::sort(all_edges.begin(), all_edges.end(), compEdges());
    std::sort(bones.begin(), bones.end(), compBones());

    auto edge_it = all_edges.begin();
    auto bone_it = bones.begin();

    std::vector<int> new_bones_cc(n, 0);  //Store how many bones are covered in each cc at a time
    boost::disjoint_sets_with_storage<> new_cc(n);
    int max_bones = -1;
    long max_4sq_d = -1;
    bool found = false;

    while(!found){

      //Increase the radius to include more bones directly
      if(bone_it != bones.end()){
        int near_tree = tri.nearest_vertex(bone_it->point)->info();
        int cc_near_tree = new_cc.find_set(near_tree);

        //Increase the number of bones covered by the component where the nearest tree to the new bone is present
        new_bones_cc[cc_near_tree]++;

        max_4sq_d = std::max(max_4sq_d, 4*bone_it->sq_d);
        max_bones = std::max(max_bones, new_bones_cc[cc_near_tree]);

        if(max_bones >= k){
          found = true;
          break;
        }

        bone_it++;
      }

      //While there are more connections and
      //Either there are no more bones to be added directly or a new bone was added directly
      while(edge_it != all_edges.end() &&
        ((bone_it < bones.end() && edge_it->sq_len < 4*(bone_it->sq_d)) ||
        bone_it == bones.end())){

        int cc1 = new_cc.find_set(edge_it->from);
        int cc2 = new_cc.find_set(edge_it->to);

        if(cc1 != cc2){
          new_cc.link(cc1, cc2);

          //Update the number of bones covered by the components
          //It's not certain which will be the final component identifier, so update the value of both
          new_bones_cc[cc1] += new_bones_cc[cc2];
          new_bones_cc[cc2] = new_bones_cc[cc1];
        }

        max_4sq_d = std::max(max_4sq_d, edge_it->sq_len);
        max_bones = std::max(max_bones, new_bones_cc[cc1]);

        if(max_bones >= k){
          found = true;
          break;
        }

        edge_it++;
      }

      if(found){
        break;
      }
    }

    std::cout<<max_bones_covered<<" "<<max_4sq_d<<"\n";
  }
}
