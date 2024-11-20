#include <iostream>
#include <pthread.h>
#include <chrono>
class Integer {
private:
  	int inner_value = 0;
  	pthread_mutex_t mutex;
public:
  Integer(int value) :inner_value(value) {
  	pthread_mutex_init(&mutex, nullptr);
  }
  ~Integer(){
  	pthread_mutex_destroy(&mutex);
  }
  void inc() {
	pthread_mutex_lock(&mutex);
    	inner_value++;
    	pthread_mutex_unlock(&mutex);
  }
  int get_inner_value() {
    	return inner_value;
  }
};

void* increment(void* arg) {
  Integer* num = (Integer*)arg;
  for (int i = 0; i < 10000; ++i) {
    	num->inc();
  }
	return nullptr;
}

int main(int argc, char** argv) {
  Integer* num = new Integer(0);
  int thread_count = 50;
  pthread_t* tids = new pthread_t[thread_count];

  auto start_time = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < thread_count; ++i) {
	pthread_create(&tids[i], NULL, increment, num);  
  }

  for (int i = 0; i < thread_count; ++i) {
	pthread_join(tids[i], NULL);
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_time = end_time - start_time;
  std::cout << "Final value is " << num->get_inner_value() << std::endl;
  std::cout << "Mutex time: " << elapsed_time.count() << " seconds" << std::endl;

  delete[] tids;
  delete num;
  return 0;
}
