#ifndef TRAITS_H
#define TRAITS_H

#include <type_traits>

template<class T>
struct remove_cvref
{
    typedef std::remove_cv_t<std::remove_reference_t<T>> type;
};

template< class T >
using remove_cvref_t = typename remove_cvref<T>::type;

template<typename Function>
struct function_traits_helper;

template<typename R>
struct function_traits_helper<R(*)(void)>
{
    using result_type = R;
};

template<typename R, typename T1>
struct function_traits_helper<R(*)(T1)>
{
    using result_type = R;
    using argument_type = T1;
    using msg_type = remove_cvref_t<T1>;
};

template<class T>
struct function_traits :
    public function_traits_helper<typename std::add_pointer_t<remove_cvref_t<T>>>
{};

#endif
