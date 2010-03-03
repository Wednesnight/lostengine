/**
 * $Id: transcoding_test.cpp 319 2009-06-08 22:17:47Z unwesen $
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

#include <boost/bind.hpp>

#include <cppunit/extensions/HelperMacros.h>

#include <fhtagn/text/transcoding.h>
#include <fhtagn/text/decoders.h>
#include <fhtagn/text/encoders.h>
#include <fhtagn/text/xparser.h>

namespace {


struct test_grammar
    : public boost::spirit::grammar<test_grammar>
{
    inline void append(fhtagn::text::utf32_char_t ch) const
    {
        result.push_back(ch);
    }

    inline void check_set_encoding(fhtagn::text::utf32_char_t ch) const
    {
        // Very primitive syntax for setting an encoding. If the character value
        // corresponds to one of the defined encodings <= 24 (max at the time of
        // writing), we'll set the encoding accordingly. If not, we'll treat ch
        // as regular input.
        if (ch <= 24) {
            factory.set_encoding(static_cast<fhtagn::text::char_encoding_type>(ch));
            return;
        }
        append(ch);
    }


    template <typename ScannerT>
    struct definition
    {
        definition(test_grammar const & self)
        {
            test_rule = self.factory.anychar_p[
                    boost::bind(&test_grammar::check_set_encoding, boost::ref(self), _1)
                ]
                >> +self.factory.anychar_p[
                    boost::bind(&test_grammar::append, boost::ref(self), _1)
                ];
            BOOST_SPIRIT_DEBUG_NODE(test_rule);
        }

        boost::spirit::rule<ScannerT> test_rule;

        boost::spirit::rule<ScannerT> const & start() const { return test_rule; }
    };


    mutable fhtagn::text::char_parser_factory factory;
    mutable fhtagn::text::utf32_string        result;
};


} // anonymous namespace

class TranscodingTest
    : public CppUnit::TestFixture
{
public:
    CPPUNIT_TEST_SUITE(TranscodingTest);

        CPPUNIT_TEST(testDecodeRaw);
        CPPUNIT_TEST(testDecodeASCII);
        CPPUNIT_TEST(testDecodeISO_8859_15);
        CPPUNIT_TEST(testDecodeCP_1252);
        CPPUNIT_TEST(testDecodeUTF_8);
        CPPUNIT_TEST(testDecodeUTF_16);
        CPPUNIT_TEST(testDecodeUTF_32);
        CPPUNIT_TEST(testDecodeUniversal);

        CPPUNIT_TEST(testEncodeRaw);
        CPPUNIT_TEST(testEncodeASCII);
        CPPUNIT_TEST(testEncodeISO_8859_15);
        CPPUNIT_TEST(testEncodeCP_1252);
        CPPUNIT_TEST(testEncodeUTF_8);
        CPPUNIT_TEST(testEncodeUTF_16);
        CPPUNIT_TEST(testEncodeUTF_32);
        CPPUNIT_TEST(testEncodeUniversal);

        CPPUNIT_TEST(testChunkedTranscoding);

        CPPUNIT_TEST(testTranscode);

        CPPUNIT_TEST(testXParser);
        CPPUNIT_TEST(testBOMParser);

    CPPUNIT_TEST_SUITE_END();
private:


    void testDecodeRaw()
    {
        namespace t = fhtagn::text;

        std::string source = "Hello, \xf3 world!";
        t::utf32_string target;

        t::raw_decoder decoder;
        t::decode(decoder, source.begin(), source.end(),
                std::back_insert_iterator<t::utf32_string>(target));

        CPPUNIT_ASSERT_EQUAL(source.size(), target.size());
        for (boost::uint32_t i = 0 ; i < source.size() ; ++i) {
            // assign to uint32_t to ensure that comparison is fair, without
            // the same signedness, etc.
            boost::uint32_t expected = static_cast<unsigned char>(source[i]);
            boost::uint32_t got = target[i];
            CPPUNIT_ASSERT_EQUAL(expected, got);
        }
    }


    void testDecodeASCII()
    {
        namespace t = fhtagn::text;
        t::ascii_decoder dec;

        // decode an ASCII string
        {
            std::string source = "Hello, world!";
            t::utf32_char_t expected_array[] = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string expected = expected_array;
            t::utf32_string target;
            t::ascii_decoder decoder;
            t::decode(decoder, source.begin(), source.end(),
                    std::back_insert_iterator<t::utf32_string>(target));
            CPPUNIT_ASSERT_EQUAL(expected.size(), target.size());
            // can't use CPPUNIT_ASSERT_EQUAL because no streaming operator is
            // defined for utf32_strings.
            CPPUNIT_ASSERT(expected == target);
        }

        // test decoding into a static utf32_char_t array, i.e. non-string
        // targets
        {
            std::string source = "Hello, world!";
            t::utf32_char_t target[sizeof("Hello, world!")] = { 0 };
            t::ascii_decoder decoder;
            t::decode(decoder, source.begin(), source.end(), target);
         }

        // ensure that an extended ASCII string will be decoded with replacement
        // chars...
        {
            std::string source = "Hello, \xf3 world!";
            t::utf32_string target;
            t::ascii_decoder decoder;
            std::string::iterator error_iter = t::decode(decoder,
                    source.begin(), source.end(),
                    std::back_insert_iterator<t::utf32_string>(target));
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), target.size());
            CPPUNIT_ASSERT_EQUAL(static_cast<std::string::difference_type>(15),
                    (error_iter - source.begin()));
            // ensure that instead of the invalid 0xf3, a replacement char
            // is produced
            CPPUNIT_ASSERT_EQUAL(static_cast<t::utf32_char_t>(0xfffd), target[7]);
        }

        // ... or decoding ends if replacement chars are not desired
        {
            std::string source = "Hello, \xf3 world!";
            t::utf32_string target;
            t::ascii_decoder decoder;
            decoder.use_replacement_char(false);
            std::string::iterator error_iter = t::decode(decoder,
                    source.begin(), source.end(),
                    std::back_insert_iterator<t::utf32_string>(target));
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(7), target.size());
            CPPUNIT_ASSERT_EQUAL(static_cast<std::string::difference_type>(7),
                    (error_iter - source.begin()));
        }
    }


    void testDecodeISO_8859_15()
    {
        namespace t = fhtagn::text;

        // \xa4 is the euro sign in ISO-8859-15
        std::string source = "Hello, \xa4 world!";
        t::utf32_string target;
        t::iso8859_15_decoder decoder;
        std::string::iterator error_iter = t::decode(decoder,
                source.begin(), source.end(),
                std::back_insert_iterator<t::utf32_string>(target));
        CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), target.size());
        CPPUNIT_ASSERT_EQUAL(static_cast<std::string::difference_type>(15),
                (error_iter - source.begin()));
        // ensure that the euro sign has been decoded properly
        CPPUNIT_ASSERT_EQUAL(static_cast<t::utf32_char_t>(0x20ac), target[7]);
    }

    void testDecodeCP_1252()
    {
        namespace t = fhtagn::text;

        // \x80 is the euro sign in CP-1252
        std::string source = "Hello, \x80 world!";
        t::utf32_string target;
        t::cp1252_decoder decoder;
        std::string::iterator error_iter = t::decode(decoder,
                source.begin(), source.end(),
                std::back_insert_iterator<t::utf32_string>(target));
        CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), target.size());
        CPPUNIT_ASSERT_EQUAL(static_cast<std::string::difference_type>(15),
                (error_iter - source.begin()));
        // ensure that the euro sign has been decoded properly
        CPPUNIT_ASSERT_EQUAL(static_cast<t::utf32_char_t>(0x20ac), target[7]);
    }


    void testDecodeUTF_8()
    {
        namespace t = fhtagn::text;

        // \xe2\x82\xac is the euro sign in UTF-8
        std::string source = "Hello, \xe2\x82\xac world!";
        t::utf32_string target;
        t::utf8_decoder decoder;
        std::string::iterator error_iter = t::decode(decoder,
                source.begin(), source.end(),
                std::back_insert_iterator<t::utf32_string>(target));
        CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), target.size());
        CPPUNIT_ASSERT_EQUAL(static_cast<std::string::difference_type>(17),
                (error_iter - source.begin()));
        // ensure that the euro sign has been decoded properly
        CPPUNIT_ASSERT_EQUAL(static_cast<t::utf32_char_t>(0x20ac), target[7]);
    }

    void testDecodeUTF_16()
    {
        namespace t = fhtagn::text;

        // unicode code point 119070 (hex 1D11E) is musical G clef, which
        // is an example of code points encoded in four bytes in UTF-16:
        // Numeric values: D834 DD1E
        //
        // LE: 34 D8 1E DD
        char le_source[] = { '\xff', '\xfe', 'H',    '\x00', 'e', '\x00', 'l', '\x00',
                             'l',    '\x00', 'o',    '\x00', ',', '\x00', ' ', '\x00',
                             '\x34', '\xd8', '\x1e', '\xdd', ' ', '\x00', 'w', '\x00',
                             'o',    '\x00', 'r',    '\x00', 'l', '\x00', 'd', '\x00',
                             '!', '\x00' };
        // BE: D8 34 DD 1E
        char be_source[] = { '\xfe', '\xff', '\x00', 'H',    '\x00', 'e', '\x00', 'l',
                             '\x00', 'l',    '\x00', 'o',    '\x00', ',', '\x00', ' ',
                             '\xd8', '\x34', '\xdd', '\x1e', '\x00', ' ', '\x00', 'w',
                             '\x00', 'o',    '\x00', 'r',    '\x00', 'l', '\x00', 'd',
                             '\x00', '!' };

        // UTF-16LE decoding, with a specialized utf16le_decoder
        t::utf32_string target1;
        {
            t::utf16le_decoder decoder;
            // add 2 to start & end to skip BOM
            char * error_ptr = t::decode(decoder,
                    le_source + 2, le_source + 34,
                    std::back_insert_iterator<t::utf32_string>(target1));
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), target1.size());
            CPPUNIT_ASSERT_EQUAL(error_ptr, le_source + 34);
            // ensure that the G clef sign has been decoded properly
            CPPUNIT_ASSERT_EQUAL(static_cast<t::utf32_char_t>(0x1d11e), target1[7]);
        }

        // UTF-16BE decoding, with a specialized utf16be_decoder
        t::utf32_string target2;
        {
            t::utf16be_decoder decoder;
            // add 2 to start & end to skip BOM
            char * error_ptr = t::decode(decoder,
                    be_source + 2, be_source + 34,
                    std::back_insert_iterator<t::utf32_string>(target2));
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), target2.size());
            CPPUNIT_ASSERT_EQUAL(error_ptr, be_source + 34);
            // ensure that the G clef sign has been decoded properly
            CPPUNIT_ASSERT_EQUAL(static_cast<t::utf32_char_t>(0x1d11e), target2[7]);
        }

        // compare both conversions
        CPPUNIT_ASSERT(target1 == target2);

        // UTF-16LE decoding based on a BOM
        {
            t::utf32_string target;
            t::utf16_decoder decoder;
            char * error_ptr = t::decode(decoder,
                    le_source, le_source + 34,
                    std::back_insert_iterator<t::utf32_string>(target));
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), target.size());
            CPPUNIT_ASSERT_EQUAL(error_ptr, le_source + 34);
            // ensure that the G clef sign has been decoded properly
            CPPUNIT_ASSERT_EQUAL(static_cast<t::utf32_char_t>(0x1d11e), target[7]);
        }

        // UTF-16BE decoding based on a BOM
        {
            t::utf32_string target;
            t::utf16_decoder decoder;
            char * error_ptr = t::decode(decoder,
                    be_source, be_source + 34,
                    std::back_insert_iterator<t::utf32_string>(target));
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), target.size());
            CPPUNIT_ASSERT_EQUAL(error_ptr, be_source + 34);
            // ensure that the G clef sign has been decoded properly
            CPPUNIT_ASSERT_EQUAL(static_cast<t::utf32_char_t>(0x1d11e), target[7]);
        }

    }


    void testDecodeUTF_32()
    {
        namespace t = fhtagn::text;

        // unicode code point 119070 (hex 1D11E) is musical G clef, which
        // is an example of code points encoded in four bytes in UTF-32:
        char le_source[] = "\xff\xfe\0\0H\0\0\0e\0\0\0l\0\0\0l\0\0\0o\0\0\0"
                           ",\0\0\0 \0\0\0\x1e\xd1\x01\0 \0\0\0w\0\0\0o\0\0\0"
                           "r\0\0\0l\0\0\0d\0\0\0!\0\0\0";
        char be_source[] = "\0\0\xfe\xff\0\0\0H\0\0\0e\0\0\0l\0\0\0l\0\0\0o"
                           "\0\0\0,\0\0\0 \0\x01\xd1\x1e\0\0\0 \0\0\0w\0\0\0o"
                           "\0\0\0r\0\0\0l\0\0\0d\0\0\0!";

        // UTF-32LE decoding, with a specialized utf32le_decoder
        t::utf32_string target1;
        {
            t::utf32le_decoder decoder;
            // add 4 to start & end to skip BOM
            char * error_ptr = t::decode(decoder,
                    le_source + 4, le_source + 64,
                    std::back_insert_iterator<t::utf32_string>(target1));
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), target1.size());
            CPPUNIT_ASSERT_EQUAL(error_ptr, le_source + 64);
            // ensure that the G clef sign has been decoded properly
            CPPUNIT_ASSERT_EQUAL(static_cast<t::utf32_char_t>(0x1d11e), target1[7]);
        }

        // UTF-32BE decoding, with a specialized utf32be_decoder
        t::utf32_string target2;
        {
            t::utf32be_decoder decoder;
            // add 2 to start & end to skip BOM
            char * error_ptr = t::decode(decoder,
                    be_source + 4, be_source + 64,
                    std::back_insert_iterator<t::utf32_string>(target2));
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), target2.size());
            CPPUNIT_ASSERT_EQUAL(error_ptr, be_source + 64);
            // ensure that the G clef sign has been decoded properly
            CPPUNIT_ASSERT_EQUAL(static_cast<t::utf32_char_t>(0x1d11e), target2[7]);
        }

        // compare both conversions
        CPPUNIT_ASSERT(target1 == target2);

        // UTF-32LE decoding based on a BOM
        {
            t::utf32_string target;
            t::utf32_decoder decoder;
            char * error_ptr = t::decode(decoder,
                    le_source, le_source + 64,
                    std::back_insert_iterator<t::utf32_string>(target));
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), target.size());
            CPPUNIT_ASSERT_EQUAL(error_ptr, le_source + 64);
            // ensure that the G clef sign has been decoded properly
            CPPUNIT_ASSERT_EQUAL(static_cast<t::utf32_char_t>(0x1d11e), target[7]);
        }

        // UTF-32BE decoding based on a BOM
        {
            t::utf32_string target;
            t::utf32_decoder decoder;
            char * error_ptr = t::decode(decoder,
                    be_source, be_source + 64,
                    std::back_insert_iterator<t::utf32_string>(target));
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), target.size());
            CPPUNIT_ASSERT_EQUAL(error_ptr, be_source + 64);
            // ensure that the G clef sign has been decoded properly
            CPPUNIT_ASSERT_EQUAL(static_cast<t::utf32_char_t>(0x1d11e), target[7]);
        }
    }


    void testDecodeUniversal()
    {
        namespace t = fhtagn::text;

        {
            // \xe2\x82\xac is the euro sign in UTF-8
            std::string source = "Hello, \xe2\x82\xac world!";
            t::utf32_string target;
            t::universal_decoder decoder;
            decoder.set_encoding(t::UTF_8);
            std::string::iterator error_iter = t::decode(decoder,
                    source.begin(), source.end(),
                    std::back_insert_iterator<t::utf32_string>(target));
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), target.size());
            CPPUNIT_ASSERT_EQUAL(static_cast<std::string::difference_type>(17),
                    (error_iter - source.begin()));
            // ensure that the euro sign has been decoded properly
            CPPUNIT_ASSERT_EQUAL(static_cast<t::utf32_char_t>(0x20ac), target[7]);
        }

        {
            // \xf3 is illegal in ASCII
            std::string source = "Hello, \xf3 world!";
            t::utf32_string target;
            t::universal_decoder decoder;
            std::string::iterator error_iter = t::decode(decoder,
                    source.begin(), source.end(),
                    std::back_insert_iterator<t::utf32_string>(target));
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), target.size());
            CPPUNIT_ASSERT_EQUAL(static_cast<std::string::difference_type>(15),
                    (error_iter - source.begin()));
            // ensure that the euro sign has been decoded properly
            CPPUNIT_ASSERT_EQUAL(static_cast<t::utf32_char_t>(0xfffd), target[7]);
        }

    }


    void testEncodeRaw()
    {
        namespace t = fhtagn::text;

        t::utf32_char_t source_array[] = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 0xf3, ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
        t::utf32_string source = source_array;

        std::string target;

        t::raw_encoder encoder;
        t::encode(encoder, source.begin(), source.end(),
                std::back_insert_iterator<std::string>(target));

        CPPUNIT_ASSERT_EQUAL(source.size(), target.size());
        for (boost::uint32_t i = 0 ; i < source.size() ; ++i) {
            // assign to uint32_t to ensure that comparison is fair, without
            // the same signedness, etc.
            boost::uint32_t expected = source[i];
            boost::uint32_t got = static_cast<unsigned char>(target[i]);
            CPPUNIT_ASSERT_EQUAL(expected, got);
        }
    }


    void testEncodeASCII()
    {
        namespace t = fhtagn::text;

        // simple test, expected to succeed.
        {
            t::utf32_char_t source_array[] = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string source = source_array;

            std::string expected = "Hello, world!";

            std::string target;
            t::ascii_encoder encoder;
            t::utf32_string::const_iterator error_iter = t::encode(encoder, source.begin(),
                    source.end(), std::back_insert_iterator<std::string>(target));
            CPPUNIT_ASSERT(source.end() == error_iter);
            CPPUNIT_ASSERT_EQUAL(expected, target);
        }

        // test with non-ASCII character (a umlaut). Because the replacement
        // char cannot be encoded in ASCII, we merely expect this char to be
        // skipped in the output
        {
            t::utf32_char_t source_array[] = { 'H', 'e', 'l', 'l', 'o', ',', 0xe4, ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string source = source_array;

            std::string expected = "Hello, world!";

            std::string target;
            t::ascii_encoder encoder;
            t::utf32_string::const_iterator error_iter = t::encode(encoder, source.begin(),
                    source.end(), std::back_insert_iterator<std::string>(target));
            CPPUNIT_ASSERT(source.end() == error_iter);
            CPPUNIT_ASSERT_EQUAL(expected, target);
        }

        // Sometimes you see ASCII text encoding unknown characters as ? - let's
        // see if that works.
        {
            t::utf32_char_t source_array[] = { 'H', 'e', 'l', 'l', 'o', ',', 0xe4, ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string source = source_array;

            std::string expected = "Hello,? world!";

            std::string target;
            t::ascii_encoder encoder;
            encoder.replacement_char('?');
            t::utf32_string::const_iterator error_iter = t::encode(encoder, source.begin(),
                    source.end(), std::back_insert_iterator<std::string>(target));
            CPPUNIT_ASSERT(source.end() == error_iter);
            CPPUNIT_ASSERT_EQUAL(expected, target);
        }

        // lastly, ensure that when we want to fail at invalid characters we
        // fail at the correct input position.
        {
            t::utf32_char_t source_array[] = { 'H', 'e', 'l', 'l', 'o', ',', 0xe4, ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string source = source_array;

            std::string expected = "Hello,";

            std::string target;
            t::ascii_encoder encoder;
            encoder.use_replacement_char(false);
            t::utf32_string::const_iterator error_iter = t::encode(encoder, source.begin(),
                    source.end(), std::back_insert_iterator<std::string>(target));
            CPPUNIT_ASSERT(source.begin() + 6 == error_iter);
            CPPUNIT_ASSERT_EQUAL(expected, target);
        }
    }

    void testEncodeISO_8859_15()
    {
        namespace t = fhtagn::text;

        // simple test, expected to succeed.
        {
            t::utf32_char_t source_array[] = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string source = source_array;

            std::string expected = "Hello, world!";

            std::string target;
            t::iso8859_15_encoder encoder;
            t::utf32_string::const_iterator error_iter = t::encode(encoder, source.begin(),
                    source.end(), std::back_insert_iterator<std::string>(target));
            CPPUNIT_ASSERT(source.end() == error_iter);
            CPPUNIT_ASSERT_EQUAL(expected, target);
        }

        // test with non-ASCII character (a umlaut). This must be valid in iso-8859-15
        {
            t::utf32_char_t source_array[] = { 'H', 'e', 'l', 'l', 'o', ',', 0xe4, ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string source = source_array;

            std::string expected = "Hello,\xe4 world!";

            std::string target;
            t::iso8859_15_encoder encoder;
            t::utf32_string::const_iterator error_iter = t::encode(encoder, source.begin(),
                    source.end(), std::back_insert_iterator<std::string>(target));
            CPPUNIT_ASSERT(source.end() == error_iter);
            CPPUNIT_ASSERT_EQUAL(expected, target);
        }

        // test with non-ISO-8859-15 character (G clef) - that's expected to be ignored ...
        {
            t::utf32_char_t source_array[] = { 'H', 'e', 'l', 'l', 'o', ',', 0x1d11e, ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string source = source_array;

            std::string expected = "Hello, world!";

            std::string target;
            t::iso8859_15_encoder encoder;
            t::utf32_string::const_iterator error_iter = t::encode(encoder, source.begin(),
                    source.end(), std::back_insert_iterator<std::string>(target));
            CPPUNIT_ASSERT(source.end() == error_iter);
            CPPUNIT_ASSERT_EQUAL(expected, target);
        }

        // ... or optionally encoded as a ? ...
        {
            t::utf32_char_t source_array[] = { 'H', 'e', 'l', 'l', 'o', ',', 0x1d11e, ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string source = source_array;

            std::string expected = "Hello,? world!";

            std::string target;
            t::iso8859_15_encoder encoder;
            encoder.replacement_char('?');
            t::utf32_string::const_iterator error_iter = t::encode(encoder, source.begin(),
                    source.end(), std::back_insert_iterator<std::string>(target));
            CPPUNIT_ASSERT(source.end() == error_iter);
            CPPUNIT_ASSERT_EQUAL(expected, target);
        }

        // ... or encoding breaks off at that point
        {
            t::utf32_char_t source_array[] = { 'H', 'e', 'l', 'l', 'o', ',', 0x1d11e, ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string source = source_array;

            std::string expected = "Hello,";

            std::string target;
            t::iso8859_15_encoder encoder;
            encoder.use_replacement_char(false);
            t::utf32_string::const_iterator error_iter = t::encode(encoder, source.begin(),
                    source.end(), std::back_insert_iterator<std::string>(target));
            CPPUNIT_ASSERT(source.begin() + 6 == error_iter);
            CPPUNIT_ASSERT_EQUAL(expected, target);
        }
    }



    void testEncodeCP_1252()
    {
        namespace t = fhtagn::text;

        // simple test, expected to succeed.
        {
            t::utf32_char_t source_array[] = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string source = source_array;

            std::string expected = "Hello, world!";

            std::string target;
            t::cp1252_encoder encoder;
            t::utf32_string::const_iterator error_iter = t::encode(encoder, source.begin(),
                    source.end(), std::back_insert_iterator<std::string>(target));
            CPPUNIT_ASSERT(source.end() == error_iter);
            CPPUNIT_ASSERT_EQUAL(expected, target);
        }

        // test with non-ASCII character (a umlaut). This must be valid in CP-1252
        {
            t::utf32_char_t source_array[] = { 'H', 'e', 'l', 'l', 'o', ',', 0xe4, ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string source = source_array;

            std::string expected = "Hello,\xe4 world!";

            std::string target;
            t::cp1252_encoder encoder;
            t::utf32_string::const_iterator error_iter = t::encode(encoder, source.begin(),
                    source.end(), std::back_insert_iterator<std::string>(target));
            CPPUNIT_ASSERT(source.end() == error_iter);
            CPPUNIT_ASSERT_EQUAL(expected, target);
        }

        // test with non-CP-1252 character (G clef) - that's expected to be ignored ...
        {
            t::utf32_char_t source_array[] = { 'H', 'e', 'l', 'l', 'o', ',', 0x1d11e, ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string source = source_array;

            std::string expected = "Hello, world!";

            std::string target;
            t::cp1252_encoder encoder;
            t::utf32_string::const_iterator error_iter = t::encode(encoder, source.begin(),
                    source.end(), std::back_insert_iterator<std::string>(target));
            CPPUNIT_ASSERT(source.end() == error_iter);
            CPPUNIT_ASSERT_EQUAL(expected, target);
        }

        // ... or optionally encoded as a ? ...
        {
            t::utf32_char_t source_array[] = { 'H', 'e', 'l', 'l', 'o', ',', 0x1d11e, ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string source = source_array;

            std::string expected = "Hello,? world!";

            std::string target;
            t::cp1252_encoder encoder;
            encoder.replacement_char('?');
            t::utf32_string::const_iterator error_iter = t::encode(encoder, source.begin(),
                    source.end(), std::back_insert_iterator<std::string>(target));
            CPPUNIT_ASSERT(source.end() == error_iter);
            CPPUNIT_ASSERT_EQUAL(expected, target);
        }

        // ... or encoding breaks off at that point
        {
            t::utf32_char_t source_array[] = { 'H', 'e', 'l', 'l', 'o', ',', 0x1d11e, ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string source = source_array;

            std::string expected = "Hello,";

            std::string target;
            t::cp1252_encoder encoder;
            encoder.use_replacement_char(false);
            t::utf32_string::const_iterator error_iter = t::encode(encoder, source.begin(),
                    source.end(), std::back_insert_iterator<std::string>(target));
            CPPUNIT_ASSERT(source.begin() + 6 == error_iter);
            CPPUNIT_ASSERT_EQUAL(expected, target);
        }
    }


    void testEncodeUTF_8()
    {
        namespace t = fhtagn::text;

        // simple test, expected to succeed.
        {
            t::utf32_char_t source_array[] = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string source = source_array;

            std::string expected = "Hello, world!";

            std::string target;
            t::utf8_encoder encoder;
            t::utf32_string::const_iterator error_iter = t::encode(encoder, source.begin(),
                    source.end(), std::back_insert_iterator<std::string>(target));
            CPPUNIT_ASSERT(source.end() == error_iter);
            CPPUNIT_ASSERT_EQUAL(expected, target);
        }

        // test with non-ASCII character (a umlaut). This must be valid in UTF-8
        {
            t::utf32_char_t source_array[] = { 'H', 'e', 'l', 'l', 'o', ',', 0xe4, ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string source = source_array;

            std::string expected = "Hello,\xc3\xa4 world!";

            std::string target;
            t::utf8_encoder encoder;
            t::utf32_string::const_iterator error_iter = t::encode(encoder, source.begin(),
                    source.end(), std::back_insert_iterator<std::string>(target));
            CPPUNIT_ASSERT(source.end() == error_iter);
            CPPUNIT_ASSERT_EQUAL(expected, target);
        }

        // test with euro sign - again must be valid
        {
            t::utf32_char_t source_array[] = { 'H', 'e', 'l', 'l', 'o', ',', 0x20ac, ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string source = source_array;

            std::string expected = "Hello,\xe2\x82\xac world!";

            std::string target;
            t::utf8_encoder encoder;
            t::utf32_string::const_iterator error_iter = t::encode(encoder, source.begin(),
                    source.end(), std::back_insert_iterator<std::string>(target));
            CPPUNIT_ASSERT(source.end() == error_iter);
            CPPUNIT_ASSERT_EQUAL(expected, target);
        }
    }


    void testEncodeUTF_16()
    {
        namespace t = fhtagn::text;

        // simple test, expected to succeed.
        {
            t::utf32_char_t source_array[] = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 0x1d11e, ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string source = source_array;
            // unicode code point 119070 (hex 1D11E) is musical G clef, which
            // is an example of code points encoded in four bytes in UTF-16:
            // Numeric values: D834 DD1E
            //
            // LE: 34 D8 1E DD
            char le_target[] = { 'H', '\x00', 'e',    '\x00', 'l',    '\x00',
                                 'l', '\x00', 'o',    '\x00', ',',    '\x00',
                                 ' ', '\x00', '\x34', '\xd8', '\x1e', '\xdd',
                                 ' ', '\x00', 'w',    '\x00', 'o',    '\x00',
                                 'r', '\x00', 'l',    '\x00', 'd',    '\x00',
                                 '!', '\x00' };
            // BE: D8 34 DD 1E
            char be_target[] = { '\x00', 'H', '\x00', 'e',    '\x00', 'l',
                                 '\x00', 'l', '\x00', 'o',    '\x00', ',',
                                 '\x00', ' ', '\xd8', '\x34', '\xdd', '\x1e',
                                 '\x00', ' ', '\x00', 'w',    '\x00', 'o',
                                 '\x00', 'r', '\x00', 'l',    '\x00', 'd',
                                 '\x00', '!' };

            std::string le_expected(le_target, sizeof(le_target));
            std::string be_expected(be_target, sizeof(be_target));

            std::string target;
            t::utf16le_encoder encoder1;
            t::utf32_string::const_iterator error_iter = t::encode(encoder1, source.begin(),
                    source.end(), std::back_insert_iterator<std::string>(target));
            CPPUNIT_ASSERT(source.end() == error_iter);
            CPPUNIT_ASSERT_EQUAL(le_expected, target);

            target.clear();
            t::utf16be_encoder encoder2;
            error_iter = t::encode(encoder2, source.begin(), source.end(),
                    std::back_insert_iterator<std::string>(target));
            CPPUNIT_ASSERT(source.end() == error_iter);
            CPPUNIT_ASSERT_EQUAL(be_expected, target);
        }
    }


    void testEncodeUTF_32()
    {
        namespace t = fhtagn::text;
        namespace b = fhtagn::byte_order;

        // unicode code point 119070 (hex 1D11E) is musical G clef, which
        // is an example of code points encoded in four bytes in UTF-32:
        char le_source[] = "\xff\xfe\0\0H\0\0\0e\0\0\0l\0\0\0l\0\0\0o\0\0\0"
                           ",\0\0\0 \0\0\0\x1e\xd1\x01\0 \0\0\0w\0\0\0o\0\0\0"
                           "r\0\0\0l\0\0\0d\0\0\0!\0\0\0";
        char be_source[] = "\0\0\xfe\xff\0\0\0H\0\0\0e\0\0\0l\0\0\0l\0\0\0o"
                           "\0\0\0,\0\0\0 \0\x01\xd1\x1e\0\0\0 \0\0\0w\0\0\0o"
                           "\0\0\0r\0\0\0l\0\0\0d\0\0\0!";

        // UTF-32LE decoding, with a specialized utf32le_decoder
        t::utf32_string source_string;
        {
            t::utf32le_decoder decoder;
            // add 2 to start & end to skip BOM
            char * error_ptr = t::decode(decoder,
                    le_source + 4, le_source + 64,
                    std::back_insert_iterator<t::utf32_string>(source_string));
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), source_string.size());
            CPPUNIT_ASSERT_EQUAL(error_ptr, le_source + 64);
            // ensure that the G clef sign has been decoded properly
            CPPUNIT_ASSERT_EQUAL(static_cast<t::utf32_char_t>(0x1d11e), source_string[7]);
        }

        // Encode as UTF-32BE and compare
        {
            t::utf32be_encoder encoder;
            char buf[sizeof(be_source)] = { 0 };
            t::encode(encoder, source_string.begin(), source_string.end(), buf);

            for (boost::uint32_t i = 0 ; i < sizeof(be_source) - 4 ; ++i) {
                CPPUNIT_ASSERT_EQUAL(be_source[i + 4], buf[i]);
            }
        }

        // Encode as UTF-32LE and compare
        {
            t::utf32le_encoder encoder;
            char buf[sizeof(be_source)] = { 0 };
            t::encode(encoder, source_string.begin(), source_string.end(), buf);

            for (boost::uint32_t i = 0 ; i < sizeof(le_source) - 4 ; ++i) {
                CPPUNIT_ASSERT_EQUAL(le_source[i + 4], buf[i]);
            }
        }


        // Now we can encode this with a utf32_encoder, and depending on platform,
        // it should come out identical to le_source or be_source (without bom).
        {
            t::utf32_encoder encoder;
            char buf[sizeof(be_source)] = { 0 };
            t::encode(encoder, source_string.begin(), source_string.end(), buf);

            char * expected = NULL;
            if (b::host_byte_order() == b::FHTAGN_BIG_ENDIAN) {
                expected = be_source + 4;
            } else {
                expected = le_source + 4;
            }

            for (boost::uint32_t i = 0 ; i < sizeof(be_source) - 4 ; ++i) {
                CPPUNIT_ASSERT_EQUAL(expected[i], buf[i]);
            }
        }
    }


    void testEncodeUniversal()
    {
        namespace t = fhtagn::text;

        // unicode code point 119070 (hex 1D11E) is musical G clef, which
        // is an example of code points encoded in four bytes in UTF-32:
        char le_source[] = "\xff\xfe\0\0H\0\0\0e\0\0\0l\0\0\0l\0\0\0o\0\0\0"
                           ",\0\0\0 \0\0\0\x1e\xd1\x01\0 \0\0\0w\0\0\0o\0\0\0"
                           "r\0\0\0l\0\0\0d\0\0\0!\0\0\0";

        // UTF-32LE decoding, with a specialized utf32le_decoder
        t::utf32_string source_string;
        {
            t::utf32le_decoder decoder;
            // add 2 to start & end to skip BOM
            char * error_ptr = t::decode(decoder,
                    le_source + 4, le_source + 64,
                    std::back_insert_iterator<t::utf32_string>(source_string));
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), source_string.size());
            CPPUNIT_ASSERT_EQUAL(error_ptr, le_source + 64);
            // ensure that the G clef sign has been decoded properly
            CPPUNIT_ASSERT_EQUAL(static_cast<t::utf32_char_t>(0x1d11e), source_string[7]);
        }

        // Now we can encode the utf32-string in source_string into UTF-8...
        {
            std::string target;
            t::universal_encoder encoder;
            encoder.set_encoding(t::UTF_8);
            t::utf32_string::iterator error_iter = t::encode(encoder,
                    source_string.begin(), source_string.end(),
                    std::back_insert_iterator<std::string>(target));
            // G clef becomes several bytes.
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(18), target.size());
        }

        // ... and try the same with ASCII
        {
            std::string target;
            t::universal_encoder encoder;
            encoder.replacement_char('?');
            t::utf32_string::iterator error_iter = t::encode(encoder,
                    source_string.begin(), source_string.end(),
                    std::back_insert_iterator<std::string>(target));
            // G clef will be replaced by '?'
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), target.size());
            CPPUNIT_ASSERT_EQUAL('?', target[7]);
        }
    }



    void testChunkedTranscoding()
    {
        namespace t = fhtagn::text;

        // chunked decoding
        // \xe2\x82\xac is the euro sign in UTF-8
        std::string source = "Hello, \xe2\x82\xac world!";
        {
            t::utf32_string target;

            std::string::iterator iter = source.begin();
            t::utf8_decoder decoder;
            while (iter != source.end()) {
                t::utf32_char_t buf[4] = { 0 };
                fhtagn::ssize_t bufsize = 4;
                iter = t::decode(decoder, iter, source.end(), buf, bufsize);
                target.append(buf, bufsize);
            }
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), target.size());
            CPPUNIT_ASSERT_EQUAL(static_cast<std::string::difference_type>(17),
                    (iter - source.begin()));
            // ensure that the euro sign has been decoded properly
            CPPUNIT_ASSERT_EQUAL(static_cast<t::utf32_char_t>(0x20ac), target[7]);
        }

        t::utf32_string target;
        {
            std::string::iterator iter = source.begin();
            t::utf8_decoder decoder;
            while (iter != source.end()) {
                t::utf32_char_t buf;
                fhtagn::ssize_t bufsize = 1;
                iter = t::decode(decoder, iter, source.end(), &buf, bufsize);
                target.append(&buf, bufsize);
            }
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), target.size());
            CPPUNIT_ASSERT_EQUAL(static_cast<std::string::difference_type>(17),
                    (iter - source.begin()));
            // ensure that the euro sign has been decoded properly
            CPPUNIT_ASSERT_EQUAL(static_cast<t::utf32_char_t>(0x20ac), target[7]);
        }

        // chunked encoding
        {
            std::string output;

            t::utf32_string::iterator iter = target.begin();
            t::utf8_encoder encoder;
            while (iter != target.end()) {
                char buf[4] = { 0 };
                fhtagn::ssize_t bufsize = 4;
                iter = t::encode(encoder, iter, target.end(), buf, bufsize);
                CPPUNIT_ASSERT(bufsize <= 4);
                output.append(buf, bufsize);
            }
            CPPUNIT_ASSERT(target.end() == iter);
            CPPUNIT_ASSERT_EQUAL(source.size(), output.size());
            CPPUNIT_ASSERT_EQUAL(source, output);
        }
    }


    void testTranscode()
    {
        namespace t = fhtagn::text;

        // transcode euro sign, mostly
        {
            std::string source = "Hello, \xe2\x82\xac world!";
            std::string target;

            // the call gets more confusing because we want to specify a weird and
            // small temporary buffer size.
            t::utf8_decoder decoder;
            t::iso8859_15_encoder encoder;
            t::transcode(decoder, source.begin(), source.end(), encoder,
                    std::back_insert_iterator<std::string>(target));

            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(15), target.size());
            CPPUNIT_ASSERT_EQUAL(std::string::value_type(0xa4), target[7]);
        }

        // transcode with an illegal input character
        {
            std::string source = "Hello, \xf3 world!";
            std::string target;

            // the call gets more confusing because we want to specify a weird and
            // small temporary buffer size.
            t::ascii_decoder decoder;
            t::iso8859_15_encoder encoder;
            std::string::iterator iter = t::transcode(decoder, source.begin(),
                    source.end(), encoder,
                    std::back_insert_iterator<std::string>(target));

            CPPUNIT_ASSERT_EQUAL(static_cast<ptrdiff_t>(7), iter - source.begin());
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(7), target.size());
        }

        // transcode with an illegal output character
        {
            std::string source = "Hello, \xe2\x82\xac world!";
            std::string target;

            // the call gets more confusing because we want to specify a weird and
            // small temporary buffer size.
            t::utf8_decoder decoder;
            t::ascii_encoder encoder;
            std::string::iterator iter = t::transcode(decoder, source.begin(),
                    source.end(), encoder,
                    std::back_insert_iterator<std::string>(target));

            CPPUNIT_ASSERT_EQUAL(static_cast<ptrdiff_t>(7), iter - source.begin());
            CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(7), target.size());
        }
    }



    void testXParser()
    {
        namespace t = fhtagn::text;

        // default parse
        {
            test_grammar grammar;
            BOOST_SPIRIT_DEBUG_NODE(grammar);

            t::utf32_char_t expected_array[] = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string expected = expected_array;
            CPPUNIT_ASSERT_EQUAL(true, boost::spirit::parse("Hello, world!", grammar).full);
            CPPUNIT_ASSERT(expected == grammar.result);
        }

        // parse UTF-8
        {
            test_grammar grammar;
            BOOST_SPIRIT_DEBUG_NODE(grammar);
            grammar.factory.set_encoding(t::UTF_8);

            t::utf32_char_t expected_array[] = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 0x20ac, ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string expected = expected_array;
            CPPUNIT_ASSERT_EQUAL(true, boost::spirit::parse("Hello, \xe2\x82\xac world!", grammar).full);
            CPPUNIT_ASSERT(expected == grammar.result);
        }

        // parse determining the encoding from the input #1
        {
            test_grammar grammar;
            BOOST_SPIRIT_DEBUG_NODE(grammar);

            t::utf32_char_t expected_array[] = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 0x20ac, ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string expected = expected_array;
            CPPUNIT_ASSERT_EQUAL(true, boost::spirit::parse("\x01Hello, \xe2\x82\xac world!", grammar).full);
            CPPUNIT_ASSERT(expected == grammar.result);
        }

        // parse determining the encoding from the input #2
        {
            test_grammar grammar;
            BOOST_SPIRIT_DEBUG_NODE(grammar);

            t::utf32_char_t expected_array[] = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 0x20ac, ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
            t::utf32_string expected = expected_array;
            CPPUNIT_ASSERT_EQUAL(true, boost::spirit::parse("\x18Hello, \x80 world!", grammar).full);
            CPPUNIT_ASSERT(expected == grammar.result);
        }
    }

    void testBOMParser()
    {
        namespace t = fhtagn::text;

        // parse bom - test whether utf-8 bom is detected properly
        {
            t::bom_parser parser;
            CPPUNIT_ASSERT_EQUAL(true, boost::spirit::parse("\xef\xbb\xbf", parser).full);
            CPPUNIT_ASSERT_EQUAL((int) t::UTF_8, (int) parser.encoding);
        }

        // parse bom - invalid bom must result in RAW encoding
        {
            t::bom_parser parser;
            CPPUNIT_ASSERT_EQUAL(false, boost::spirit::parse("\xef\xbb", parser).full);
            CPPUNIT_ASSERT_EQUAL((int) t::RAW, (int) parser.encoding);
        }

    }
};


CPPUNIT_TEST_SUITE_REGISTRATION(TranscodingTest);
