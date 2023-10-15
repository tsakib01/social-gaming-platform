#include <iostream>
#include <cpp-tree-sitter.h>
#include "GameConfigLoader.h"
#include "GameInstance.h"

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [game file input]\n";
        return EXIT_FAILURE;
    }

    std::string_view gameFilePath = argv[1];
    std::cout << "Game file path: " << gameFilePath << "\n";

    // TODO: Creating GameInstances with configloader should probably be handled by GIM
    GameConfigLoader gameConfigLoader{gameFilePath};
    auto rules = gameConfigLoader.createGameRules();
    auto state = gameConfigLoader.createGameState();
    
    GameInstance game = GameInstance(rules, std::move(state));
    ts::Language language = tree_sitter_socialgaming();
  ts::Parser parser{language};

  std::ifstream ifs(gameFilePath);
  std::stringstream buffer;
  buffer << ifs.rdbuf();
  std::string sourcecode = buffer.str();
  ifs.close();
  std::cout << sourcecode << '\n';
  ts::Tree tree = parser.parseString(sourcecode);
  ts::Node root = tree.getRootNode();
  ts::Node configuration = root.getChildByFieldName("configuration");
  std::cout<<configuration.getType();
    int configCount=configuration.getNumNamedChildren();
    std::cout<<"config"<<std::endl;
std::vector<std::pair<std::string, std::vector<std::pair<std::string,std::string>>>> setups;
  std::cout << configuration.getType() <<" | Num Named children:"<<configCount<<'\n'<< " | Num children: " << configuration.getNumChildren() << '\n'<<std::endl;
    ts::Extent configRange = configuration.getByteRange();
    std::cout<<"configuration:"<<std::endl;
    std::cout << sourcecode.substr(configRange.start, configRange.end - configRange.start) << '\n'<<std::endl;

    for(int index=3;index<configCount;index++){
        ts::Extent identifierRange=configuration.getNamedChild(index).getChild(0).getByteRange();
        std::string identifier=sourcecode.substr(identifierRange.start,identifierRange.end-identifierRange.start);

        int childCount =configuration.getNamedChild(index).getNumChildren();
        std::vector<std::pair<std::string,std::string>> values;
        for(int j=2;j<childCount-1;j+=2){
            ts::Extent leftRange=configuration.getNamedChild(index).getChild(j).getByteRange();
            ts::Extent rightRange=configuration.getNamedChild(index).getChild(j+1).getByteRange();
         
           std::string left=sourcecode.substr(leftRange.start,leftRange.end-leftRange.start);
            std::string right=sourcecode.substr(rightRange.start,rightRange.end-rightRange.start);
           
            values.push_back(std::make_pair(left,right));
        }
        setups.push_back(std::make_pair(identifier,values));

    }
    for( auto elem : setups){
        std::cout<<elem.first<<std::endl;
        for(auto& ele:elem.second){
            std::cout<<"   "<<ele.first<<" "<<ele.second<<std::endl;
        }
        std::cout<<std::endl;
    }
    return EXIT_SUCCESS;  

}
