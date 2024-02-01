#include <iostream>
#include <string>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

typedef std::vector<std::string> VS;
typedef std::vector<VS> VVS;

void create_matches(int s1, int s2, int c, int f, VVS &hobbies, graph &G){
  int i1 = 0, i2 = 0;

  int match = 0;
  //Find the number of matching hobbies.
  //Make use of the fact that hobbies are ordered
  while(i1 < c && i2 < c){
    if(hobbies[s1][i1] == hobbies[s2][i2]){
      match++;
      i1++; i2++;
      continue;
    }

    if(hobbies[s1][i1] < hobbies[s2][i2]){
      i1++;
      continue;
    }

    if(hobbies[s1][i1] > hobbies[s2][i2]){
      i2++;
      continue;
    }
  }

  //Connect the two children if they have enough hobbies in common
  if(match > f){
    boost::add_edge(s1, s2, G);
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, c, f;  //n = students, c = characteristics, f = common
    std::cin>>n>>c>>f;

    VVS hobbies(n, VS(c));
    for(int i = 0; i < n; i++){
      for(int j = 0; j < c; j++){
        std::string s;
        std::cin>>s;

        hobbies[i][j] = s;
      }

      //Speed-up search by ordering the hobbies of each child
      std::sort(hobbies[i].begin(), hobbies[i].end());
    }

    graph G(n);

    for(int i = 0; i < n; i++){
      for(int j = i+1; j < n; j++){
        create_matches(i, j, c, f, hobbies, G);
      }
    }

    //Find if it's possible to create pairs for all the children
    std::vector<vertex_desc> mate_map(n);
    boost::edmonds_maximum_cardinality_matching(G,
      boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

    int matching_size = boost::matching_size(G,
      boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

    if(matching_size == n/2){
      std::cout<<"not optimal\n";
    }else{
      std::cout<<"optimal\n";
    }
  }
}
