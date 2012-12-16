#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <wrp/bingo.hxx>

int main ( int ac, char** avs ) try
{
  std::cout << "=== bingo simulator start ===" << std::endl;
  
  using namespace WonderRabbitProject::bingo;
  using card_type = card<>;
  using lottery_machine_type = lottery_machine<>;

  if(ac < 3){
    std::cerr << "oops! USAGE is `bingo_simulator {scaling} {maximum_index} [log_file]`\n";
    return 1;
  }
  
  const size_t scaling = std::atoll(avs[1]);
  const card_type::index_type maximum_index = std::atoll(avs[2]);
  
  std::cout
    << "scaling       : " << scaling       << "\n"
       "maximum_index : " << maximum_index
    << std::endl;

  card_type c ( scaling, maximum_index );
  lottery_machine_type m(maximum_index);
  
  for(bool bingo = false; m.is_playable() && !bingo;){
    auto i = m();
    bingo = c.push(i);
    auto elapsed = m.elapsed_n();
    auto last = m.last_n();
    std::cout
      << "* * * * * * * * * * * * * * * * \n"
         "elapsed : " << elapsed << "\n"
         "last    : " << last    << "\n"
         "current : " << i       << "\n" 
         "bingo   : " << std::boolalpha << bingo
      << std::endl;
    auto indeces = c.cr_indeces();
    auto status = c.cr_status();
    for(auto y = 0; y < scaling; ++y){
      for(auto x = 0; x < scaling; ++x){
        std::cout
          << " "
          << std::setw(3)
          << (status[y][x] ? std::string("[*]") : std::to_string(indeces[y][x]))
          << " "
          ;
      }
      std::cout << std::endl;
    }    
  }

  if(ac > 3){
    auto filename = std::string(avs[3]);
    std::cout
      << "\n"
         "<<logging>>\n"
         "filename : " << filename << "\n"
         "mode     : append, TSV(scaling, maximum_index, bingo elapsed)\n"
      << std::endl;
    std::ofstream ofs(filename, std::ios::app);
    ofs
      << scaling << "\t"
      << maximum_index << "\t"
      << m.elapsed_n()
      << std::endl;
  }

  std::cout << "=== bingo simulator end ===" << std::endl;
  
} catch (const std::exception&& e){
  std::cerr << "oops! " << e.what() << "\n";
  return -2;
} catch ( ... )
{
  std::cerr << "oops! unknown exception\n";
  return -1;
}
