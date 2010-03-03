/**
 * $Id: cppunit_output.cpp 319 2009-06-08 22:17:47Z unwesen $
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

#include <cppunit/Test.h>
#include <cppunit/TestFailure.h>

#include <fhtagn/util/cppunit_output.h>
#include <assert.h>

namespace fhtagn {
namespace util {

namespace {

/** Helper struct to produce padding on an output stream. **/
struct pad
{
    pad(fhtagn::size_t pad_length, char pad_char = ' ')
        : m_pad_length(pad_length)
        , m_pad_char(pad_char)
    {
    }

    virtual ~pad()
    {
    }

    virtual void output(std::ostream & os) const
    {
        for (fhtagn::size_t i = 0 ; i < m_pad_length ; ++i) {
            os << m_pad_char;
        }
    }

    friend std::ostream & operator<<(std::ostream & os, pad const & p)
    {
        p.output(os);
        return os;
    }

    fhtagn::size_t  m_pad_length;
    char            m_pad_char;
};


/** Helper struct for indenting output on an output stream **/
struct indent
    : public pad
{
    indent(fhtagn::size_t indent_level, fhtagn::size_t pad_size)
        : pad(indent_level * pad_size)
    {
    }
};

/** Simplify use of the indent struct */
#define INDENT indent((m_results.size() - 1), m_indent_by)


/** Helper struct for drawing a line on an output stream **/
struct line
    : public indent
{
    line(fhtagn::size_t indent_level, fhtagn::size_t pad_size, char line_char,
            fhtagn::size_t max_line)
        : indent(indent_level, pad_size)
    {
        m_pad_length = max_line - m_pad_length;
        m_pad_char = line_char;
    }
};

/** Simplify use of the line struct */
#define LINE(line_char) line((m_results.size() - 1), m_indent_by, line_char, \
        m_max_line)

} // anonymous namespace


/*****************************************************************************
 * VerboseOutput implementation
 **/

VerboseOutput::VerboseOutput(std::ostream & os,
        fhtagn::size_t indent_by /* = 2 */, fhtagn::size_t max_line /* = 79 */)
    : m_results()
    , m_status(ST_OK)
    , m_os(os)
    , m_indent_by(indent_by)
    , m_max_line(max_line)
{
    results tmp = { 0, 0, 0 };
    m_results.push(tmp);
}



// Called when just before a TestCase is run.
void
VerboseOutput::startTest(CppUnit::Test * test)
{
    std::string label = "Running `" + test->getName() + "'";
    fhtagn::size_t pad_size = m_max_line - label.size() - INDENT.m_pad_length - 12;
    if (pad_size > m_max_line) {
        pad_size = 1;
    }
    m_os << INDENT << label << "..." << pad(pad_size);
}


// Called when a failure occurs while running a test.
void
VerboseOutput::addFailure(CppUnit::TestFailure const & failure)
{
    m_status = (failure.isError() ? ST_ERROR : ST_FAILURE);
}


// Called just after a TestCase was run (even if a failure occured).
void
VerboseOutput::endTest(CppUnit::Test * test)
{
    switch (m_status) {
        case ST_OK:
            m_os << "OK" << std::endl;
            ++(m_results.top().successes);
            break;
        case ST_FAILURE:
            m_os << "FAILURE" << std::endl;
            ++(m_results.top().failures);
            break;
        case ST_ERROR:
            m_os << "ERROR" << std::endl;
            ++(m_results.top().errors);
            break;
        default:
            assert(0);
    }
    m_status = ST_OK;
}


// Called by a TestComposite just before running its child tests.
void
VerboseOutput::startSuite(CppUnit::Test * suite)
{
    m_os << INDENT << LINE('=') << std::endl;
    m_os << indent(m_results.size(), m_indent_by) << "Starting test suite `"
        << suite->getName() << "' with " << suite->getChildTestCount()
        << " children" << std::endl;
    m_os << INDENT << LINE('-') << std::endl;

    results tmp = { 0, 0, 0 };
    m_results.push(tmp);
}

// Called by a TestComposite after running its child tests.
void
VerboseOutput::endSuite(CppUnit::Test * suite)
{
    results suite_results = m_results.top();
    m_results.pop();

    m_os << INDENT << LINE('-') << std::endl;
    m_os << indent(m_results.size(), m_indent_by) << "Suite `" << suite->getName()
              << "' - Total: " << (suite_results.successes + suite_results.failures + suite_results.errors)
              << "   Success: " << suite_results.successes
              << "   Failure: " << suite_results.failures
              << "   Error: " << suite_results.errors << std::endl << std::endl;

    m_results.top().successes += suite_results.successes;
    m_results.top().failures += suite_results.failures;
    m_results.top().errors += suite_results.errors;
}


// Called by a TestRunner before running the test.
void
VerboseOutput::startTestRun(CppUnit::Test * test,
        CppUnit::TestResult * eventManager)
{
    m_os << std::endl;
}


