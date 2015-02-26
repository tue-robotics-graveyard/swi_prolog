#include <reasoner/reasoner.h>

#include <iostream>

int main(int argc, char **argv) {

    Reasoner r;

    r.query("assert(foo(bla))");
    r.query("assert(foo(blop))");

    QueryResult res;
    r.query("foo(X)", res);

    for(QueryResult::const_iterator it = res.begin(); it != res.end(); ++it)
    {
        const Bindings& bindings = *it;
        for(Bindings::const_iterator it_b = bindings.begin(); it_b != bindings.end(); ++it_b)
        {
            const std::string& var = it_b->first;
            const std::string& value = it_b->second;

            std::cout << var << " = " << value << std::endl;
        }

        std::cout << ";" << std::endl;
    }

}
