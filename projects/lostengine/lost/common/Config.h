#ifndef LOST_COMMON_CONFIG_H
#define LOST_COMMON_CONFIG_H

//#define BOOST_SPIRIT_DEBUG

#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <map>
#include <boost/lexical_cast.hpp>
#include <boost/spirit/core.hpp>

#include "lost/common/Logger.h"

namespace lost
{
  namespace common
  {

    struct ConfigValue
    {
      std::string value;

      template<typename ValueType>
      ValueType as() const
      {
        return boost::lexical_cast<ValueType>( value );
      }
    };

    typedef std::map<std::string, ConfigValue> ConfigMap;

    template<typename ParserType>
    struct BaseAction
    {
      ParserType& parser;

      BaseAction( ParserType& parserValue ) : parser(parserValue) {}
    };

    struct ConfigFileParser
    {
      ConfigMap&          map;
      std::ostringstream& content;
      std::ostringstream  output;
      std::string         currentKey;
      std::string         currentValue;
      std::string         currentComment;

      ConfigFileParser( ConfigMap& configMap, std::ostringstream& configContent )
      : map(configMap), content(configContent)
      {
      }

      struct LoadKeyValueAction : public BaseAction<ConfigFileParser>
      {
        LoadKeyValueAction( ConfigFileParser& configFileParser ) : BaseAction<ConfigFileParser>( configFileParser ) {}
        void operator()( const char *first, const char *last) const
        {
          parser.map[parser.currentKey].value = parser.currentValue;
        }
      };
      struct LoadCommentAction : public BaseAction<ConfigFileParser>
      {
        LoadCommentAction( ConfigFileParser& configFileParser ) : BaseAction<ConfigFileParser>( configFileParser ) {}
        void operator()( const char *first, const char *last) const
        {
        }
      };

      struct MergeKeyValueAction : public BaseAction<ConfigFileParser>
      {
        MergeKeyValueAction( ConfigFileParser& configFileParser ) : BaseAction<ConfigFileParser>( configFileParser ) {}
        void operator()( const char *first, const char *last) const
        {
          if (parser.map.find( parser.currentKey ) != parser.map.end())
          {
            parser.output << parser.currentKey << "=" << parser.map[parser.currentKey].value << std::endl;
            parser.map.erase( parser.currentKey );
          }
          else
          {
            parser.output << parser.currentKey << "=" << parser.currentValue << std::endl;
          }
        }
      };
      struct MergeCommentAction : public BaseAction<ConfigFileParser>
      {
        MergeCommentAction( ConfigFileParser& configFileParser ) : BaseAction<ConfigFileParser>( configFileParser ) {}
        void operator()( const char *first, const char *last) const
        {
          parser.output << "# " << parser.currentComment << std::endl;
        }
      };

      template<typename KeyValueAction, typename CommentAction>
      bool parse()
      {
        using namespace boost::spirit;

        rule<> configkey   = +(alnum_p | ".");
        rule<> configvalue = *(anychar_p - space_p);
        rule<> configline  = *space_p >> configkey[assign_a(currentKey)] >> *space_p >> ch_p("=") >>
                             *(space_p - eol_p) >> configvalue[assign_a(currentValue)] >> *(space_p - eol_p) >> (eol_p | end_p);
        rule<> commentline = *space_p >> ch_p("#") >> *space_p >> (*(anychar_p - eol_p))[assign_a(currentComment)] >> (eol_p | end_p);
        rule<> emptyline   = *(space_p - eol_p) >> eol_p;
        rule<> configfile  = *(
                                  configline   [KeyValueAction( *this )]
                                | commentline  [CommentAction( *this )]
                                | emptyline
                              );

        BOOST_SPIRIT_DEBUG_NODE(configkey);
        BOOST_SPIRIT_DEBUG_NODE(configvalue);
        BOOST_SPIRIT_DEBUG_NODE(configline);
        BOOST_SPIRIT_DEBUG_NODE(commentline);
        BOOST_SPIRIT_DEBUG_NODE(emptyline);
        BOOST_SPIRIT_DEBUG_NODE(configfile);

        return boost::spirit::parse(content.str().c_str(), configfile).full;
      }

      bool load()
      {
        return parse<LoadKeyValueAction, LoadCommentAction>();
      }

