#include "reasoner/reasoner.h"

#include "swi-cpp/SWI-cpp.h"

// Needed for specifying custom signal handler
#include <signal.h>

#include <iostream>

#include <map>

//#include <ros/ros.h>

// ----------------------------------------------------------------------------------------------------

Reasoner::Reasoner()
{
    // Initialize Prolog Engine
    putenv((char*)"SWI_HOME_DIR=/usr/lib/swi-prolog");

    char** args = new char*[1];
    args[0] = new char[1000];
    strcpy(args[0], "/");

    prolog_engine_ = new PlEngine(1, args);

    // Prolog has its own signal handler which seems to interfere with ROS' handler
    // Therefore, make sure ROS is shutdown if Prolog receives an interrupt signal
    PL_signal(SIGINT, &Reasoner::sighandler);
}

// ----------------------------------------------------------------------------------------------------

Reasoner::~Reasoner()
{
}

// ----------------------------------------------------------------------------------------------------

bool Reasoner::query(const std::string& query_str, QueryResult& res)
{
    PlTermv av(1);

    PlTerm query;
    std::map<std::string, PlTerm> str_to_var;


    // Convert the string to prolog term, and get the variable bindings

    PlTerm var_bindings;

    PlTermv args(3);
    args[0] = PlAtom(query_str.c_str());
    args[1] = query;
    args[2] = var_bindings;

    av[0] = PlCompound("atom_to_term", args);

    try
    {
        PlQuery q("call", av);
        while(q.next_solution())
        {
            PlTail pl_list(var_bindings);
            PlTerm var_binding;
            while(pl_list.next(var_binding))
                str_to_var[(char*)var_binding[1]] = var_binding[2];

            // Run the query, and show the result

            PlTermv a(1);
            a[0] = query;

            try
            {
                PlQuery q("call", a);

                while(q.next_solution())
                {
                    res.push_back(std::map<std::string, std::string>());
                    std::map<std::string, std::string>& bindings = res.back();

                    for(std::map<std::string, PlTerm>::const_iterator it = str_to_var.begin(); it != str_to_var.end(); ++it)
                    {
                        bindings[it->first] = (char*)it->second;
                    }
                }

                return true;
            }
            catch ( PlException &ex )
            {
                std::cerr << (char *)ex << std::endl;
            }
        }
    }
    catch ( PlException &ex )
    {
        std::cerr << (char *)ex << std::endl;
        return false;
    }

    return false;
}

// ----------------------------------------------------------------------------------------------------

//bool Reasoner::loadDatabase(const std::string& filename)
//{
//    return PlCall("consult", PlTermv(filename.c_str()));
//}

// ----------------------------------------------------------------------------------------------------

void Reasoner::sighandler(int signo)
{
//    ros::shutdown();
}

