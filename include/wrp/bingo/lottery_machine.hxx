#pragma once

#include <random>
#include <deque>
#include <boost/range/algorithm_ext.hpp>

namespace WonderRabbitProject {
  namespace bingo {
    
    template<class INDEX_TYPE = default_index_type>
    struct lottery_machine {
        using index_type = INDEX_TYPE;
        using indeces_type = std::deque<index_type>;

        lottery_machine() = delete;
        lottery_machine ( const index_type maximum_index )
          : maximum_index_ ( maximum_index )
          , indeces_uncalled_ ( maximum_index ) {
          boost::iota ( indeces_uncalled_, 1 );
        }

        void reset() {
          auto i = std::back_inserter<indeces_type> ( indeces_uncalled_ );
          std::move ( indeces_called_.begin(), indeces_called_.end(), i );
        }

        void shuffle() {
          std::random_device rd;
          std::mt19937_64 rng ( rd() );
          std::shuffle ( indeces_uncalled_.begin(), indeces_uncalled_.end(), rng );
        }

        index_type next() {
          shuffle();
          const auto i = indeces_uncalled_.front();
          indeces_uncalled_.pop_front();
          indeces_called_.push_front ( i );
          return i;
        }

        index_type operator() () {
          return next();
        }

        bool is_playable() {
          return !indeces_uncalled_.empty();
        }

        indeces_type indeces_called() const {
          return indeces_called_;
        }

        indeces_type indeces_uncalled() const {
          return indeces_uncalled_;
        }
        
        size_t last_n() const{
          return indeces_uncalled_.size();
        }
        
        size_t elapsed_n() const{
          return indeces_called_.size();
        }

      private:
        indeces_type indeces_uncalled_, indeces_called_;
        const index_type maximum_index_;
    };
    
  }
}
