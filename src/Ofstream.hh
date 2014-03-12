#include <iostream>
#include <fstream>

class my_ostream
{
public:
  my_ostream() : my_fstream("some_file.txt") {}; // check if opening file succeeded!!
  // for regular output of variables and stuff
  template<typename T> my_ostream& operator<<(const T& something)
  {
    //std::cout << something;
    my_fstream << something;
    return *this;
  }
  // for manipulators like std::endl
  typedef std::ostream& (*stream_function)(std::ostream&);
  my_ostream& operator<<(stream_function func)
  {
    //func(std::cout);
    func(my_fstream);
    return *this;
  }
private:
  std::ofstream my_fstream;
};

extern my_ostream mout;

