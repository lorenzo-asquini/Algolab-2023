#include <iostream>
#include <string>
#include <vector>
#include <climits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
    boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
  graph &G;

  public:
    explicit edge_adder(graph &G) : G(G) {}

    void add_edge(int from, int to, long capacity) {
      auto c_map = boost::get(boost::edge_capacity, G);
      auto r_map = boost::get(boost::edge_reverse, G);
      const auto e = boost::add_edge(from, to, G).first;
      const auto rev_e = boost::add_edge(to, from, G).first;
      c_map[e] = capacity;
      c_map[rev_e] = 0;
      r_map[e] = rev_e;
      r_map[rev_e] = e;
    }
};

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int h, w;
    std::cin>>h>>w;

    std::string note;
    std::cin>>note;

    //Characters in the front page
    std::vector<std::vector<char>> front(h, std::vector<char>(w));
    for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
        char c;
        std::cin>>c;

        front[i][j] = c;
      }
    }

    //Characters in the back page, stored to make them match the front page
    std::vector<std::vector<char>> back(h, std::vector<char>(w));
    for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
        char c;
        std::cin>>c;

        back[i][w-j-1] = c;
      }
    }

    //Frequency of each letter of the note
    std::vector<int> note_freq(26, 0);
    for(int i = 0; i < (int)note.size(); i++){
      note_freq[note[i]-'A']++;
    }

    //Frequency of the pairs in the newspaper
    std::vector<int> news_freq(26*26, 0);
    for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
        int front_letter = front[i][j] - 'A';
        int back_letter = back[i][j] - 'A';
        int pair_id = front_letter*26+back_letter;

        news_freq[pair_id]++;
      }
    }

    //Vertices from 0 to 25 for the note letters
    //Vertices from 26 to 26+26*26 for the pairs of letters of the newspaper
    graph G(26*26+26);
    edge_adder adder(G);

    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    for(int f = 0; f < 26; f++){
      for(int b = 0; b < 26; b++){

        int pair_id = f*26 + b;
        adder.add_edge(v_source, 26 + pair_id, news_freq[pair_id]);

        if(news_freq[pair_id] > 0){
          adder.add_edge(26 + pair_id, f, news_freq[pair_id]);
          adder.add_edge(26 + pair_id, b, news_freq[pair_id]);
        }
      }
    }

    for(int l = 0; l < 26; l++){
      if(note_freq[l] > 0){
        adder.add_edge(l, v_sink, note_freq[l]);
      }
    }

    int flow = boost::push_relabel_max_flow(G, v_source, v_sink);

    if(flow == (int)note.size()){
      std::cout<<"Yes\n";
    }else{
      std::cout<<"No\n";
    }
  }
}
