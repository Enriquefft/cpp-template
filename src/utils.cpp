#include "utils.hpp"

#include <random>
#include <stdexcept>

namespace my_utils {

template <arithmetic_t T>
inline void validate_input(const T &min, const T &max) {
  if (min > max) {
    throw std::invalid_argument("min can't be greater than max");
  }
}

template <arithmetic_t T>
RandomNum<T>::RandomNum(const T &min, const T &max) : m_min(min), m_max(max) {
  validate_input(m_min, m_max);
}
template <arithmetic_t T>
RandomNum<T>::RandomNum(const range_t<T> &range)
    : m_min(range.first), m_max(range.second) {
  validate_input(m_min, m_max);
}

template <arithmetic_t T>
auto RandomNum<T>::operator()() -> T
  requires std::integral<T>
{
  static std::default_random_engine gen{std::random_device{}()};
  return std::uniform_int_distribution<T>{m_min, m_max}(gen);
}
template <arithmetic_t T>
auto RandomNum<T>::operator()() -> T
  requires std::floating_point<T>
{
  static std::default_random_engine gen{std::random_device{}()};
  return std::uniform_real_distribution<T>{m_min, m_max}(gen);
}

template <arithmetic_t T>
auto random_num_v(const T &min, const T &max) -> T
  requires std::integral<T>
{
  validate_input(min, max);
  static std::default_random_engine gen{std::random_device{}()};
  return std::uniform_int_distribution<T>{min, max}(gen);
}

template <arithmetic_t T>
auto random_num_v(const T &min, const T &max) -> T
  requires std::floating_point<T>
{
  validate_input(min, max);
  static std::default_random_engine gen{std::random_device{}()};
  return std::uniform_real_distribution<T>{min, max}(gen);
}

// clang-format off
// class explicit instantiation

template class RandomNum<int>;
template class RandomNum<unsigned int>;
template class RandomNum<float>;

// function explicit instantiation
template int     random_num_v<int>    (const int     &, const int&);
template unsigned int    random_num_v<unsigned int>   (const unsigned int    &, const unsigned int&);
template float   random_num_v<float>  (const float   &, const float&);


} // namespace my_utils
