#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <boost/filesystem.hpp>
#include <fstream>

namespace file_hlp {

template<typename vector_t>
inline bool LoadFile( const std::string& strUtf8FileName, vector_t& vecDest, const std::locale& locale = std::locale("") )
{
    vecDest.clear();

    boost::filesystem::path filePath( strUtf8FileName );

    if( !boost::filesystem::exists(filePath) )
    {
        return false;
    }

    std::basic_ifstream<typename vector_t::value_type> ifsFile( strUtf8FileName.c_str(), std::ios_base::in | std::ios_base::binary );

    if( !ifsFile )
    {
        return false;
    }

    ifsFile.imbue(locale);

    boost::uintmax_t fileSize = boost::filesystem::file_size( filePath );

    if( 0 != fileSize )
    {
        vecDest.resize( static_cast< size_t >(fileSize) );
        if( !ifsFile.read( &vecDest[0], static_cast< std::streamsize >( fileSize ) ) )
        {
            vecDest.clear();
            return false;
        }
    }
    else
    {
        vecDest.clear();
    }

    ifsFile.close();

    return true;
}

template <typename vector_t>
inline bool SaveFile( const std::string& fileName,
               vector_t& vecDest,
               bool hasCreateDir = true,
               bool hasTruncate = true,
               const std::locale& locale = std::locale("") )
{
    if( hasCreateDir )
    {
        std::string::size_type slash_pos = fileName.find_last_of("/\\");
        if(slash_pos != std::string::npos)
        {
            std::string dir = fileName.substr(0, slash_pos);
            if( !boost::filesystem::exists(dir) )
            {
                boost::filesystem::create_directories(dir);
            }
        }

    }

    std::basic_ofstream<typename vector_t::value_type> file( fileName.c_str(),
                                                            std::ios_base::binary | std::ios_base::out |( hasTruncate ? std::ios_base::trunc : std::ios_base::app ) );

    if(!file) return false;

    file.imbue(locale);
    file.seekp(0, std::ios_base::end);

    if( vecDest.empty() )
    {
        return false;
    }

    file.write(&vecDest[0], vecDest.size());

    file.close();

    return true;
}

} // namespace file_hlp

#endif // FILE_UTILS_H
