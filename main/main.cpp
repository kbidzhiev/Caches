#include "LRU.hpp"
#include <cassert>
#include <iostream>

int slow_get_page(int key) { return key; }


int main() {
  size_t hits{};
  size_t sz;
  int nelts;

  std::cin >> sz >> nelts;

  cache::LRU_t<int> lru{sz};

  for (int i = 0; i < nelts; ++i) {
    int p;
    std::cin >> p;
    assert(std::cin.good());
    if (lru.lookup_update(p, slow_get_page))
      hits += 1;
  }

  std::cout << "hits: " << hits << std::endl;
  return 0;
}
