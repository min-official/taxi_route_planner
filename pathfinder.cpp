#include <cstring>
#include <cassert>
#include <iostream>
#include "map.h"
#include <cmath>
#include <limits>
#include <chrono>

using namespace std;

void print_usage(const char *prog)
{
    cout << "usage: " << prog << " ALGORITHM" << endl;
};


int main(int argc, const char *argv[])
{
    if (argc != 2) {
        print_usage(argv[0]);
        return 1;
    };

    // Load a map.
    struct map::Map m;
    map::load_map(&m);

    // Shortest paths.
    struct map::Path p;

    // Select algorithm.
    if (!strncmp(argv[1], "dijkstra", 9)) {
    	//auto START = std::chrono::high_resolution_clock::now();
        // Version 1: Use Dijkstra's algorithm.
        //std::cout<<" start";
        for (int i=0; i<m.clients.size(); i++){
            // crossroad which contain cid, x, y, distance, visited, parent
            //std::cout<<" client";
            struct crossroad{
                 size_t cid;
                 float x;
                 float y;
                 float distance;
                 //If visited = 1, not visited = 0
                 int visited;
                 size_t parent;
                 };
            std::vector<struct crossroad> crossroads;
            ////std::cout<<"3";
            for (int j=0; j<m.crosses.size(); j++){
                //std::cout<<" initialize";
                //Fill in vector crossroads : first = source point
                 struct crossroad c;
                 c.cid = m.crosses[j].cid;
                 c.x = m.crosses[j].x;
                 c.y = m.crosses[j].y;
                 c.visited = 0;
                 c.parent = -1;
                 if (c.cid == m.clients[i].src_cid){
                     ////std::cout<<"5";
                     c.distance = 0;
                     crossroads.insert(crossroads.begin(), c);
                 }
                 else{
                     ////std::cout<<"6";
                     c.distance = std::numeric_limits<float>::infinity();    
                     crossroads.push_back(c);                
                 }; };            
            ////std::cout<<"7";
            struct crossroad start = crossroads[0];
            while(start.cid != m.clients[i].dst_cid){
                //std::cout<<" adjacent";
                // Make adjacent vector : adjacent crossroad of start vertex
                std::vector<size_t> adjacent;
                for (int j = 0; j<m.roads.size(); j++){
                    ////std::cout<<"9";
                     if (m.roads[j].cids[0]==start.cid ){
                         for(int k = 0; k<crossroads.size(); k++){
                             ////std::cout<<"10";
                             if(m.roads[j].cids[1]==crossroads[k].cid && crossroads[k].visited==0){
                                 size_t a = m.roads[j].cids[1];
                                 adjacent.push_back(a);
                             }
                         }
                     }
                     else if (m.roads[j].cids[1]==start.cid ){
                         for(int k = 0; k<crossroads.size(); k++){
                             ////std::cout<<"10";
                             if(m.roads[j].cids[0]==crossroads[k].cid && crossroads[k].visited==0){
                                 size_t a = m.roads[j].cids[0];
                                 adjacent.push_back(a);
                             }
                         }
                     };
                };
                //std::cout<<adjacent.size();
                ////std::cout<<"11";
                //go to every adjacent crossroads and compare distance and update the distance and parent
                while(!adjacent.empty()){
                    ////std::cout<<"12";
                    for(int j=0; j<crossroads.size(); j++){
                        ////std::cout<<"13";
                        if (adjacent.back() == crossroads[j].cid){
                            //std::cout<<" update";
                            float edge = sqrt(pow((start.x-crossroads[j].x),2)+pow((start.y-crossroads[j].y),2));
                            if(crossroads[j].distance > edge+start.distance){
                                crossroads[j].distance = edge+start.distance;
                                crossroads[j].parent = start.cid;
                            }
                        };
                        };
                        adjacent.pop_back();
                };
                //std::cout<<adjacent.size();
                ////std::cout<<" min";
                //change the start crossroads to other vertex which has minimum distance but not visited yet
                for (int j=0; j<crossroads.size(); j++){
                    if (start.cid == crossroads[j].cid){
                        crossroads[j].visited = 1;
                        //std::cout<<" visited";
                        //std::cout<<crossroads[j].cid;
                    };
                };
                
                int min = std::numeric_limits<float>::infinity();
                //struct crossroad change;
                for (int j = 0;j<crossroads.size(); j++){
                    if(crossroads[j].visited == 0 && crossroads[j].distance<min){
                        start = crossroads[j];
                        min = crossroads[j].distance;
                        //std::cout<<"minchange ";
                    }
                };
                //start = change;
                ////std::cout<<adjacent.size();
                ////std::cout<<crossroads.size();
                //std::cout<<m.clients[i].dst_cid;
                //std::cout<<start.cid;
            };
            //std::cout<<" path";
            //fill in path
            std::vector<size_t> clientpath;
            struct crossroad w;
            w.cid = m.clients[i].dst_cid;
            int k = 0;
            while(w.cid != m.clients[i].src_cid){
                for (int j = 0;j<crossroads.size(); j++){
                    if(crossroads[j].cid == w.cid){
                        w = crossroads[j];
                        break;
                    };};
                
                clientpath.push_back(w.cid);
                w.cid = w.parent;
                k++;
            };
            clientpath.push_back(w.cid);
            p.paths.push_back(clientpath);
        };
       // auto FINISH = std::chrono::high_resolution_clock::now();
        //std::chrono::duration<double> elapsed = FINISH - START;
    	//std::cout<<"Elapsed time:"<<elapsed.count()<<" s\n";
    } else if (!strncmp(argv[1], "a-star", 7)) {
        // Version 2: Use A* algorithm.
        //auto START = std::chrono::high_resolution_clock::now();
        for (int i=0; i<m.clients.size(); i++){
            // crossroad which contain cid, x, y, distance, visited, parent
            //std::cout<<" client";
            struct crossroad{
                 size_t cid;
                 float x;
                 float y;
                 float distance;
                 float straight;
                 float dtotal;
                 //If visited = 1, not visited = 0
                 int visited;
                 size_t parent;
                 };
            std::vector<struct crossroad> crossroads;
            ////std::cout<<"3";
            for (int j=0; j<m.crosses.size(); j++){
                //std::cout<<" initialize";
                //Fill in vector crossroads : first = source point
                 struct crossroad c;
                 c.cid = m.crosses[j].cid;
                 c.x = m.crosses[j].x;
                 c.y = m.crosses[j].y;
                 c.visited = 0;
                 c.parent = -1;
                 if (c.cid == m.clients[i].src_cid){
                     ////std::cout<<"5";
                     c.distance = 0;
                     c.straight = 0;
                     c.dtotal = 0;
                     crossroads.insert(crossroads.begin(), c);
                 }
                 else{
                     ////std::cout<<"6";
                     c.distance = std::numeric_limits<float>::infinity();  
                     for (int k=0; k<m.crosses.size(); k++){
                         if (m.clients[i].dst_cid == m.crosses[k].cid){
                             c.straight = sqrt(pow((c.x-m.crosses[k].x),2)+pow((c.y-m.crosses[k].y),2));
                             break;
                         };
                     };   
                     c.dtotal = c.distance + c.straight;
                     crossroads.push_back(c);                
                 }; };            
            ////std::cout<<"7";
            struct crossroad start = crossroads[0];
            while(start.cid != m.clients[i].dst_cid){
                //std::cout<<" adjacent";
                // Make adjacent vector : adjacent crossroad of start vertex
                std::vector<size_t> adjacent;
                for (int j = 0; j<m.roads.size(); j++){
                    ////std::cout<<"9";
                     if (m.roads[j].cids[0]==start.cid ){
                         for(int k = 0; k<crossroads.size(); k++){
                             ////std::cout<<"10";
                             if(m.roads[j].cids[1]==crossroads[k].cid && crossroads[k].visited==0){
                                 size_t a = m.roads[j].cids[1];
                                 adjacent.push_back(a);
                             }
                         }
                     }
                     else if (m.roads[j].cids[1]==start.cid ){
                         for(int k = 0; k<crossroads.size(); k++){
                             ////std::cout<<"10";
                             if(m.roads[j].cids[0]==crossroads[k].cid && crossroads[k].visited==0){
                                 size_t a = m.roads[j].cids[0];
                                 adjacent.push_back(a);
                             }
                         }
                     };
                };
                //std::cout<<adjacent.size();
                ////std::cout<<"11";
                //go to every adjacent crossroads and compare total distance and update the total distance and parent
                while(!adjacent.empty()){
                    ////std::cout<<"12";
                    for(int j=0; j<crossroads.size(); j++){
                        ////std::cout<<"13";
                        if (adjacent.back() == crossroads[j].cid){
                            //std::cout<<" update";
                            float edge = sqrt(pow((start.x-crossroads[j].x),2)+pow((start.y-crossroads[j].y),2));
                            if(crossroads[j].dtotal > edge+start.distance+crossroads[j].straight){
                                crossroads[j].distance = edge+start.distance;
                                crossroads[j].dtotal = edge+start.distance+crossroads[j].straight;
                                crossroads[j].parent = start.cid;
                            }
                        };
                        };
                        adjacent.pop_back();
                };
                //std::cout<<adjacent.size();
                ////std::cout<<" min";
                //change the start crossroads to other vertex which has minimum distance but not visited yet
                for (int j=0; j<crossroads.size(); j++){
                    if (start.cid == crossroads[j].cid){
                        crossroads[j].visited = 1;
                        //std::cout<<" visited";
                        //std::cout<<crossroads[j].cid;
                    };
                };
                
                int min = std::numeric_limits<float>::infinity();
                //struct crossroad change;
                for (int j = 0;j<crossroads.size(); j++){
                    if(crossroads[j].visited == 0 && crossroads[j].dtotal<min){
                        start = crossroads[j];
                        min = crossroads[j].dtotal;
                        //std::cout<<"minchange ";
                    }
                };
                //start = change;
                ////std::cout<<adjacent.size();
                ////std::cout<<crossroads.size();
                //std::cout<<m.clients[i].dst_cid;
                //std::cout<<start.cid;
            };
            //std::cout<<" path";
            //fill in path
            std::vector<size_t> clientpath;
            struct crossroad w;
            w.cid = m.clients[i].dst_cid;
            int k = 0;
            while(w.cid != m.clients[i].src_cid){
                for (int j = 0;j<crossroads.size(); j++){
                    if(crossroads[j].cid == w.cid){
                        w = crossroads[j];
                        break;
                    };};
                
                clientpath.push_back(w.cid);
                w.cid = w.parent;
                k++;
            };
            clientpath.push_back(w.cid);
            p.paths.push_back(clientpath);
        };
        //auto FINISH = std::chrono::high_resolution_clock::now();
        //std::chrono::duration<double> elapsed = FINISH - START;
    	//std::cout<<"Elapsed time:"<<elapsed.count()<<" s\n";
    } else {
        cerr << "ALGORITHM should be either dijkstra or a-star. Given: "
             << argv[1] << "." << endl;
        print_usage(argv[0]);
        return 1;
    }
	
    // Write results into a file.
    map::store_path(&p);
    
    return 0;
}
