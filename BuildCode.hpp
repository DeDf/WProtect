#ifndef BUILD_INFO_HPP
#define BUILD_INFO_HPP

#include <list>

struct Code_Piece
{
  long CodeStartAddr;
  long CodeSize;
}; 

class BuildExeInfo {
    char * build_exe_name;
    std::list<Code_Piece> list_CodePiece;
  public:
    BuildExeInfo(char * _filename)
    {
        build_exe_name = _filename;
    }
    ~BuildExeInfo()
    {}

    typedef std::list<Code_Piece>::iterator iterator;

    iterator begin()
    {
        return list_CodePiece.begin();
    }

    iterator end()
    {
        return list_CodePiece.end();
    }

    void push_back(Code_Piece & _piece)
    {
      list_CodePiece.push_back(_piece);
    }

    void for_each(void (*fuc)(iterator _iter))
    {
        for (iterator i = begin(); i != end(); i++)
        {
            fuc(i);
        }
    }

    char * get_filename()
    {
      return build_exe_name;
    }
};

#endif /* BUILD_INFO_HPP */
