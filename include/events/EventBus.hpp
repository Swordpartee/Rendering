#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>

namespace ParteeEngine 
{
    class Event;

    class EventBus 
    {
        public:
            template <typename T>
            void subscribe(std::function<void(const T &)> callback);

            template <typename T>
            void emit(const T &e);

            static EventBus &instance()
            {
                static EventBus bus;
                return bus;
            };

        private:
            std::unordered_map<std::type_index, std::vector<std::function<void(const void *)>>> subscribers;
    };

    template <typename T>
    void EventBus::subscribe(std::function<void(const T &)> callback) 
    {
        auto type = std::type_index(typeid(T));
        subscribers[type].push_back([callback](const void *e)
                                    { callback(*static_cast<const T *>(e)); });
    };

    template <typename T>
    void EventBus::emit(const T &e) 
    {
        auto type = std::type_index(typeid(T));
        for (auto& fn : subscribers[type]) 
        {
            fn(&e);
        }
    };
}   //namespace ParteeEngine