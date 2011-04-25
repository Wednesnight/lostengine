#include <iostream>
#include <assimp.hpp>      
#include <aiScene.h>       
#include <aiPostProcess.h> 

using namespace std;

int main (int argc, char * const argv[]) {
  Assimp::Importer importer;

  string filepath = argv[1];
  cout << "loading file "<< filepath << endl;
  const aiScene* scene = importer.ReadFile(filepath,
        aiProcess_CalcTangentSpace       | 
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);
  if(!scene)
  {
    cerr << "import failed: " << importer.GetErrorString() << endl;
  }
  
  std::cout << "DONE\n";
  return 0;
}
