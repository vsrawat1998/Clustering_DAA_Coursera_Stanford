#include <bits/stdc++.h>
using namespace std;
#define lli long long int

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
    void cluster(vector< pair< int, pair<int,int> > > edge_list,int num_vertices);
};
void DisjointSet::cluster(vector< pair< int, pair<int,int> > > edge_list,int num_vertices){
    int clusters_required = 4;

    // Clusters can be thought to be as disjoint sets.
    // When the problem is initialized there are num_vertices number of clusters
    // due to all of them being disjoint.
    // According to the increasing order, the edge added and the number of clusters decreases
    // We stop when we get the required number of clusters.

    int num_clusters = num_vertices;
    int num_edges = edge_list.size();
    int spacing;
    pair<int,int> pi;
    for(int iter = 0;iter<num_edges;iter++){
        if(num_clusters <= clusters_required){
            break;
        }
        pi = edge_list[iter].second;
        if(find(pi.first) != find(pi.second)){
            unon(pi.first,pi.second);
            num_clusters--;
        }
    }
    // Update the parent of each node in case the node is not directly connected 
    // to the parent of its path(Lazy union).
    for(int iter =0 ;iter<num_vertices;iter++){
        parent[iter] = find(iter);
    }   
    // Look for the first instance of edges which are not in the same cluster
    for(int iter = 0;iter<num_edges;iter++){
        pi = edge_list[iter].second;
        if(parent[pi.first] != parent[pi.second]){
            spacing = edge_list[iter].first;
            break;
        }
    }
    cout<<spacing;// Maximum spacing of the clusters
}
int main()
{
    fstream file("clustering1.txt");
    int num_vertices;
    file>>num_vertices;
    int num_edges = 499*500/2; 
    int u,v,edge_cost; // Edges u and v are connected with the given cost
    vector< pair< int, pair<int,int> > > edge_list; // Stores edge cost, with the associated edge
    for(int iter = 0;iter<num_edges;iter++){
        file>>u>>v>>edge_cost;
        edge_list.push_back({edge_cost,{u-1,v-1}});
    }
    sort(edge_list.begin(),edge_list.end()); // Lowest cost edges are chosen first, according to Kruskal's MST algorithm
    DisjointSet DS(num_vertices);
    DS.cluster(edge_list,num_vertices);
    return 0;
}
