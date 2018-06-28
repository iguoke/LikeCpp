#include <iostream>
#include <string>
#include <boost/regex.hpp>

int main()
{
    std::string str = "192.168.1.1";

    boost::regex expression("([0-9]+).([0-9]+).([0-9]+)");
    boost::smatch what;

    if(boost::regex_search(str, what, expression))
    {
        std::cout << what.size() << std::endl;
        for(size_t i = 0; i < what.size(); i++)
        {
            if(what[i].matched)
            {
                std::cout << what[i] << std::endl;
            }
        }
    }

    return 0;
}
