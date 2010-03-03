/**
 * $Id: cppunit_output.h 319 2009-06-08 22:17:47Z unwesen $
 *
 * This file is part of the Fhtagn! C++ Library.
 * Copyright (C) 2009 Jens Finkhaeuser <unwesen@users.sourceforge.net>.
 *
 * Author: Jens Finkhaeuser <unwesen@users.sourceforge.net>
 *
 * This program is licensed as free software for personal, educational or
 * other non-commerical uses: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Alternatively, licenses for commercial purposes are available as well.
 * Please send your enquiries to the copyright holder's address above.
 **/

#ifndef FHTAGN_UTIL_CPPUNIT_OUTPUT_H
#define FHTAGN_UTIL_CPPUNIT_OUTPUT_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

#include <iostream>
#include <stack>

#include <cppunit/TestListener.h>

namespace fhtagn {
namespace util {

/**
 * The VerboseOutput class implements CppUnit's TestListener interface,
 * and produces verbose output as to which tests are currently running, and
 * whether they succeeded or not. Output is produced on a std::ostream.
 **/
class VerboseOutput
    : public CppUnit::TestListener
{
public:
    /**
     * Constructor
     *
     * @param os Output stream on which to produce verbose test output.
     * @param indent_by [default: 2] Test suites may be nested, and output for
     *    each inner test suite is indented by this amount of spaces from the
     *    output of the enclosing test suite.
     * @param max_line [default: 79] Maximum number of characters per line; this
     *    value is a hint for laying out the output, but lines may still exceed
     *    this limit in extreme cases.
     **/
    VerboseOutput(std::ostream & os, fhtagn::size_t indent_by = 2,
            fhtagn::size_t max_line = 79);

private:
    /**
     * CppUnit distinguishes between tests that succeed, that fail (i.e. where
     * an assertion is not fulfilled), and errors that were not expected by the
     * test suite.
     **/
    enum status
    {
        ST_OK,
        ST_FAILURE,
        ST_ERROR
    };


    /**
     * Structure for counting successes/failures/errors of each test suite.
     **/
    struct results
    {
        boost::uint32_t successes;
        boost::uint32_t failures;
        boost::uint32_t errors;
    };


    /** @see CppUnit::TestListener */
    virtual void startTest(CppUnit::Test * test);

    /** @see CppUnit::TestListener */
    virtual void addFailure(CppUnit::TestFailure const & failure);

    /** @see CppUnit::TestListener */
    virtual void endTest(CppUnit::Test * test);

    /** @see CppUnit::TestListener */
    virtual void startSuite(CppUnit::Test * suite);

    /** @see CppUnit::TestListener */
    virtual void endSuite(CppUnit::Test * suite);

    /** @see CppUnit::TestListener */
    virtual void startTestRun(CppUnit::Test * test,
            CppUnit::TestResult * eventManager);

    /** @see CppUnit::TestListener */
    virtual void endTestRun(CppUnit::Test * test,
            CppUnit::TestResult * eventManager);


    /** Used for counting results in each nested test suite. **/
    std::stack<results> m_results;

    /**
     * Result of individual test runs; must be preserved across a few function
     * calls.
     **/
    status m_status;

    /** Stream to push output to. **/
    std::ostream & m_os;

    /** Indentation depth **/
    fhtagn::size_t m_indent_by;

    /** Maximum line size */
    fhtagn::size_t m_max_line;
};


/**
 * The HTMLOutput class essentially provides the same functionality as
 * VerboseOutput before, but outputs HTML rather than plain text.
 *
 * It's (nearly) fully customizable via CSS in that it draws plain HTML tables,
 * with each element referring to a CSS class.
 **/
class HTMLOutput
    : public CppUnit::TestListener
{
public:
    /**
     * Constructor
     *
     * @param os Output stream on which to produce verbose test output.
     **/
    HTMLOutput(std::ostream & os);

private:
    /**
     * CppUnit distinguishes between tests that succeed, that fail (i.e. where
     * an assertion is not fulfilled), and errors that were not expected by the
     * test suite.
     **/
    enum status
    {
        ST_OK,
        ST_FAILURE,
        ST_ERROR
    };


    /**
     * Structure for counting successes/failures/errors of each test suite.
     **/
    struct results
    {
        boost::uint32_t successes;
        boost::uint32_t failures;
        boost::uint32_t errors;
    };


    /** @see CppUnit::TestListener */
    virtual void startTest(CppUnit::Test * test);

    /** @see CppUnit::TestListener */
    virtual void addFailure(CppUnit::TestFailure const & failure);

    /** @see CppUnit::TestListener */
    virtual void endTest(CppUnit::Test * test);

    /** @see CppUnit::TestListener */
    virtual void startSuite(CppUnit::Test * suite);

    /** @see CppUnit::TestListener */
    virtual void endSuite(CppUnit::Test * suite);

    /** @see CppUnit::TestListener */
    virtual void startTestRun(CppUnit::Test * test,
            CppUnit::TestResult * eventManager);

    /** @see CppUnit::TestListener */
    virtual void endTestRun(CppUnit::Test * test,
            CppUnit::TestResult * eventManager);


    /** Used for counting results in each nested test suite. **/
    std::stack<results> m_results;

    /**
     * Result of individual test runs; must be preserved across a few function
     * calls.
     **/
    status m_status;

    /** Stream to push output to. **/
    std::ostream & m_os;
};

}} // namespace fhtagn::util

#endif // guard
