#include<string_view>
#include<vector>
#include <memory>
#include <cpp-tree-sitter.h>
enum KIND{
    INTEGER, STRING, BOOLEAN, ENUM,
};
class Setup{
public:
    Setup(KIND kind,std::string_view prompt);
    Setup(KIND kind,std::string_view prompt,std::string_view restInfo);
    void intProcess();
    void boolProcess();
    void strProcess();
    void processSetup();
private:
    std::string_view identifier;
    KIND kind;
    std::string_view prompt;
    //may change to other type;
    std::string_view restInfo;
};
class GameSetupLoader{
public:
    GameSetupLoader(std::string_view source);
    Setup convertNodetoSetup(const ts::Node& node);
    std::unique_ptr<std::vector<Setup>>getGameSetup (const ts::Node& node);
private:
    std::string_view source;

};

//class intSetup:public  Setup{
//public:
//    void processSetup();
//    intSetup(ts::N);
//private:
//    std::string_view name;
//    std::string_view prompt;
//};
//class boolSetup:public  Setup{
//public:
//    void processSetup();
//private:
//    std::string_view name;
//    std::string_view prompt;
//
//};
//class strSetup:public  Setup{
//public:
//    void processSetup();
//private:
//    std::string_view name;
//    std::string_view prompt;
//};