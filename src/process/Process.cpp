#include "Process.hpp"

Process::Process(pid_t pid, Status status = Status::STARTING) : _pid(pid), _status(status), _start_time(std::time(nullptr)) {}
Process::~Process() = default;