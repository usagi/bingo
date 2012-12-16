#pragma once

#include <stdexcept>
#include <limits>
#include <random>
#include <deque>
#include <boost/multi_array.hpp>
#include <boost/range/algorithm_ext.hpp>

namespace WonderRabbitProject {
  namespace bingo {

    template<class INDEX_TYPE = default_index_type, size_t DIMENSION = 2>
    struct card {
        static constexpr size_t dimension = DIMENSION;
        using index_type = INDEX_TYPE;
        using state_type = bool;

        using indeces_type = boost::multi_array<index_type, dimension>;
        using status_type = boost::multi_array<state_type, dimension>;
        static constexpr index_type value_max = std::numeric_limits<index_type>::max();
        static constexpr index_type value_bingo_free = value_max;

        card () = delete;
        card (
          size_t scaling = 3,
          index_type maximum_index = value_max - 1
        )
          : indeces ( boost::extents[scaling][scaling] )
          , status ( boost::extents[scaling][scaling] )
          , maximum_index_ ( maximum_index ) {

          if ( (scaling & 1) == 0 )
            throw std::invalid_argument ( "scaling must be odd" );

          if ( maximum_index < indeces.num_elements() - 1 )
            throw std::invalid_argument ( "maximum_index must be more than {pow(scaling, dimension) - 1}" );

          if ( maximum_index == value_max )
            throw std::invalid_argument ( "maximum_index must be less than {numeric_limits<indeces_type>::max()}" );

          std::vector<index_type> buffer ( maximum_index );
          boost::iota ( buffer, 0 );

          std::random_device rd;
          std::mt19937_64 rng ( rd() );
          std::shuffle ( buffer.begin(), buffer.end(), rng );

          std::move ( buffer.begin(), buffer.begin() + indeces.num_elements(), indeces.origin() );

          * ( indeces.origin() + ( indeces.num_elements() >> 1 ) ) = value_bingo_free;

          std::fill ( status.origin(), status.origin() + status.num_elements(), false );

          * ( status.origin() + ( status.num_elements() >> 1 ) ) = true;
        }

        bool is_bingo() const {
          const auto s = status.size();
          typename decltype ( status ) ::index_range ir ( 0, s );
          typename decltype ( status ) ::index_gen ig;

          auto predicate = [] ( index_type v ) {
            return v;
          };

          bool left_top_to_right_bottom = true;
          bool left_bottom_to_right_top = true;

          for ( size_t n = 0; n < s; ++n ) {
            auto v0 = status[ ig[ir][n] ];

            if ( std::all_of ( v0.begin(), v0.end(), predicate ) )
              return true;

            auto v1 = status[ ig[n][ir] ];

            if ( std::all_of ( v1.begin(), v1.end(), predicate ) )
              return true;

            left_top_to_right_bottom &= status[n][n];
            const auto r = s - 1 - n;
            left_bottom_to_right_top &= status[r][r];
          }

          return left_top_to_right_bottom || left_bottom_to_right_top;
        }

        const indeces_type& cr_indeces(){
          return indeces;
        }
        
        const status_type& cr_status(){
          return status;
        }
          
        bool push ( const index_type v ) {
          auto i = std::find ( indeces.origin(), indeces.origin() + indeces.num_elements(), v );
          auto n = std::distance ( indeces.origin(), i );
          * ( status.origin() + n ) = true;
          return is_bingo();
        }

        size_t size() const {
          return indeces.num_elements();
        }

        index_type maximum_index() const {
          return maximum_index_;
        }

      private:
        indeces_type indeces;
        status_type status;
        const index_type maximum_index_;
    };
    
  }
}

