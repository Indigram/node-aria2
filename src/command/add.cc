#include <cassert>
#include <aria2/aria2.h>
#include "../command.h"
#include "../queue.h"
#include "add.h"

using namespace std;
using namespace aria2;
using namespace command;

Add::Add(string uri, string uid) : uri(uri), uid(uid) {
}

void Add::execute(aria2::Session* session, std::unordered_map<aria2::A2Gid, std::string>* downloads) {
  A2Gid gid;
  vector<string> uris;
  uris.push_back(uri);
  assert(addUri(session, &gid, uris, KeyVals()) == 0);
  assert(pauseDownload(session, gid, false) == 0);

  downloads->insert(make_pair(
      gid,
      uid
  ));
}