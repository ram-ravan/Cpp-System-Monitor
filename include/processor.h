#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    std::string line, cpu;
    long user = 0,
    nice = 0,
    system = 0,
    idle = 0,
    iowait = 0, 
    irq = 0,
    softirq = 0, 
    steal = 0,
    guest = 0, 
    guest_nice = 0;

    long prev_user = 0,
    prev_nice = 0,
    prev_system = 0,
    prev_idle = 0,
    prev_iowait = 0, 
    prev_irq = 0,
    prev_softirq = 0, 
    prev_steal = 0,
    prev_guest = 0, 
    prev_guest_nice = 0;

    long prevIdle = 0, 
    currentIdle = 0, 
    prevNonIdle = 0,
    currentNOnIdle = 0,
    PrevTotal = 0,
    currentTotal = 0,
    totald = 0,
    idled = 0;

    float cpu_percentage = 0.0;
};

#endif