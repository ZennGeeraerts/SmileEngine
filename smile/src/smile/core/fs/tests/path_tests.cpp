/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2025 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        path_tests.cpp
 * @author      Zenn Geeraerts
 * @created     25 November 2025
 * @brief       File path unit tests
 */
#include "smile/core/fs/path.h"

#include <catch/catch.hpp>

namespace smile::fs
{
    TEST_CASE( "Path tests", "[file]" )
    {
        SECTION( "GetDirectory" )
        {
            Path path;

            path = "HelloWorld/Test/test_file.xml";

            CHECK( "HelloWorld/Test/" == path.GetDirectory() );

            path = "HelloWorld/Test/";

            CHECK( "HelloWorld/" == path.GetDirectory() );

            path = "/Test/";

            CHECK( "/" == path.GetDirectory() );

            path = "/Hello";

            CHECK( "/" == path.GetDirectory() );

            path = "/";

            CHECK( "" == path.GetDirectory() );
        }

        SECTION( "GetStem" )
        {
            Path path{ "HelloWorld/Test/test_file.xml" };

            CHECK( "test_file" == path.GetStem() );
        }

        SECTION( "GetFileName" )
        {
            Path path{ "HelloWorld/Test/test_file.xml" };

            CHECK( "test_file.xml" == path.GetFileName() );
        }

        SECTION( "GetExtension" )
        {
            Path path{ "HelloWorld/Test/test_file.xml" };

            CHECK( ".xml" == path.GetExtension() );
        }

        SECTION( "std::sort" )
        {
            primitive::Vector< Path > vector{ "second", "first", "xxx", "third" };

            std::sort( vector.begin(), vector.end() );

            CHECK( vector[0] == "first" );
            CHECK( vector[1] == "second" );
            CHECK( vector[2] == "third" );
            CHECK( vector[3] == "xxx" );
        }

        SECTION( "CanonicalizePath" )
        {
            Path path;

            path = "./HELLO/WORLD/test.path";
            path.CanonicalizePath();

            CHECK( "./HELLO/WORLD/test.path" == path );

            path = "../HELLO/WORLD/test.path";
            path.CanonicalizePath();

            CHECK( "../HELLO/WORLD/test.path" == path );

            path = "c:/HELLO/./WORLD/./test.path";
            path.CanonicalizePath();

            CHECK( "c:/HELLO/WORLD/test.path" == path );

            path = "c:/HELLO/././WORLD/test.path";
            path.CanonicalizePath();

            CHECK( "c:/HELLO/WORLD/test.path" == path );

            path = "/HELLO/./WORLD/./test.path";
            path.CanonicalizePath();

            CHECK( "/HELLO/WORLD/test.path" == path );

            path = "c:/HELLO/../WORLD/test.path";
            path.CanonicalizePath();

            CHECK( "c:/WORLD/test.path" == path );

            path = "/HELLO/../WORLD/test.path";
            path.CanonicalizePath();

            CHECK( "/WORLD/test.path" == path );

            path = "HELLO/../../WORLD/test.path";
            path.CanonicalizePath();

            CHECK( "../WORLD/test.path" == path );

            path = "/HELLO/WORLD/../../test.path";
            path.CanonicalizePath();

            CHECK( "/test.path" == path );
        }
    }
}