// Called by a TestRunner after running the test.
void
VerboseOutput::endTestRun(CppUnit::Test * test,
        CppUnit::TestResult * eventManager)
{
    m_os << LINE('=') << std::endl;
    if (m_results.top().errors || m_results.top().failures) {
        m_os << INDENT << "Details of errors and failures follow:"
            << std::endl << std::endl;
    }
}


/*****************************************************************************
 * HTMLOutput implementation
 **/


HTMLOutput::HTMLOutput(std::ostream & os)
    : m_results()
    , m_status(ST_OK)
    , m_os(os)
{
    results tmp = { 0, 0, 0 };
    m_results.push(tmp);
}



// Called when just before a TestCase is run.
void
HTMLOutput::startTest(CppUnit::Test * test)
{
    // FIXME div around name?
    m_os << "<table class=\"test-run\"><tr class=\"test-run\">" << std::endl;
    m_os << "<td class=\"test-label\">Running " << test->getName() << "...</td>" << std::endl;
    m_os << "<td class=\"test-output\"><pre class=\"test-output\">";
}


// Called when a failure occurs while running a test.
void
HTMLOutput::addFailure(CppUnit::TestFailure const & failure)
{
    m_status = (failure.isError() ? ST_ERROR : ST_FAILURE);
}


// Called just after a TestCase was run (even if a failure occured).
void
HTMLOutput::endTest(CppUnit::Test * test)
{
    // close test output
    m_os << "</pre></td>" << std::endl;

    switch (m_status) {
        case ST_OK:
            m_os << "<td class=\"test-success\">OK</td>" << std::endl;
            ++(m_results.top().successes);
            break;
        case ST_FAILURE:
            m_os << "<td class=\"test-failure\">FAILURE</td>" << std::endl;
            ++(m_results.top().failures);
            break;
        case ST_ERROR:
            m_os << "<td class=\"test-error\">ERROR</td>" << std::endl;
            ++(m_results.top().errors);
            break;
        default:
            assert(0);
    }
    m_os << "</tr></table>" << std::endl;

    m_status = ST_OK;
}


// Called by a TestComposite just before running its child tests.
void
HTMLOutput::startSuite(CppUnit::Test * suite)
{
    m_os << "<table class=\"suite\">" << std::endl;
    m_os << "<tr class=\"suite-header\">" << std::endl;
    m_os << "<th class=\"suite-header\">" << std::endl;
    m_os << "Starting test suite <a class=\"suite-title\" href=\"#\">"
        << suite->getName() << "</a> with " << suite->getChildTestCount()
        << " children" << std::endl
        << "</th>" << std::endl
        << "</tr>" << std::endl;

    m_os << "<tr class=\"suite-content\">" << std::endl;

    results tmp = { 0, 0, 0 };
    m_results.push(tmp);
}

// Called by a TestComposite after running its child tests.
void
HTMLOutput::endSuite(CppUnit::Test * suite)
{
    results suite_results = m_results.top();
    m_results.pop();

    m_os << "</tr>" << std::endl;

    m_os << "<tr class=\"suite-summary\">" << std::endl
         << "<td class=\"suite-summary\"><table class=\"suite-summary\"><tr class=\"suite-summary-inner\">" << std::endl;
    m_os << "<td class=\"suite-summary-label\">Suite <a class=\"suite-title\" href=\"#\">" << suite->getName()
         << "</a></td>" << std::endl
         << "<td class=\"suite-summary-total-label\">Total:</td>" << std::endl
         << "<td class=\"suite-summary-total\">" << (suite_results.successes + suite_results.failures + suite_results.errors) << "</td>" << std::endl
         << "<td class=\"suite-summary-success-label\">Success:</td>" << std::endl
         << "<td class=\"suite-summary-success\">" << suite_results.successes << "</td>" << std::endl
         << "<td class=\"suite-summary-failure-label\">Failure:</td>" << std::endl
         << "<td class=\"suite-summary-failure\">" << suite_results.failures << "</td>" << std::endl
         << "<td class=\"suite-summary-error-label\">Error:</td>" << std::endl
         << "<td class=\"suite-summary-error\">" << suite_results.errors << "</td>" << std::endl
         ;
    m_os << "</tr></table></td>" << std::endl;
    m_os << "</tr>" << std::endl;

    m_results.top().successes += suite_results.successes;
    m_results.top().failures += suite_results.failures;
    m_results.top().errors += suite_results.errors;
}


// Called by a TestRunner before running the test.
void
HTMLOutput::startTestRun(CppUnit::Test * test,
        CppUnit::TestResult * eventManager)
{
}


// Called by a TestRunner after running the test.
void
HTMLOutput::endTestRun(CppUnit::Test * test,
        CppUnit::TestResult * eventManager)
{
    m_os << "Details of errors and failures follow:" << std::endl;
}



}} // namespace fhtagn::util
