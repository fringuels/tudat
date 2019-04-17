/*    Copyright (c) 2010-2018, Delft University of Technology
 *    All rigths reserved
 *
 *    This file is part of the Tudat. Redistribution and use in source and
 *    binary forms, with or without modification, are permitted exclusively
 *    under the terms of the Modified BSD license. You should have received
 *    a copy of the license with this file. If not, please or visit:
 *    http://tudat.tudelft.nl/LICENSE.
 *
 */

#include "Tudat/InputOutput/groundStationsDatabaseReader.h"

#include <iostream>
#include <fstream>
#include <algorithm>

namespace tudat
{
namespace input_output
{

//! Function to parse data file
void groundStationsDatabaseReader::parser(const std::string &inputDataFile )
{
    std::ifstream readFile ( inputDataFile );

    std::ofstream writeFile;
   // writeFile.open ( parsedFilePath_ + "temporaryFile.txt" );
    writeFile.open ( "/Users/valeriofilice/Tudat/tudatBundle/tudat/Tudat/External/GroundStationDatabase/temporaryFile.txt" );

    if ( readFile.is_open( ) )
    {
        std::string line;
        while( getline( readFile, line ) )
        {
            if( line[0] == '!' || line.empty( ) )
            {
                writeFile << line << '\n';
            }
            else if( line.find( "FRAME" ) != std::string::npos )
            {
                writeFile << line.substr(line.find( "FRAME" ) ) << '\n';
            }
            else
            {
                line.erase( std::remove_if( line.begin( ), line.end( ), isspace ), line.end( ) );

                writeFile << line << '\n';
            }
        }
    }
    else
    {
        std::cerr << "Couldn't open input file for reading.\n";
    }

    writeFile.close();

    std::ifstream readFile2 ( "/Users/valeriofilice/Tudat/tudatBundle/tudat/Tudat/External/GroundStationDatabase/temporaryFile.txt" );

    std::ofstream writeFile2;
   // writeFile.open ( parsedFilePath_ + "GS_locations_database.txt" );
    writeFile2.open ( "/Users/valeriofilice/Tudat/tudatBundle/tudat/Tudat/External/GroundStationDatabase/ground_station_locations_database.txt" );

    if ( readFile2.is_open( ) )
    {
        std::string line;
        while( getline( readFile2, line ) )
        {
            if( line[0] == '!' || line.empty( ) )
            {
                writeFile2 << line << '\n';
            }
            else if( line.find( "BEGIN" ) != std::string::npos )
            {
                auto delimiterPos1 = line.find( "BEGIN" );
                auto delimiterPos2 = line.find( "END" );
                auto delimiterPos3 = line.find( "AXISOFF" );

                auto one = line.substr( 0, delimiterPos1 - 1);
                auto two = line.substr( delimiterPos1, delimiterPos2 - delimiterPos1 );
                auto three = line.substr( delimiterPos2, delimiterPos3 - delimiterPos2 );
                auto four = line.substr( delimiterPos3 );

                writeFile2 << one << ";" << two << ";" << three << ";" << four << ";" << '\n';
            }
            else if( line.find( "DBCODE" ) != std::string::npos )
            {
                auto delimiterPos1 = line.find( "DBNAME" );
                auto delimiterPos2 = line.find( "AXISTYPE" );
                auto delimiterPos3 = line.find( "AXISOFF" );

                auto one = line.substr( 0, delimiterPos1 - 1);
                auto two = line.substr( delimiterPos1, delimiterPos2 - delimiterPos1 );
                auto three = line.substr( delimiterPos2, delimiterPos3 - delimiterPos2 );
                auto four = line.substr( delimiterPos3 );

                writeFile2 << one << ";" << two << ";" << three << ";" << four << ";" << '\n';
            }
            else if( line.find( "AXISTYPE" ) != std::string::npos )
            {
                auto delimiterPos1 = line.find( "AXISTYPE" );
                auto delimiterPos2 = line.find( "AXISOFF" );

                auto one = line.substr( 0, delimiterPos1 - 1);
                auto two = line.substr( delimiterPos1, delimiterPos2 - delimiterPos1 );
                auto three = line.substr( delimiterPos2 );

                writeFile2 << one << ";" << two << ";" << three << ";" << '\n';
            }
            else if( line.find( "X=" ) != std::string::npos )
            {
                auto delimiterPos2 = line.find( "Y=" );
                auto delimiterPos3 = line.find( "Z=" );

                auto one = line.substr( 0, delimiterPos2 - 1 );
                auto two = line.substr( delimiterPos2, delimiterPos3 - delimiterPos2 );
                auto three = line.substr( delimiterPos3 );

                writeFile2 << one << ";" << two << ";" << three << ";" << '\n';
            }
            else if( line.find( "DXDT=" ) != std::string::npos )
            {
                auto delimiterPos2 = line.find( "DYDT=" );
                auto delimiterPos3 = line.find( "DZDT=" );
                auto delimiterPos4 = line.find( "EPOCH=" );

                auto one = line.substr( 0, delimiterPos2 - 1 );
                auto two = line.substr( delimiterPos2, delimiterPos3 - delimiterPos2) ;
                auto three = line.substr( delimiterPos3, delimiterPos4 - delimiterPos3 );
                auto four = line.substr( delimiterPos4 );

                writeFile2 << one << ";" << two << ";" << three << ";" << four << ";" << '\n';
            }
            else
            {
                writeFile2 << line << '\n';
            }
        }
    }
    else
    {
        std::cerr << "Couldn't open temporaryFile.txt for reading.\n";
    }
    writeFile2.close();
    remove( "/Users/valeriofilice/Tudat/tudatBundle/tudat/Tudat/External/GroundStationDatabase/temporaryFile.txt" );
}

//! Function to read the Cartesian position of a specific ground station from GS_locations_database.txt file.
Eigen::Vector3d groundStationsDatabaseReader::getGroundStationPositionfromDatabase( std::string gsName )
{
//    std::ifstream cFile ( groundStationsDatabaseFile_ );
    std::ifstream cFile ( "/Users/valeriofilice/Tudat/tudatBundle/tudat/Tudat/External/GroundStationDatabase/ground_station_locations_database.txt" );

    Eigen::Vector3d gsPosition;

    if ( cFile.is_open( ) )
    {
        std::string line;
        while( getline( cFile, line ) )
        {
            if( line[0] == '!' || line.empty( ) )
            {
                continue;
            }
            else if( line.find( gsName ) != std::string::npos )
            {
                getline( cFile, line );

                if( line[0] == '!' )
                {
                    while( line[0] == '!' )
                    {
                        getline( cFile, line );
                    }
                }

                auto delimiterPos2 = line.find( "Y=" );
                auto delimiterPos3 = line.find( "Z=" );

                double x = std::stod(line.substr(2, delimiterPos2 - 2));
                double y = std::stod(line.substr(delimiterPos2 + 2, delimiterPos3 - delimiterPos2 + 2));
                double z = std::stod(line.substr(delimiterPos3 + 2, std::string::npos - 1 ));

                ( gsPosition << x, y, z ).finished( );
                break;
            }

        }

    }
    else {
        std::cerr << "Couldn't open GS_locations_database.txt for reading.\n";
    }

    return gsPosition;
}



} // namespace input_output
} // namespace tudat
