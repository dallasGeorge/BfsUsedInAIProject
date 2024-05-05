#include <tuple>

#include <vector>

#include <queue>

#include <unordered_map>

#include <iostream>

using namespace std;

class Parking {
  private: int M,
  N;
  int ** matrix;
  public: Parking() {
    N = 3;
    M = 4;
    matrix = new int * [N];
    for (int i = 0; i < M; ++i) {
      matrix[i] = new int[M];
    }
  }
  Parking(int row, int col) {
    N = row;
    M = col;
    matrix = new int * [N];
    for (int i = 0; i < N; ++i) {
      matrix[i] = new int[M];
    }
  }
  ~Parking() {
    /**for (int i = 0; i < N; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;**/
  }
  void setContent(int n, int m, int cont) {
    matrix[n][m] = cont;
  }
  int getN() {
    return N;
  }
  int getM() {
    return M;
  }
  void printMatrix() {
    printf("Parking:\n");
    for (int i = 0; i < N; ++i) {

      for (int j = 0; j < M; ++j) {
        if (j == 0) {
          printf("|");
        }
        if (matrix[i][j] == 0) {
          printf(" ");
        } else if (matrix[i][j] == 1) {
          printf("s");

        } else if (matrix[i][j] == 2) {
          printf("2");

        } else if (matrix[i][j] == 3) {
          printf("3");
        }
        printf("|");
      }
      printf("\n");
      if (i != N - 1) {
        for (int ii = 0; ii < N * 3; ii++) {
          printf("-");
        }
      }

      printf("\n");
    }
  }
  bool isEmpty() {
    for (int i = 0; i < N; i++) {

      for (int j = 0; j < M; j++) {
        if (matrix[i][j] != 0 && matrix[i][j] != 1) {
          return false;
        }
      }
    }
    return true;
  }
  int getType(int i, int j) {
    return matrix[i][j];
  }
  std::vector < std::tuple < int,
  int,
  int >> generateMoves() {
    std::vector < std::tuple < int, int, int >> moves;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
        if (matrix[i][j] == 2) {
          if (i != N-1) {
            if (matrix[i + 1][j] == 0) {
              moves.push_back(std::make_tuple(i, j, 1));
            }
          } else {
            moves.push_back(std::make_tuple(i, j, 1));
          }
          if (i != 0) {
            if (matrix[i - 1][j] == 0) {
              moves.push_back(std::make_tuple(i, j, -1));
            }
          } else {
            moves.push_back(std::make_tuple(i, j, -1));
          }
        } else if (matrix[i][j] == 3) {
          if (j != M-1) {
            if (matrix[i][j + 1] == 0) {
              moves.push_back(std::make_tuple(i, j, 1));
            }
          } else {
            moves.push_back(std::make_tuple(i, j, 1));
          }
          if (j != 0) {
            if (matrix[i][j - 1] == 0) {
              moves.push_back(std::make_tuple(i, j, -1));
            }
          } else {
            moves.push_back(std::make_tuple(i, j, -1));
          }
        }
      }

    }
    return moves;
  }
};

struct Tree {
  Parking val;
  vector < Tree * > children;
  Tree(Parking x): val(x) {}
};

std::vector<Parking> nextSteps(Tree* node) {
  std::vector<std::tuple<int, int, int>> valid_moves = node->val.generateMoves();
  std::vector<Parking> newStates;
  for (std::tuple<int, int, int> move : valid_moves) {
    Parking p = node->val;
    int row = std::get<0>(move);
    int col = std::get<1>(move);
    
    int typeOfCar = p.getType(row, col);
    p.setContent(row, col, 0);
    if (std::get<2>(move) == 1) {
      if (typeOfCar == 2 && row != p.getN()-1 ) {
        p.setContent(row + 1, col, 2);
      } else if (typeOfCar == 3 && col != p.getM()-1 ) {
        p.setContent(row, col + 1, 3);
      }
    } else if(std::get<2>(move) == -1) {
      if (typeOfCar == 2 && row != 0) {
        p.setContent(row - 1, col, 2);
      } else if (typeOfCar == 3 && col != 0) {
        p.setContent(row, col - 1, 3);
      }
    }
    newStates.push_back(p);
    node->children.emplace_back(new Tree(p));
  }
  return newStates;
}

int bfs(Tree * root) {
  int num =0;
  queue<Tree*> queue;
  unordered_map<Tree*, Tree*> parent;

  parent[root] = nullptr;
    
    queue.push(root);

    while (!queue.empty()) {
        Tree* node = queue.front();
        
        queue.pop();

        node->val.printMatrix();
        num = num+1;
        if (node->val.isEmpty()) {
          vector<Parking> path;

            while (node != nullptr) {
                path.push_back(node->val);
                node = parent[node];
            }
            return num;
        }

    
   

    nextSteps(node);


    for (Tree * child: node -> children) {
      queue.push(child);
      parent[child] = node;
    }
  }

  cout << "Solution not found in tree." << endl;
  return 0;
}

int
main(void) {
  Parking park;
  park.setContent(0, 0, 0);
  park.setContent(0, 1, 0);
  park.setContent(0, 2, 3); //car A/D =3
  park.setContent(0, 3, 1); //stone = 1
  park.setContent(1, 0, 1);
  park.setContent(1, 1, 3);
  park.setContent(1, 2, 2); //car B/N
  park.setContent(1, 3, 0);
  park.setContent(2, 0, 0);
  park.setContent(2, 1, 0);
  park.setContent(2, 2, 1);
  park.setContent(2, 3, 0);

  Tree * root = new Tree(park);

  int path =bfs(root);
  if (path!=0) {
      
        
        cout << "NUMBER OF STEPS:" <<path <<endl;;
    } else {
        cout << "Target node not found!" << endl;
    }
    
  Parking park2;
  park2.setContent(0, 0, 0);
  park2.setContent(0, 1, 3);
  park2.setContent(0, 2, 3); //car A/D =3
  park2.setContent(0, 3, 1); //stone = 1
  park2.setContent(1, 0, 1);
  park2.setContent(1, 1, 3);
  park2.setContent(1, 2, 2); //car B/N
  park2.setContent(1, 3, 0);
  park2.setContent(2, 0, 1);
  park2.setContent(2, 1, 0);
  park2.setContent(2, 2, 1);
  park2.setContent(2, 3, 0); 
    Tree * root2 = new Tree(park2);

  int path2 =bfs(root2);
  if (path2!=0) {
      
        
        cout << "NUMBER OF STEPS:" <<path2 <<endl;
    } else {
        cout << "Target node not found!" << endl;
    }
  
  Parking park3;
  park3.setContent(0, 0, 2);
  park3.setContent(0, 1, 3);
  park3.setContent(0, 2, 3); //car A/D =3
  park3.setContent(0, 3, 1); //stone = 1
  park3.setContent(1, 0, 1);
  park3.setContent(1, 1, 3);
  park3.setContent(1, 2, 2); //car B/N
  park3.setContent(1, 3, 3);
  park3.setContent(2, 0, 1);
  park3.setContent(2, 1, 2);
  park3.setContent(2, 2, 1);
  park3.setContent(2, 3, 1); 
      Tree * root3 = new Tree(park3);
  int path3 =bfs(root3);
  if (path3!=0) {
      
        
        cout << "NUMBER OF STEPS:" <<path3;
    } else {
        cout << "Target node not found!" << endl;
    }
  return 0;
}