#include "signals_handler.h"

signals_handler::sig_handler_signature signals_handler::create_sigint_handler(server& srv,
        std::vector<std::weak_ptr<base_workers_keeper>> workers)
{
    return [&srv, workers](const boost::system::error_code&, int) {
        srv.stop_accepting();

        for(auto worker : workers)
        {
            auto worker_sptr = worker.lock();
            if(worker_sptr)
                worker_sptr->stop_workers();
        }

        std::exit(0);
    };
}
