#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

vector<int> random_numbers_vector;
mutex mutex_;
condition_variable cv_;
bool cv_flag = true;

void publisher(int vector_length)
{
    for (int i = 0; i < vector_length; i++)
    {
        if (cv_flag)
        {
            unique_lock<mutex> lock(mutex_);
            int random_number = rand();
            random_numbers_vector.push_back(random_number);
            cout<<__func__<<": publishing number: " << random_number << endl;
            cv_flag = false;
        }

        while (!cv_flag) {
            unique_lock<mutex> lock(mutex_);
            cv_.wait(lock);
        }
    }
}

void extractor(int vector_length)
{
    for (int i = 0; i < vector_length; i++)
    {
        if (!cv_flag) {
            unique_lock<mutex> lock(mutex_);
            //mutex_.lock();
            cout << __func__ << ": extracing number-" << i << " = " << random_numbers_vector.back() << endl;
            random_numbers_vector.pop_back();
            cv_flag = true;
            cv_.notify_one();
            //mutex_.unlock();
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main()
{
    int iterator = 10;
    thread publish(publisher, iterator);
    thread extract(extractor, iterator);

    publish.join();
    extract.join();
}

