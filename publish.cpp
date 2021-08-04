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
bool cv_flag = false;

void publisher(int vector_length)
{

    unique_lock<mutex> lock(mutex_);

    for (int i = 0; i < vector_length; i++)
    {

        while (!cv_flag)
        {

            cv_.wait(lock);
        }

        int random_number = rand();
        random_numbers_vector.push_back(random_number);
        cout << random_number;
        cv_flag = true;
    }
}

void extractor(int vector_length)
{

    unique_lock<mutex> lock(mutex_);
    //RAII->unlock automatically
    //always unlocks when it gets out of scope
    //never use rescources without unique or lockguards

    for (int i = 0; i < vector_length; i++)
    {

        while (random_numbers_vector.size() < 1)
        {
            cv_flag = false;
            cv_.notify_one();

            this_thread::sleep_for(chrono::seconds(1));
        }

        cout << "number" << i << " = " << random_numbers_vector.back() << endl;

        random_numbers_vector.pop_back();

        cv_flag = false;
        cv_.notify_one();
    }
}

//google style guide
//

int main()
{

    int iterator = 10;
    thread publish(publisher, iterator);
    thread extract(extractor, iterator);

    publish.join();
    extract.join();
}