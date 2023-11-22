#include "ServerManager.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

int
main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage:\n  " << argv[0] << " <port>\n"
          << "  e.g. " << argv[0] << " 8000\n";
    return 1;
  }

  const unsigned short port = std::stoi(argv[1]);
  std::unique_ptr<ServerManager> serverManager = std::make_unique<ServerManager>(port, "../social-gaming/webchat.html");
  serverManager->startServer();
  
  return 0;
}

