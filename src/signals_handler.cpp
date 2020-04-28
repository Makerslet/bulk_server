#include "signals_handler.h"
#include "statistic_formatter.h"

#include <iostream>

void signals_handler::set_state(std::shared_ptr<command_handler> handler,
                                std::vector<std::weak_ptr<base_workers_keeper>> wk)
{
    _handler = handler;
    _workers_keepers = wk;
}
void signals_handler::handle_sigint(int)
{
    auto handler_sptr = _handler.lock();
    if(handler_sptr)
    {
        handler_sptr->stop_handling();
        std::cout << statistic_formatter::format(handler_sptr->statistic()) << std::endl;
    }

    for(auto worker : _workers_keepers)
    {
        auto worker_sptr = worker.lock();
        if(worker_sptr)
        {
            size_t num_th_workers = worker_sptr->num_workers();
            for(size_t th_worker = 0; th_worker < num_th_workers; ++th_worker)
                std::cout << statistic_formatter::format(worker_sptr->get_worker_context(th_worker)) << std::endl;
        }
    }
    std::exit(0);
}