      bool merge()
      {
        bool result;

        // save the variable map
        ConfigMap saveMap = map;

        result = parse<MergeKeyValueAction, MergeCommentAction>();
        if (result)
        {
          // clear the output
          content.str("");
          content.clear();
          // write the parsed data
          content.write( output.str().c_str(), output.str().size() );
          // write the remaining values
          ConfigMap::iterator idx;
          for (idx = map.begin(); idx != map.end(); ++idx)
          {
            DOUT((*idx).first << "=" << (*idx).second.value);
            content << (*idx).first << "=" << (*idx).second.value << std::endl;
          }
        }

        // restore the variable map
        map = saveMap;

        return result;
      }
    };

    struct CommandLineParser
    {
      ConfigMap&   map;
      std::string  currentKey;
      std::string  currentValue;

      CommandLineParser( ConfigMap& configMap ) : map(configMap) {}

      struct KeyValueAction : public BaseAction<CommandLineParser>
      {
        KeyValueAction( CommandLineParser& commandLineParser ) : BaseAction<CommandLineParser>( commandLineParser ) {}
        void operator()( const char *first, const char *last) const
        {
          parser.map[parser.currentKey].value = parser.currentValue;
        }
      };

      bool parse( std::string& argument )
      {
        using namespace boost::spirit;

        rule<> configkey   = +(alnum_p | ".");
        rule<> configvalue = +(anychar_p - space_p);
        rule<> configpair  = configkey[assign_a(currentKey)] >> "=" >> configvalue[assign_a(currentValue)];
        rule<> commandline = *( configpair[KeyValueAction( *this )] );

        BOOST_SPIRIT_DEBUG_NODE(configkey);
        BOOST_SPIRIT_DEBUG_NODE(configvalue);
        BOOST_SPIRIT_DEBUG_NODE(configpair);
        BOOST_SPIRIT_DEBUG_NODE(commandline);

        return boost::spirit::parse(argument.c_str(), commandline).full;
      }

    };

    struct Config
    {
      ConfigMap variables;

      Config() {}

      Config( int argc, char *argv[] )
      {
        init( argc, argv );
      }

      Config( int argc, char *argv[], const std::string& filename )
      {
        init( argc, argv, filename );
      }

      ConfigValue& operator[]( const std::string& name )
      {
        if (this->valid( name )) return variables[name];
        throw std::invalid_argument( "config variable not found" );
      }

      bool valid( const std::string& name ) const
      {
        return ( variables.find( name ) != variables.end() );
      }

      ConfigValue& add( const std::string& key, const std::string& value )
      {
        variables[key].value = value;
        return variables[key];
      }

      template <typename Type>
      Type get( const std::string& inName, Type inDefault )
      {
        ConfigValue& result = (this->valid( inName ))
                                ? variables[inName]
                                : add( inName, boost::lexical_cast<std::string>(inDefault) );
        return result.as<Type>();
      }

      template <typename Type>
      ConfigValue& set( const std::string& inName, Type inValue )
      {
        std::stringstream buffer;
        buffer << inValue;
        return add( inName, buffer.str() );
      }

      void init( int argc, char *argv[] )
      {
        loadFromCommandline( argc, argv );
      }

      void init( int argc, char *argv[], const std::string& filename )
      {
        loadFromFile( filename );
        init( argc, argv );
      }

      void loadFromCommandline( int argc, char *argv[] )
      {
        CommandLineParser parser( variables );
        std::string       value;

        for (int idx = 1; idx < argc; ++idx)
        {
          value = argv[idx];
          parser.parse( value );
        }
      }

      void loadFromFile( const std::string filename )
      {
        std::ifstream ifs( filename.c_str() );
        assert( ifs.good() );
        std::ostringstream buffer;
        buffer << ifs.rdbuf();

        ConfigFileParser parser( variables, buffer );
        if (!parser.load()) throw std::runtime_error( "invalid config file format" );
      }

      void saveToFile( const std::string filename )
      {
        std::ifstream ifs( filename.c_str() );
        assert( ifs.good() );
        std::ostringstream buffer;
        buffer << ifs.rdbuf();

        ConfigFileParser parser( variables, buffer );
        if (!parser.merge()) throw std::runtime_error( "invalid config file format" );

        std::ofstream ofs( filename.c_str() );
        assert( ofs.good() );
        ofs.write( buffer.str().c_str(), buffer.str().size() );
      }

    };

  }
}

#endif
