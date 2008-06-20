#ifndef LOST_LSYSTEM_LSYSTEM_H
#define LOST_LSYSTEM_LSYSTEM_H

#include <string>
#include <map>
#include <list>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "lost/math/Matrix.h"
#include "lost/math/Vec3.h"

namespace lost
{
  namespace lsystem
  {

    struct LSystemResult
    {
      char         identifier;
      math::Matrix matrix;
      bool         drawable;
      LSystemResult( const char& inIdentifier, const math::Matrix& inMatrix, const bool inDrawable = true )
      : identifier(inIdentifier),
        matrix(inMatrix),
        drawable(inDrawable)
      {
      }
    };
    typedef std::list<boost::shared_ptr<LSystemResult> > LSystemResultList;

    struct LSystemState
    {
    private:
      math::Vec3                        mAngle;
      std::string                       mAxiom;
      std::map<char, std::string>       mVariableMap;
      std::map<char, LSystemResultList> mReplaceMap;
      LSystemResultList                 mResult;

      void stringToLSystemResultList( const std::string& inString, LSystemResultList& inResultList )
      {
        math::Matrix matrix;
        for (unsigned int idx = 0; idx < inString.size(); ++idx)
        {
          bool drawable = false;
          matrix.zero();
          switch (inString[idx])
          {
            case 'f':
              matrix.initTranslation( math::Vec3(1,1,1) );
              break;
            case 'X':
              matrix.initRotateX( mAngle.x );
              break;
            case 'x':
              matrix.initRotateX( 360-mAngle.x );
              break;
            case 'Y':
              matrix.initRotateY( mAngle.y );
              break;
            case 'y':
              matrix.initRotateY( 360-mAngle.y );
              break;
            case 'Z':
              matrix.initRotateZ( mAngle.z );
              break;
            case 'z':
              matrix.initRotateZ( 360-mAngle.z );
              break;
            case 'F':
            default :
              drawable = true;
              matrix.initTranslation( math::Vec3(1,1,1) );
          }
          inResultList.push_back( boost::shared_ptr<LSystemResult>(new LSystemResult(inString[idx], matrix, drawable)) );
        }
      }

    public:
      LSystemState( const std::string& inAxiom, const std::map<char, std::string>& inVariableMap, const math::Vec3& inAngle )
      : mAngle(inAngle),
        mAxiom(inAxiom),
        mVariableMap(inVariableMap)
      {
      }

      math::Vec3& angle() { return mAngle; }
      void angle( const math::Vec3& inAngle ) { mAngle = inAngle; }

      std::string& axiom() { return mAxiom; }
      void axiom( const std::string& inAxiom ) { mAxiom = inAxiom; }

      std::map<char, LSystemResultList>& replaceMap() { return mReplaceMap; }

      LSystemResultList& result() { return mResult; }

      std::map<char, std::string>& variableMap() { return mVariableMap; }
      void variableMap( const std::map<char, std::string>& inVariableMap ) { mVariableMap = inVariableMap; }

      // reset state with current params
      void reset()
      {
        mReplaceMap.clear();
        mResult.clear();

        stringToLSystemResultList(mAxiom, mResult);
        for (std::map<char, std::string>::iterator idx = mVariableMap.begin(); idx != mVariableMap.end(); ++idx)
        {
          stringToLSystemResultList((*idx).second, mReplaceMap[(*idx).first]);
        }
      }

    };

    struct LSystem
    {
    public:
      LSystem() {}

      // advance given LSystemState (steps = iteration count from current state)
      void advance( boost::shared_ptr<LSystemState>& state, unsigned int steps = 1 )
      {
        for (unsigned int stepCount = 0; stepCount < steps; ++stepCount)
        {
          LSystemResultList&                 result  = state->result();
          std::map<char, LSystemResultList>& replace = state->replaceMap();
          for (LSystemResultList::iterator idx = result.begin(); idx != result.end(); )
          {
            if (replace.find( (*idx)->identifier ) != replace.end())
            {
              char identifier = (*idx)->identifier;
              idx = result.erase(idx);
              result.insert(idx, replace[identifier].begin(), replace[identifier].end());
            }
            else
            {
              ++idx;
            }
          }
        }
      }

      // regress given LSystemState (steps = iteration count from current state)
      void regress( boost::shared_ptr<LSystemState>& state, unsigned int steps = 1 )
      {
        throw std::runtime_error( "lost::lsystem::LSystem::regress() : not implemented" );
      }

      void walk( const boost::shared_ptr<LSystemState>& state, const lost::math::Matrix& startMatrix, const boost::function<void (lost::math::Matrix&, LSystemResult&)> callback )
      {
        std::list<lost::math::Matrix> stack;
        LSystemResultList& result = state->result();
        LSystemResult      currentResult(0, startMatrix, false);
        for (LSystemResultList::iterator idx = result.begin(); idx != result.end(); ++idx)
        {
          switch ((*idx)->identifier)
          {
            case '[':
              stack.push_back(currentResult.matrix);
              break;
            case ']':
              currentResult.matrix = stack.back();
              stack.pop_back();
              break;
            default:
              currentResult.identifier = (*idx)->identifier;
              currentResult.drawable   = (*idx)->drawable;
              if (!(*idx)->drawable)
              {
                currentResult.matrix = currentResult.matrix * (*idx)->matrix;
              }
              callback((*idx)->matrix, currentResult);
          }
        }
      }

    };

  }
}

#endif
