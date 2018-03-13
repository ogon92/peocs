#include <fstream>
#include <iostream>

using namespace std;

class FileUtil
{
  private:
    fstream file;

  public:
    static FileUtil &getInstance()
    {
        static FileUtil instance;
        return instance;
    }

    void close()
    {
        if (file.is_open())
            file.close();
    }

    fstream &getFile(string filename)
    {
        if (!file.is_open())
            file.open(filename, std::ios::out);

        return file;
    }

    ~FileUtil()
    {
        if (file.is_open())
            file.close();
    }
};
