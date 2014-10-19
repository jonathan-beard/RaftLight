#include <cassert>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <raft>
#include <vector>

#include "print.tcc"

template < typename T > class Generate : public Kernel
{
public:
   Generate( std::int64_t count = 1000000) : Kernel(),
                                          count( count )
   {
      output.addPort< T >( "number_stream" );
   }

   virtual raft::kstatus run()
   {
      if( count-- > 1 )
      {
         output[ "number_stream" ].push( count );
         return( raft::proceed );
      }
      output[ "number_stream" ].push( count, raft::eof );
      return( raft::stop );
   }

private:
   std::int64_t count;
};

int
main( int argc, char **argv )
{
   
   Map map;
   map.link( new Generate< std::int64_t >(), new Print< std::int64_t, '\n' >() );
   map.exe();
   return( EXIT_SUCCESS );
}
