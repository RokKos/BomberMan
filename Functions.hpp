#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <queue>

using std::ifstream;
using std::ofstream;
using std::string;
using std::queue;
using std::pair;
using std::make_pair;

void LoadLevel(const string& path, World& l) {
     ifstream in (path.c_str());
     int w, h;
     string s;
     in >> w >> h;
     l.make_map(w, h);
     for (int i =0; i<h; i++){
         in >> s;
         for (int j=0; j<w; j++){
              if (s[j] == '.') {
                   l.set(i,j,0);
              } 
              else if (s[j] == '*'){
                   l.set(i,j,1);
              }
              else {
                   l.set(i,j,2);
              }
         }
     }
     in.close();
}

bool checkposs(const World& w){
     vector<vector<bool> > visited (w.h, vector<bool>(w.w, false));
     bool b = false;
     for (int i=0; i < w.h; i++){                  
         for (int j=0; j < w.w; j++){
             if (w.grid[i][j] <= 1 && !visited[i][j]) {
                 if (b) return false;
                 b = true;
                 queue<pair<int,int> > q;
                 q.push(make_pair(i,j));
                 while (!q.empty()) {
                       pair<int,int> t = q.front();
                       q.pop();
                       int x = t.first;
                       int y = t.second;
                       
                       if (visited[x][y]) continue;
                       visited[x][y] = true;
                       
                       if (x > 0     && w.grid[x-1][y] <= 1 && !visited[x-1][y]) q.push(make_pair(x-1, y));
                       if (y > 0     && w.grid[x][y-1] <= 1 && !visited[x][y-1]) q.push(make_pair(x, y-1));
                       if (x < w.h-1 && w.grid[x+1][y] <= 1 && !visited[x+1][y]) q.push(make_pair(x+1, y));
                       if (y < w.w-1 && w.grid[x][y+1] <= 1 && !visited[x][y+1]) q.push(make_pair(x, y+1));
                 }
             }
         }
     }                                 
     return true;
}
 
void createLvl(int width, int height, World& w){
     w.make_map(width, height);
     for (int j=0; j < width; j++) {
         w.set(0,j,2);
         w.set(w.h-1,j,2);
     }
     for (int j=0; j < height; j++) {
         w.set(j,0,2);
         w.set(j, w.w-1,2);
     }
     for (int i=0; i < height; i++){                  
         for (int j=0; j < width; j++){
             if (i==0 || i==height-1 || j==0 || j== width-1){ // nastavmo rob
                continue;
             }  // nastavmo kote
             else if (i == 1 && (j == 2 || j == 1 || j == w.w-3 || j == w.w-2) ||
                 i == 2 && (j == 1 || j == w.w-2) ||
                 i == w.h-2 && (j == 1 || j == 2 || j == w.w-3 || j == w.w-2) ||
                 i == w.h-3 && (j == 1 || j == w.w-2)) {
                w.set(i,j,0);
             }               
             else {
                  // cout << i << ", " << j << endl;
                  int b = rand() % 3;
                  w.set(i,j,b);
                  
                  //cout << w << endl;
                  if (!checkposs(w)){
                    //  cout << "invalid!" << endl;
                      w.set(i,j,0);
                  } 
                  //cout << w << endl;
                  //cout << "------------" << endl;
             }
         }
     }

     
}
     
#endif
