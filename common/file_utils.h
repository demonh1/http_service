#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <boost/filesystem.hpp>
#include <fstream>

namespace file_hlp {

template<typename vector_t> inline bool LoadFile( const std::string& strUtf8FileName, vector_t& vecDest, const std::locale& locale = std::locale("") )
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

} // namespace file_hlp

#endif // FILE_UTILS_H
