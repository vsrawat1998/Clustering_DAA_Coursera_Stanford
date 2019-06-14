#include <bits/stdc++.h>
using namespace std;
#define lli long long int
//#define fastio() ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL)
using namespace std::chrono;
class DisjointSet {
    vector<int> parent;
    vector<int> rnk;
    // find() uses path compression to get running time of O(log*(n)) for union and find.
    int find(int node){ 
        // Recursively find the parent of the entire path and connect the vertices
        // of the currrent path to the parent.
        if(node != parent[node])
            parent[node] = find(parent[node]); 
        return parent[node];
    }
    // unon() merges two Disjoint sets/Clusters according to their ranks.
    // Rank ---> Height of a node in the disjoint set tree without path compression
    // Rank invariant ---> Rank(Parent) > Rank(Child)
    // Rank only increases when we merge two set of same rank.
    void unon(int node_i,int node_j){
        int node_i_id = find(node_i);
        int node_j_id = find(node_j);
        if(node_i_id == node_j_id){
            return;
        }
        if(rnk[node_i_id]>rnk[node_j_id]){
            parent[node_j_id] = node_i_id;
        }
        else{
            parent[node_i_id] = node_j_id;
            if(rnk[node_i_id] == rnk[node_j_id])
                rnk[node_j_id]++;
        }
    }
public:
    DisjointSet(int num_vertices){
        parent.resize(num_vertices);
        rnk.resize(num_vertices);
        for(int iter = 0;iter<num_vertices;iter++){
            parent[iter] = iter;
            rnk[iter] = 0;
        }
    }
    void cluster(vector<int> value_in_integer,map< int, list<int> > map_bits_to_int,int num_vertices);
};
// In the given question, we are required to find the number of clusters such that
// the spacing is at least 3, i.e., if we take two bits from different clusters
// then the Hamming Distance of those bits is at least 3.
void DisjointSet::cluster(vector<int> value_in_integer,map< int, list<int> > map_bits_to_int,int num_vertices){
    
    
    int num_bits = 24;
    int clusters = num_vertices;

    // for Hamming Distance 0
    // In this case, same vertices(bits) are merged in the same cluster
    // if there are multiple copies present.

    for(int iter =0;iter<num_vertices;iter++){
        if(map_bits_to_int[value_in_integer[iter]].size()>1){

            for(auto list_iter = map_bits_to_int[value_in_integer[iter]].begin();
            list_iter!=map_bits_to_int[value_in_integer[iter]].end();list_iter++){

                if(*list_iter == iter)
                    continue;
                if(find(iter)!=find(*list_iter)){
                    unon(iter,*list_iter);
                    clusters--;
                }

            }
        }
    }

    // for Hamming Distance 1
    // We can take the current value of the vertex and do XOR operation with
    // 1 bit at different positions.
    // for example:
    // 110000 ^ 100000 = 010000 ---> differs by Hamming Distance of 1 because
    // first 1 bit is not present

    // for Hamming Distance 2
    // We can take the current value of the vertex and do XOR operation with
    // 1 bit at two different positions such that the 1 bit do not coincide.
    // for example:
    // 110000 ^ 100100 = 010100 ---> differs by Hamming Distance of 2 because
    // first 1 bit is not present and 4th 1 bit is a new addition.

    // We can find both Hamming Distance 1 & 2 in 24*25/2 = 300 iterations
    // 24 iterations for distance 1 and 276 iterations for distance 2

    int curr_value,neighbour_value;
    int par_curr_value,par_closest_neighbour_value;
    int curr_parent;

    for(int iter1 =0;iter1<num_vertices;iter1++){
        curr_value = value_in_integer[iter1];
        for(int j = 0;j<num_bits;j++){
            for(int k = j;k<num_bits;k++){
                if(k == j)
                    neighbour_value = (curr_value^(1<<j)); // Hamming Distance 1
                else
                    neighbour_value = curr_value^(1<<j)^(1<<k); // Hamming Distance 2
                // If neighbour_value is in the graph, only then we merge with current vertex.
                if(map_bits_to_int.find(neighbour_value)!=map_bits_to_int.end()){ 
                    par_curr_value = find(map_bits_to_int[curr_value].front());
                    // We don't need to worry about same vertices as Hamming Distance of 0
                    // adds them to the same cluster. Only one of them suffices our need.
                    par_closest_neighbour_value = find(map_bits_to_int[neighbour_value].front());
                    if(par_curr_value!=par_closest_neighbour_value){ 
                        unon(par_curr_value,par_closest_neighbour_value);
                        clusters--;
                    }

                }
            }
        }
    }
    cout<<clusters<<"\n";
}

// O(301*N*log(N)) where N - number of vertices


int main()
{   
    auto start = high_resolution_clock::now(); 
    fstream file("clustering_big.txt");
    int num_vertices,num_bits;
    file>>num_vertices>>num_bits;
    int temp_arr[24];
    map< int, list<int> > map_bits_to_int;
    vector<int> value_in_integer;
    int value;
    for(int iter =0;iter<num_vertices;iter++){
        value = 0;
        for(int iter = 0;iter<num_bits;iter++)
            file>>temp_arr[iter];
        for(int iter = num_bits-1;iter>=0;iter--){
            if(temp_arr[iter])
                value+= (1<<num_bits-iter-1);
        }
        value_in_integer.push_back(value);
        map_bits_to_int[value].push_back(iter);
    }

    DisjointSet DS(num_vertices);
    DS.cluster(value_in_integer,map_bits_to_int,num_vertices);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start); 
    cout << duration.count()/1000; 
    return 0;
}
