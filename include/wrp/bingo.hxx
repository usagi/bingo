#pragma once

#include <stdexcept>
#include <limits>
#include <random>
#include <deque>
#include <boost/multi_array.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>

namespace WonderRabbitProject {
  namespace bingo {
    using default_index_type = unsigned;
  }
}

#include "bingo/card.hxx"
#include "bingo/lottery_machine.hxx"
