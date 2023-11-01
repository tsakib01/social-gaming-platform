#include "ServerManager.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

int
main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Usage:\n  " << argv[0] << " <port> <html response>\n" << "  e.g. " << argv[0] << " 8000 ../social-gaming/webchat.html\n";
    return 1;
  }

  const unsigned short port = std::stoi(argv[1]);
  std::unique_ptr<ServerManager> serverManager = std::make_unique<ServerManager>(port, argv[2]);
  serverManager->startServer();
  
  return 0;
}

