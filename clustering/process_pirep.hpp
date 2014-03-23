#ifndef PROCESS_PIREP_INCLUDED
#define PROCESS_PIREP_INCLUDED
#define _USE_MATH_DEFINES
#define _FEET_TO_NM 0.000164579 
#define _EARTH_RADIUS_NM 3440.06

#include<cmath>
#include<Eigen/Dense>
#include<boost/date_time.hpp>
#include<csv.hpp>
/* Resources for converting spherical coordinates to cartesian cooordinates - 
//http://rbrundritt.wordpress.com/2008/10/14/conversion-between-spherical-and-cartesian-coordinates-systems/
https://stackoverflow.com/questions/1185408/converting-from-longitude-latitude-to-cartesian-coordinates  
*/

namespace bt = boost::posix_time;
using Eigen::MatrixXd;

time_t pt_to_time_t(const bt::ptime& pt)
{
    bt::ptime timet_start(boost::gregorian::date(1970,1,1));
    bt::time_duration diff = pt - timet_start;
    return diff.ticks()/bt::time_duration::rep_type::ticks_per_second;
}


std::string erase_quotes(const std::string& str)
{
    std::string stripped(str);
    stripped.erase(0, 1);
    stripped.erase(stripped.size() - 1);
    return stripped;
}

MatrixXd get_cartesian(float lat, float lon, float alt)
{
    MatrixXd r(3, 1);
    float R = alt + _EARTH_RADIUS_NM;
    r(0, 0) = R * cos(lat) * cos(lon);
    r(1, 0) = R * cos(lat) * sin(lon);
    r(2, 0) = R * sin(lat);
    return r;
}

MatrixXd process_pirep(const std::string& filename)
{
    std::ifstream file(filename.c_str()); 
    csv_row row;
    int size = 0;

    //determine the size of the file
    while(file >> row)
        size++;
    file.clear();
    file.seekg(0, std::ios::beg); 
    MatrixXd X(size,4);
    
    bt::ptime pt;
    // 1 - timestamp
    // 12 - Latitude
    // 13 - Longitude
    // 14 - Altitude (ft.)
    
    for (int i=0; i <size; ++i)
    {
        file >> row;
        std::istringstream is(row[1]);
        is.imbue(std::locale(std::locale::classic(), new bt::time_input_facet("\"%Y-%m-%d %H:%M:%S\"")));
        is >> pt;
        X(i, 0) = pt_to_time_t(pt);
        X(i, 1) = atof(erase_quotes(row[12]).c_str()) * M_PI / 180;
        X(i, 2) = atof(erase_quotes(row[13]).c_str()) * M_PI / 180;
        X(i, 3) = atof(erase_quotes(row[14]).c_str()) * _FEET_TO_NM;
        X.block<1, 3>(i, 1) = get_cartesian(X(i, 1), X(i, 2), X(i, 3)).transpose();
    }
    return X.rightCols(X.cols()-1);
}
#endif

