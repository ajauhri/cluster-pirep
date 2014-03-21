// Idea taken from https://stackoverflow.com/questions/1120140/csv-parser-in-c
#ifndef CSV_HPP_INCLUDED
#define CSV_HPP_INCLUED
#include<iterator>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>

class csv_row
{
    public:
        std::string const& operator[](std::size_t index) const
        {
            return m_data[index];
        }
        std::size_t size() const
        {
            return m_data.size();
        }
        void readNextRow(std::istream& str)
        {
            std::string         line;
            std::getline(str,line);

            std::stringstream   lineStream(line);
            std::string         cell;

            m_data.clear();
            while(std::getline(lineStream,cell,','))
            {
                m_data.push_back(cell);
            }
        }
    private:
        std::vector<std::string>    m_data;
};

std::istream& operator>>(std::istream& str,csv_row& data)
{
    data.readNextRow(str);
    return str;
}  
#endif
