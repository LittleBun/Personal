#include<iostream>
#include<vector>
#include<string>
#include<fstream>

using namespace std;

void collect_data(vector< vector<int> > &points, string filename="training_points.txt") {
  fstream f(filename, ios_base::in);
  int x1, x2, label;
  vector<int> point;
  while(f >> x1 >> x2 >> label) {
    vector<int> point;
    point.push_back(x1);
    point.push_back(x2);
    point.push_back(label);
    cout << x1 <<" " << x2 << " " << label << endl;
    points.push_back(point);
  }

  f.close();
}

bool cal_weight(vector< vector<int> > &points, vector<int> w, float rate){
  bool flag = false;
  int output = 0;
  for(auto p : points) {
    float y = w[0] * p[0] + w[1] * p[1] +w[2];
    if(p[1] > y) {
      output = 0;
    }
    else {
      output = 1;
    }
    if(output != p[2]) {
      flag = true;
      w[0] += rate * (p[2] - y) * p[0];
      w[1] += rate * (p[2] - y) * p[1];
      w[2] += rate * (p[2] - y);
    }
  }
  return flag;
}

int main() {
  vector< vector<int> > points;
  vector<int> weight(3, 0);
  float rate = 0.1;
  int it = 0;
  collect_data(points, "data.txt");
  while(cal_weight(points, weight, rate)) {
    it += 1;
    if(it > 500) {
      cout << "No Solution!!!" << endl;
      exit(0);
    }
  }
  cout << "Solution: y = " << weight[0] << "*x + " << weight[1] << endl;
}
