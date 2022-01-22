#include <iostream>
#include <vector>
#include <random>
#include <tuple>

using namespace std;

const int m_wall = 1;         //壁
const int m_empty = 0;        //通路
const int MIN_BAL = 5;        //map最小値
const int MAX_BAL = 30;       //map最大値
vector<vector<size_t> > map;  //map(vectorの2次元配列)
vector<int> CDPath;           //掘削候補地

//mapサイズ乱数生成(範囲 : MIN_BAL ~ MAX_BAL+MIN_BAL)
int get_rand(){
  int reInt = rand() % MAX_BAL + MIN_BAL;
  if(reInt%2 == 0) return ++reInt;
  else return reInt;
}

//方向random
int get_dir(vector<string> dir){
  int dirInt = rand() % dir.size();
  // cout << dirInt << " " << dir[dirInt] << endl;
  if(dir[dirInt] == "U") return 0;
  else if(dir[dirInt] == "R") return 1;
  else if(dir[dirInt] == "D") return 2;
  else if(dir[dirInt] == "L") return 3;
  else return -1;
}

//map初期化
void mapInit(vector<vector<size_t> >& map){

  //外壁も一旦通路とする(あとで直す)
  for(int i=0; i<map.size(); i++){
    for(int j=0; j<map[i].size(); j++){
      if(i == 0 || j == 0 || i == map.size()-1 || j == map[0].size()-1){
        map[i][j] = m_empty;
      }else{
        map[i][j] = m_wall;
      }
    }
  }
}

//mapの外壁囲う
void mapEnclose(vector<vector<size_t> >& map){

  //外壁も一旦通路とする(あとで直す)
  for(int i=0; i<map.size(); i++){
    for(int j=0; j<map[i].size(); j++){
      if(i == 0 || j == 0 || i == map.size()-1 || j == map[0].size()-1){
        map[i][j] = m_wall;
      }
    }
  }
}

//生成チェック
const bool mapCheck(const vector<vector<size_t> >& map){
  if(map.size() <= 2 || map[map.size()-1].size() <= 2) return false;
  return true;
}

//通路の設定 & 掘削候補地の記録
void SetPath(vector<vector<size_t> >& map, int x, int y){

  map[x][y] = m_empty;

  if(x%2 == 1 && y%2 == 1){

    CDPath.push_back(x);
    CDPath.push_back(y);
  }
}

//新たな出発点を探す
int *GetPos(){

  int posInt = rand() % CDPath.size();
  if(posInt%2 == 1) posInt--;

  int ret[2];
  ret[0] = CDPath[posInt];
  ret[1] = CDPath[posInt+1];

  CDPath.erase(CDPath.begin()+posInt-1);
  CDPath.erase(CDPath.begin()+posInt);

  return ret;
}

//穴掘り堀り
void mapDig(int x, int y){

  //進める方向の確認
  while(true){
    vector<string> dir;
    //Up
    if(map[x-1][y] == m_wall && map[x-2][y] == m_wall) dir.push_back("U");
    //Right
    if(map[x][y+1] == m_wall && map[x][y+2] == m_wall) dir.push_back("R");
    //Down
    if(map[x+1][y] == m_wall && map[x+2][y] == m_wall) dir.push_back("D");
    //Left
    if(map[x][y-1] == m_wall && map[x][y-2] == m_wall) dir.push_back("L");

    //どこにもいけなかったら終了
    if(dir.empty()) break;

    // SetPath(map, x, y);

    //ランダムで選んだ方向へ穴を掘る
    switch(get_dir(dir)){
      case 0:
        SetPath(map, --x, y);
        SetPath(map, --x, y);
        break;
      case 1:
        SetPath(map, x, ++y);
        SetPath(map, x, ++y);
        break;
      case 2:
        SetPath(map, ++x, y);
        SetPath(map, ++x, y);
        break;
      case 3:
        SetPath(map, x, --y);
        SetPath(map, x, --y);
        break;
    }
  }

  if(!CDPath.empty()){
    auto ret = GetPos();
    int x = ret[0];
    int y = ret[1];
    mapDig(x,y);
  }
}

//迷路生成
void mapMake(vector<vector<size_t> >& map){

  if(!mapCheck(map)) return;

  //mapの初期化
  mapInit(map);

  //map[1][1]を空にする
  map[1][1] = m_empty;

  //穴掘りする
  mapDig(1, 1);

}

//出力
void mapOutput(vector<vector<size_t> >& map) {

  if(!mapCheck(map)) return;

  //壁 = '#', 通路 = '.'
  for (size_t i = 0; i < map.size(); ++i) {
      for (size_t j = 0; j < map[i].size(); ++j) {
          if(map[i][j] == 1){
            cout << "#" << " ";
          }else{
            cout << " " << " ";
          }
      }
      cout << endl;
  }
}

//迷路の作成
void createMaze(){

  //ランダムな２次元配列(vectorの２次元配列)を宣言
  int x = get_rand();
  int y = get_rand();
  cout << x << " x " << y << endl;
  map.assign(x, vector<size_t>(y, 0));

  //迷路生成
  mapMake(map);

  //周りを囲う
  mapEnclose(map);

  //出力
  mapOutput(map);

  //CDPathの中身確認
  // for(int i=0; i<CDPath.size(); i++){
  //   cout << i << " : " << CDPath[i] << endl;
  //   if(i%2==1) cout << endl;
  // }
}

//メイン
int main() {

    createMaze();

    return 0;
}
