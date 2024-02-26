#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Connection { // representa la coneccion entre dos nodos
    int destino;
    int distancia;
};

struct Way{ // un camino con su peso o distancia
    vector<int> way; //indices de los nodos en el grafo
    int distance = 0;
};

struct Node{ //Nodos del grafo
    int number;
    vector<Connection> connectionList; //relaciones del nodo
    Way lowCostWay;
    bool visited = false;
};


class Grafo {
public:
    // Constructor
    Grafo(int numberVectors): nodeList(numberVectors) {
        for(int i=0 ; i<nodeList.size(); i++){
            nodeList[i].number = i+1;
        }
    } //instancia el grafo con la cantidad exacta de nodos

    // Función para agregar una arista al grafo con la distancia de la carretera
    void agregarArista(int startNode, int endNode, int distancia) { //Llena el grafo con los caminos
        Connection connection;
        connection.destino = endNode+1;
        connection.distancia = distancia;

        nodeList[startNode].connectionList.push_back(connection);
        connection.destino = startNode+1;
        nodeList[endNode].connectionList.push_back(connection);
    }

    Way dijstrack(int currentWayIndex, int endNode){
        int waysFound = 0;
        Way currentWay = wayList[currentWayIndex]; //obtiene el camino actual
        int lastNumberNode = currentWay.way[currentWay.way.size()-1] -1; //Obtener el numero del ultimo nodo del camino
        Node lastNode = nodeList[lastNumberNode]; //Obtener el ultimo nodo del camino.
        
        cout<<"cantidad de conexiones con " << lastNode.number <<": " << lastNode.connectionList.size()<<endl;
        
        if(find(wayList[currentWayIndex].way.begin(), wayList[currentWayIndex].way.end(), endNode) != wayList[currentWayIndex].way.end()){
            return wayList[currentWayIndex];
        }
        for(int i=0; i < lastNode.connectionList.size(); i++){
            cout<<"-----------------------------------------------------------"<<endl;
            cout<<"indice del bucle: "<< i << endl<<endl;
            Way wayTemp = currentWay;
            int destino = lastNode.connectionList[i].destino;
            cout<<"Añadiendo " << destino << " a la cola"<<endl;
            wayTemp.way.push_back(destino); // agrega elementos al camino
            wayTemp.distance += lastNode.connectionList[i].distancia;
            
            if(isWayValid(wayTemp)){
                //nodeList[destino-1].lowCostWay = nodeList[destino-1].lowCostWay.distance > wayTemp.distance ? wayTemp : nodeList[destino-1].lowCostWay; //ajusta el menor camino al nodo
                if(waysFound != 0 ){
                    wayList.push_back(wayTemp);
                }
                else wayList[currentWayIndex] = wayTemp;
                waysFound++;
            }
            printWayList();
        }
        
        return dijstrack(getLowestCostWayInQueue(), endNode);
    }

    void getLowerWayPossible(int start, int end){
        cout<<"Comenzando dijstrack" <<endl;
        Way initialWay, result;
        initialWay.way.push_back(start);
        wayList.push_back(initialWay);
        result = dijstrack(0, end);

        cout<<"Se consiguio el camino mas corto que es "<<endl;
        printWay(result);
        cout<<endl<<"Con " << result.distance <<"km en total"<<endl;
    }


    int getLowestCostWayInQueue(){
        int minorCost = 0;

        for(int i=1; i < wayList.size(); i++){
            if(wayList[i].distance < wayList[minorCost].distance) minorCost = i; 
        }
        return minorCost;
    }
    bool isWayValid(Way possibleWay){ // verifica que los caminos no se repitan los nodos
        int position = possibleWay.way[0];
        for(int i=0; i < possibleWay.way.size(); i++)
            for(int k=0; k < possibleWay.way.size(); k++)
                if(k == i) continue;
                else if(possibleWay.way[i] == possibleWay.way[k]) return false;
            

        return true;
    }
    // Función para imprimir el grafo
    void printGraph() {
        for (int i = 0; i < nodeList.size(); i++) {
            cout << "- El vertice " << nodeList[i].number << " tiene una carretera con (";
            for (int k = 0; k < nodeList[i].connectionList.size(); k++) {
                Connection connectionTemp = nodeList[i].connectionList[k];
                cout << connectionTemp.destino << " de " << connectionTemp.distancia << "km  y ";
            }
            cout << " )"<< endl<< endl;
        }
    }
    void printWay(Way possibleWay){
        for(int k = 0; k < possibleWay.way.size() ; k++){
            cout<< possibleWay.way[k] << ", ";
        }
        cout<<endl;
    }

    void printWayList(){
        cout<< "imprimiendo los caminos " << wayList.size() <<endl;
        for(int i=0; i < wayList.size() ; i++){
            cout << "El camino del indice " << i+1 << ": "; 
            for(int k = 0; k < wayList[i].way.size() ; k++){
                cout<< wayList[i].way[k] << ", ";
            }
            cout<<endl;
        }
        cout<<endl;
    }
    vector<Node> getNodeList(){
        return nodeList;
    }

private:
    // Lista para representar el grafo
    vector<Node> nodeList;
    vector<Way> wayList;
};


int main(){
    int n, m, p, g, u, v, d; //n=nodo, m=arista, p= punto de partida, g = punto de llegada, 
    //U y V son dos nodos (cruces) conectados por una carretera de distancia D
    
    
    cin >> n >> m >> p >> g;
    Grafo grafo(n);
    for(int i=0; i < m; i++ ){
        cout << "va por " << i << " / "<< m << endl;
        cin >> u >> v >> d;
        grafo.agregarArista(u-1,v-1,d);
    }
    //grafo.printGraph();
    grafo.getLowerWayPossible(p, g);

    return 0;
}