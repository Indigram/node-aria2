#include <cassert>
#include <aria2/aria2.h>
#include "start.h"

using namespace std;
using namespace aria2;
using namespace command;

Start::Start(string uid) : uid(uid) {
}

void Start::execute(Session* session, unordered_map<A2Gid, string>* downloads) {
  for (unordered_map<A2Gid, string>::iterator it = downloads->begin(); it != downloads->end(); ++it) {
    if (it->second == uid) {
      assert(unpauseDownload(session, it->first) == 0);
      return;
    }
  }
  assert(0 == 1);
}