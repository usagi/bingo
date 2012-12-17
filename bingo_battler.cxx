#include <cassert>
#include <stdexcept>
#include <iostream>
#include <memory>

#include <glog/logging.h>

#include <wrp/bingo.hxx>

// for client

// for server
#include <Poco/Util/ServerApplication.h>

namespace {
  using namespace std;
  using namespace WonderRabbitProject::bingo;
  using card_type = card<>;
  using lottery_machine_type = lottery_machine<>;

  void initialize_logger( const char* );
  void exit_with_show_usage();

  struct entity {
    virtual void run( const vector<string>& a ) = 0;
  };

  struct server final: public entity {
    void run( const vector<string>& a ) override;
  };

  struct client final: public entity {
    void run( const vector<string>& a ) override;
  };
}

int main( int ac, char** avs ) try {

  const auto arguments = vector<string>( avs, avs + ac );

  initialize_logger( arguments.at( 0 ).data() );

  LOG( INFO ) << "=== bingo battler start ===";

  if( arguments.size() < 2 )
    exit_with_show_usage();

  const auto& mode = arguments.at( 1 );

  unique_ptr<entity> e;

  if( mode == "server" )
    e.reset( new server() );
  else
    if( mode == "client" )
      e.reset( new client() );
    else
      exit_with_show_usage();

  if( e )
    e->run( {avs, avs + ac} );
  else
    throw std::runtime_error( "entity has nullptr" );

  LOG( INFO ) << "=== bingo batter end ===";

} catch( const exception && e ) {

  LOG( ERROR ) << e.what();
  return -2;

} catch( ... ) {

  LOG( ERROR ) << "unknown exception";
  return -1;

}

namespace {

  void initialize_logger( const char* e ) {
    google::InitGoogleLogging( e );
    google::SetStderrLogging( 0 );
  }

  void exit_with_show_usage() {
    LOG( ERROR )
        << "in exit_with_show_usage";

    std::cerr
        << "oops! USAGE is\n"
        " server mode: `bingo_battler server {scaling} {maximum_index}`\n"
        " client mode: `bingo_battler client {server-address}`\n"
        ;
    std::exit( 1 );
  }

  void server::run( const std::vector< std::string >& a ) {
    LOG(INFO)<<"server::run";

  }

  void client::run( const std::vector< std::string >& a ) {
    LOG(INFO)<<"client::run";
  }

}
