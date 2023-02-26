#include <iostream>
#include <random>
#include "TrafficLight.h"
#include <chrono>
/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.

    std::lock_guard<std::mutex> uLock(_mutexMQ);
    _queue.emplace_back(msg); // FIXME: push with move ?
    _conditionMQ.notify_one();

}


/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
    /// https://knowledge.udacity.com/questions/272558
    TrafficObject::threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases,this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles.

    _currentPhase = red;

    auto t_start = std::chrono::high_resolution_clock::now();
    std::random_device rd;
    std::mt19937 rng(rd());
    int min_sec = 4;
    int max_sec = 6;
    std::uniform_int_distribution<int> uni(min_sec,max_sec); // Guaranteed unbiased
    auto toggle_sec = uni(rng);

    while(1){

        auto t_curr = std::chrono::high_resolution_clock::now();
        double elapsed_time_sec = std::chrono::duration<double, std::milli>(t_curr-t_start).count()/1000;

        if(elapsed_time_sec >= toggle_sec){

            if(_currentPhase == red){
                _currentPhase = green;
            }
            else {
                _currentPhase = red;
            }
            toggle_sec = uni(rng);
            t_start = std::chrono::high_resolution_clock::now();

            // https://knowledge.udacity.com/questions/459283
            _message.send(std::move(_currentPhase));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

