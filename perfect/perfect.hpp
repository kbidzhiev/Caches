#pragma once

#include <list>
#include <unordered_map>
#include <utility>
#include <vector>
#include <algorithm>

namespace cache {

template <typename T, typename KeyT = int> class perfect_t {
  size_t sz_;
  using KV = typename std::pair<KeyT, T>;

  using MapIt = typename std::unordered_map<KeyT, T>::iterator;
  std::unordered_map<KeyT, T> hash_;
  std::list<KV> future_pages_;

public:
  explicit perfect_t(size_t sz, const std::vector<KV> &future_pages)
      : sz_{sz}, future_pages_(future_pages.begin(), future_pages.end()) {}

  bool full() const { return hash_.size() == sz_; };

  template <typename F> bool lookup_update(KeyT key, F slow_get_page) {
    auto hit = hash_.find(key);
    if (hit == hash_.end()) { // not found
      if (full()) {
        if (sz_ == 0) {
          return false;
        }
        // eviction strategy:
        // "Replace the page that will not be used for the longest time in the
        // future."
        auto key_to_remove = find_furthest_in_the_future();
        hash_.erase(key_to_remove);
      }
      hash_.emplace(key, slow_get_page(key));
      return false;
    }

    return true;
  }

private:
  size_t distance_until(const KeyT &key) const {
    const auto & fp = future_pages_;
    for(auto it = fp.begin(); it != fp.end(); ++it){
      if (it -> first == key){
        return std::distance(fp.begin(), it);
      }
    }
    return 0;
# if 0 
    auto it = std::find_if(future_pages_.begin(), future_pages_.end(), key,
                           [](auto &[page_key, page], keyT &target) {
                             return page_key == target;
                           });
    return std::distance(it, future_pages.begin());
#endif
  }

  KeyT find_furthest_in_the_future() const {
    KeyT furthest_key = hash_.begin()->first;
    size_t max_dist = distance_until(furthest_key);
    for (auto &[key, _] : hash_) {
      size_t dist = distance_until(key);
      if (dist > max_dist) {
        max_dist = dist;
        furthest_key = key;
      }
    }
    return furthest_key;
  }
};

} // namespace cache
