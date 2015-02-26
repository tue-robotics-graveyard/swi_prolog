#include <reasoner/reasoner.h>

#include <iostream>

#include <swi_prolog/Query.h>

#include <ros/node_handle.h>
#include <ros/service_server.h>

Reasoner* reasoner;

// ----------------------------------------------------------------------------------------------------

bool querySrv(swi_prolog::Query::Request& req, swi_prolog::Query::Response& res)
{
    QueryResult qres;
    reasoner->query(req.query, qres);

    for(QueryResult::const_iterator it = qres.begin(); it != qres.end(); ++it)
    {
        res.bindings.push_back(swi_prolog::Bindings());
        swi_prolog::Bindings& bindings_msg = res.bindings.back();

        const Bindings& bindings = *it;
        for(Bindings::const_iterator it_b = bindings.begin(); it_b != bindings.end(); ++it_b)
        {
            const std::string& var = it_b->first;
            const std::string& value = it_b->second;

            bindings_msg.variables.push_back(var);
            bindings_msg.values.push_back(value);
        }
    }

    return true;
}

// ----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{

    Reasoner r;
    reasoner = &r;

    ros::init(argc, argv, "reasoner_server");
    ros::NodeHandle n;

    ros::ServiceServer service = n.advertiseService("reasoner/query", querySrv);
    ROS_INFO("Reasoner server ready");
    ros::spin();
}
