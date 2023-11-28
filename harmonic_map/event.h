#pragma once

#include <functional>

namespace DartLib
{
template<typename... Args>
class Event
{
public:
    template<typename FunctionT>
    void setCallback(FunctionT&& fun)
    {
        m_fun = std::forward<FunctionT>(fun);
    }

    void operator()(Args... args_)
    {
        if (m_fun)
        {
            m_fun(args_...);
        }
    }

protected:
    std::function<void(Args...)> m_fun;
};
}
