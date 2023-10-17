#include "GameSetupLoader.h"
Setup::Setup(KIND kind,std::string_view prompt):kind(kind),prompt(prompt),restInfo(std::string_view{}){}
Setup::Setup(KIND kind,std::string_view prompt,std::string_view restInfo):kind(kind),prompt(prompt),restInfo(restInfo){}