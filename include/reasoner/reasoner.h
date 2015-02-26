#ifndef _REASONER_H_
#define _REASONER_H_

#include <string>
#include <vector>
#include <map>

class PlEngine;

typedef std::map<std::string, std::string> Bindings;
typedef std::vector<Bindings> QueryResult;

class Reasoner
{

public:

    Reasoner();

    ~Reasoner();

    bool query(const std::string& query_str)
    {
        QueryResult res;
        return query(query_str, res);
    }

    bool query(const std::string& query_str, QueryResult& res);

private:

    PlEngine* prolog_engine_;

    static void sighandler(int signo);

};


#endif
