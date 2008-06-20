#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <boost/program_options.hpp>
#include <string>

namespace po = boost::program_options;
using namespace std;

namespace lostbase
{
  class configuration
  {
    protected:
      po::options_description options;
      po::variables_map       options_variables;        

    public:
      configuration()
      {
      }

      void add_variable( string a_name, string a_description )
      {
        options.add_options()( a_name.c_str(), a_description.c_str() );
      }

      lostbase::configuration& operator()( string a_name, string a_description )
      {
        options.add_options()( a_name.c_str(), a_description.c_str() );
        return *this;
      }


      template <typename type> void add_variable( string a_name, type a_type, string a_description )
      {
        options.add_options()( a_name.c_str(), a_type, a_description.c_str() );
      }

      template <typename type> lostbase::configuration& operator()( string a_name, type a_type, string a_description )
      {
        options.add_options()( a_name.c_str(), a_type, a_description.c_str() );
        return *this;
      }


      void load_commandline( int argc, char *argv[] )
      {
        po::store( po::parse_command_line( argc, argv, options ), options_variables );
        po::notify( options_variables );
      }

      bool isset( string a_variable )
      {
        return options_variables.count( a_variable );
      }

      const po::variable_value& get_variable( string a_name )
      {
        return options_variables[a_name];
      }

      void print()
      {
        cout << options << "\n";
      }
  };
}

#endif
