/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_cplusplus_14.h
 * @version     1.0
 * @date        Jun 9, 2017 3:54:30 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_CPLUSPLUS_14_H_
#define SRC_COMMON_LCT_CPLUSPLUS_14_H_

namespace cplusplus14
{
   template <class T, std::size_t N, class ... Args>
   struct get_number_of_element_from_tuple_by_type_impl
   {
      static constexpr auto value = N;
   };

   template <class T, std::size_t N, class ... Args>
   struct get_number_of_element_from_tuple_by_type_impl<T, N, T, Args...>
   {
      static constexpr auto value = N;
   };

   template <class T, std::size_t N, class U, class ... Args>
   struct get_number_of_element_from_tuple_by_type_impl<T, N, U, Args...>
   {
      static constexpr auto value = get_number_of_element_from_tuple_by_type_impl<T, N + 1, Args...>::value;
   };

   template <class T, class ... Args>
   T& get(std::tuple<Args...>& t)
   {
      return std::get < cplusplus14::get_number_of_element_from_tuple_by_type_impl<T, 0, Args...>::value > (t);
   }

   template <class T, class ... Args>
   const T& get(const std::tuple<Args...>& t)
   {
      return std::get < cplusplus14::get_number_of_element_from_tuple_by_type_impl<T, 0, Args...>::value > (t);
   }

} // namespace cplusplus14

#if __cplusplus < 201402L
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
namespace std
{
   template<class T> struct _Unique_if
   {
      typedef unique_ptr<T> _Single_object;
   };

   template<class T> struct _Unique_if<T[]>
   {
      typedef unique_ptr<T[]> _Unknown_bound;
   };

   template<class T, size_t N> struct _Unique_if<T[N]>
   {
      typedef void _Known_bound;
   };

   template<class T, class... Args>
   typename _Unique_if<T>::_Single_object make_unique(Args&&... args)
   {
      return unique_ptr<T>(new T(std::forward<Args>(args)...));
   }

   template<class T>
   typename _Unique_if<T>::_Unknown_bound make_unique(size_t n)
   {
      typedef typename remove_extent<T>::type U;
      return unique_ptr<T>(new U[n]());
   }

   template<class T, class... Args>
   typename _Unique_if<T>::_Known_bound make_unique(Args&&...) = delete;
}
#endif


#endif /* SRC_COMMON_LCT_CPLUSPLUS_14_H_ */
