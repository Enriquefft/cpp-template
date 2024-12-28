#ifndef INCLUDE_UTILS_HPP_
#define INCLUDE_UTILS_HPP_

#include <cstdint>
#include <utility> // std::pair
#include <vector>

namespace my_utils {

using ll = intmax_t;
using ull = uintmax_t;

template <typename T> using matrix_t = std::vector<std::vector<T>>;

template <typename T> using range_t = std::pair<T, T>;

template <typename T>
concept arithmetic_t = std::is_arithmetic_v<T>;

template <arithmetic_t T> class RandomNum {
public:
  explicit RandomNum(const range_t<T> &range);
  RandomNum(const T &min, const T &max);

  auto operator()() -> T
    requires std::integral<T>;
  auto operator()() -> T
    requires std::floating_point<T>;

private:
  T m_min;
  T m_max;
};

template <arithmetic_t T> auto random_num_v(const T &min, const T &max) -> T;

template <arithmetic_t T>
inline auto random_num_v(const range_t<T> &range) -> T {
  return RandomNum_v(range.first, range.second);
}

// class explicit instantiation

extern template class RandomNum<int>;
extern template class RandomNum<unsigned int>;
extern template class RandomNum<float>;
extern template class RandomNum<int8_t>;
extern template class RandomNum<uint8_t>;

// function explicit instantiation
extern template int random_num_v<int>(const int &, const int &);
extern template unsigned int random_num_v<unsigned int>(const unsigned int &,
                                                        const unsigned int &);
extern template float random_num_v<float>(const float &, const float &);
extern template int8_t random_num_v<int8_t>(const int8_t &, const int8_t &);
extern template uint8_t random_num_v<uint8_t>(const uint8_t &, const uint8_t &);

} // namespace my_utils

#endif // INCLUDE_UTILS_HPP_
