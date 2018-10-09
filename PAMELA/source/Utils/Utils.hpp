#pragma once

// Project includes
#include "Utils/Types.hpp"

// Std library includes
#include <algorithm>
#include <utility>
#include <string>
#include <sstream>

namespace PAMELA
{

    namespace utils
    {
        template< typename T>
            bool nearlyEqual(T a, T b, T epsilon = 1e-6) {
                return std::fabs(a - b) < epsilon;
            }

		/* CUSTOM COPY FUNCTIONS */
		// Call std::copy when iterator value type is the same for input and output and std::transform otherwise

		template <typename InputIt, typename OutputIt>
		OutputIt copy(InputIt first, InputIt last, OutputIt dest,
			typename std::enable_if<std::is_same<typename std::iterator_traits<InputIt>::value_type,
			typename std::iterator_traits<OutputIt>::value_type>::value>::type* = nullptr)
		{
			return std::copy(first, last, dest);
		}

		template <typename InputIt, typename OutputIt>
		OutputIt copy(InputIt first, InputIt last, OutputIt dest,
			typename std::enable_if<!std::is_same<typename std::iterator_traits<InputIt>::value_type,
			typename std::iterator_traits<OutputIt>::value_type>::value>::type* = nullptr)
		{
			using input_type = typename std::iterator_traits<InputIt>::value_type;
			using output_type = typename std::iterator_traits<OutputIt>::value_type;
			return std::transform(first, last, dest, [](const input_type& a) { return static_cast<output_type>(a); });
		}

		/* CUSTOM INDEX SEQUENCES */

		namespace detail
		{

			template <typename T, T Val, typename Seq>
			struct add_to_seq;

			template <typename T, T Val, T... Is>
			struct add_to_seq<T, Val, std::integer_sequence<T, Is...>>
			{
				using type = std::integer_sequence<T, (Val + Is)...>;
			};

		}

		template <typename T, T N1, T N2>
		struct make_integer_range
		{
			using type = typename detail::add_to_seq<T, N1, typename std::make_integer_sequence<T, N2 - N1>::type>::type;
		};

		template <Types::size_type N1, Types::size_type N2>
		using make_index_range = make_integer_range<Types::size_type, N1, N2>;

		/* ARRAY MANIPULATORS */

		namespace detail
		{

			template <typename Array, Types::size_type... I>
			std::array<typename Array::value_type, sizeof...(I)> subindex(Array& arr, std::index_sequence<I...>)
			{
				return{ arr[I]... };
			}

		}

		template <Types::size_type N1, Types::size_type N2, typename Array>
		std::array<typename Array::value_type, N2 - N1> subindex(Array& a)
		{
			using Indices = typename utils::make_index_range<N1, N2>::type;
			return detail::subindex(a, Indices());
		}

		template <Types::size_type N, typename Array>
		std::array<typename Array::value_type, std::tuple_size<Array>::value - N> remove_first(Array& a)
		{
			return subindex<N, std::tuple_size<Array>::value>(a);
		}

		template <Types::size_type N, typename Array>
		std::array<typename Array::value_type, std::tuple_size<Array>::value - N> remove_last(Array& a)
		{
			return subindex<0, std::tuple_size<Array>::value - N>(a);
		}

		/* CONTAINER HELPERS */

		namespace detail
		{

			template <typename... Ts>
			struct is_container_helper { };

			template <typename T, typename _ = void>
			struct is_container : std::false_type { };

			template<typename T>
			struct is_container<T,
				typename std::conditional<
				false,
				detail::is_container_helper<
				typename T::value_type,
				typename T::size_type,
				typename T::iterator,
				typename T::const_iterator,
				decltype(std::declval<T>().size()),
				decltype(std::declval<T>().begin()),
				decltype(std::declval<T>().end()),
				decltype(std::declval<T>().cbegin()),
				decltype(std::declval<T>().cend())
				>,
				void
				>::type
			> : public std::true_type{};

		}

		template <typename T>
		using is_container = detail::is_container<typename std::remove_reference<T>::type>;

		// Overloads removed for non-container classes
		template <typename Container, typename std::enable_if<is_container<typename std::remove_reference<Container>::type>::value>::type* = nullptr>
		std::string to_string(Container&& cont)
		{
			std::ostringstream os;
			os << " ";
			for (auto& i : cont)
				os << i << " ";
			return os.str();
		}

	}

}
