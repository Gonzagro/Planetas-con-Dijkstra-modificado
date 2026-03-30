#include <iostream>
#include <vector>
#include <queue>
#include <functional>
using namespace std;
typedef pair<int, int> arista;

const int infinito = 1e9;

struct grafo_pesado {
    
    vector<vector<pair<int, int>>>  representacion; //lista de ady con peso

   grafo_pesado(int vertices){ 
    representacion.resize(vertices);
   }
   void insertar(int u, int v, int peso){
    representacion[u].push_back({v,peso});
    representacion[v].push_back({u,peso});
   }
   int pesado(int v, int w){
        for(auto u : representacion[v]){
            if(u.first == w){
                return u.second;
            }
        }
        return infinito;
   }
   vector<pair<int,int>> vecindarioDe(int vertice){
        return representacion[vertice];
    }
    bool pertenece(arista vw){
        for(int i=0; i < vecindarioDe(vw.first).size(); i++){
            if( vw.second == i) return true;
        }
        return false;
    }
   int cantvertices(){
    return representacion.size();
   }
   vector<int> nodos(){
    vector<int> res {};
    for(int i =0; i < representacion.size(); i++){
        res.push_back(i);
    }
    return res;
   }
};

bool busqueda_binaria (vector<int> lista, int objetivo, int izq, int der){
    while (izq <= der) {
        int medio = (izq + der)/2;
        if(lista[medio] == objetivo) return true;
        if( lista[medio] > objetivo) der = medio -1;
        else{
            izq = medio +1;
        } 
    }
    return false;
}

vector<int> dij(priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> S, vector<int> distancia, int v, grafo_pesado& G, vector<vector<int>>& tiempos){

    vector<bool> visitado(G.cantvertices(),false);
    
    int salida = 0;
        for(int i =0; i < tiempos[v].size(); i++){
            if(salida == tiempos[v][i]) salida +=1;
            else{
                break;
            }
        }

    for(auto i : G.vecindarioDe(v)){

        int peso = G.pesado(v,i.first) + salida;
        while(busqueda_binaria( tiempos[i.first], peso, 0, tiempos[i.first].size() -1)){
            peso += 1;
        }
        distancia[i.first] = peso;
    }
    distancia[v] = salida;
    S.push({salida,v});
    
    while( !S.empty()){
        pair<int,int> e = S.top(); 
        S.pop();
        visitado[e.second] = true;

        for(auto i : G.vecindarioDe(e.second)){
            if(!visitado[i.first]){

                int peso = G.pesado(e.second,i.first) + e.first; 
                while(busqueda_binaria( tiempos[i.first], peso, 0, tiempos[i.first].size() -1)){
                    peso += 1;
                }

                S.push({peso, i.first});
            }
        }
       
        
        for(auto i : G.vecindarioDe(e.second)){
            if(!visitado[i.first]){
                int peso = G.pesado(e.second,i.first) + e.first; 
                
                    while(busqueda_binaria( tiempos[i.first], peso, 0, tiempos[i.first].size() -1)){
                        peso += 1;
                    }

                

                if(distancia[i.first] > peso){
                    
                    distancia[i.first] = peso;
                    
                }
            }
        }

    }
    return distancia;
}

int main(){
    int n, m;
    cin >> n >> m;
    grafo_pesado G(n);
    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c ;
        G.insertar(a -1, b -1, c);
    }
    vector<vector<int>> tiempos(n);
    for (int i = 0; i < n; i++) {
        int k;
        cin >> k ;
        for(int j = 0; j < k; j++){
            int ti;
            cin >> ti;
            tiempos[i].push_back(ti);
        }
    }
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> S;
    vector<int> distancia(n, infinito);
    vector<int> res_vector = dij(S, distancia, 0, G, tiempos);
    int res = res_vector[n-1];
    if(res == infinito) res = -1;
    cout << res << endl;
    return 0;
}
