#include "LFU.hpp"
#include "LRU.hpp"
#include "perfect.hpp"
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <utility>

int slow_get_page(int key) { return key; }

std::vector<std::pair<int, int>>
download_all_pages(const std::vector<int> &keys) {
  std::vector<std::pair<int, int>> pages;
  pages.reserve(keys.size());
  for (const auto key : keys) {
    pages.emplace_back(key, slow_get_page(key));
  }
  return pages;
}

int main() {
  size_t sz;
  int nelts;

  std::cin >> sz >> nelts;
  std::vector<int> keys;
  for (int i = 0; i < nelts; ++i) {
    int key;
    std::cin >> key;
    assert(std::cin.good());
    keys.push_back(key);
  }

  auto pages = download_all_pages(keys);
  cache::perfect_t<int> perfect_cache(sz, pages);
  cache::LRU_t<int> lru{sz};
  cache::LFU_t<int> lfu{sz};
  std::map<std::string, int> cache_hits;
  for (const auto key : keys) {
    if (perfect_cache.lookup_update(key, slow_get_page))
      cache_hits["perfect_cache"] += 1;

    if (lru.lookup_update(key, slow_get_page))
      cache_hits["LRU"] += 1;

    if (lfu.lookup_update(key, slow_get_page))
      cache_hits["LFU"] += 1;
  }

  std::cout << "Cache type\tN hits\n";
  for (auto [cache_type, nhits] : cache_hits) {
    std::cout << cache_type << "\t" << nhits << std::endl;
  }
  return 0;
}
