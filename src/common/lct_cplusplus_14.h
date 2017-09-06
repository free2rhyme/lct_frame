/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         lct_cplusplus_14.h
 * @version      1.0
 * @date         Jun 9, 2017 3:54:30 PM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
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

#endif /* SRC_COMMON_LCT_CPLUSPLUS_14_H_ */
