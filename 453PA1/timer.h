#include <time.h>

class timer {
  public:
    void start() {
      begTime = clock();
    }

    float elapsedTime() {
      return ((float) clock() - begTime) / CLOCKS_PER_SEC;
    }

  private:
    unsigned long begTime;
};
