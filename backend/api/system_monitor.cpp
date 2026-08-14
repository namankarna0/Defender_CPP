#include "api/system_monitor.h"

#include <ctime>
#include <fstream>
#include <sstream>
#include <sys/statvfs.h>

namespace {
constexpr const char* kActivityLog = "backend/system-monitor/activity_log.txt";

// Reads /proc/meminfo line by line so the trailing "kB" unit cannot stop parsing.
double read_memory_percent() {
  std::ifstream memory_file("/proc/meminfo");
  std::string line;
  long long total_kib = 0;
  long long available_kib = 0;

  while (std::getline(memory_file, line)) {
    std::istringstream field(line);
    std::string label;
    long long value = 0;
    field >> label >> value;
    if (label == "MemTotal:") total_kib = value;
    if (label == "MemAvailable:") available_kib = value;
  }
  return total_kib > 0 ? 100.0 * (total_kib - available_kib) / total_kib : -1.0;
}

// Calculates CPU use from consecutive aggregate CPU samples.
double read_cpu_percent() {
  std::ifstream stat("/proc/stat");
  std::string label;
  long long user = 0, nice = 0, system = 0, idle = 0;
  long long iowait = 0, irq = 0, softirq = 0, steal = 0;
  stat >> label >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

  const long long total = user + nice + system + idle + iowait + irq + softirq + steal;
  const long long idle_total = idle + iowait;
  static long long previous_total = total;
  static long long previous_idle = idle_total;
  const long long total_delta = total - previous_total;
  const long long idle_delta = idle_total - previous_idle;
  previous_total = total;
  previous_idle = idle_total;
  return total_delta > 0 ? 100.0 * (total_delta - idle_delta) / total_delta : 0.0;
}

// Uses the thermal-zone value when the operating system exposes one.
double read_temperature() {
  std::ifstream sensor("/sys/class/thermal/thermal_zone0/temp");
  long milli_degrees = 0;
  if (!(sensor >> milli_degrees)) return -1.0;
  return milli_degrees / 1000.0;
}

// Persists live readings in the CSV format consumed by the activity-log table.
void append_activity(const std::string& timestamp, double cpu, double memory,
                     double disk, double temperature) {
  std::ifstream existing(kActivityLog, std::ios::binary);
  const bool needs_header = !existing.good() ||
      existing.peek() == std::ifstream::traits_type::eof();
  std::ofstream log(kActivityLog, std::ios::app);
  if (!log) return;
  if (needs_header) log << "timestamp,cpu_percent,mem_percent,disk_percent,temp_celsius\n";
  log << timestamp << ',' << cpu << ',' << memory << ',' << disk << ',' << temperature << '\n';
}
}

// Collects a current host-level metric snapshot for the monitoring API.
std::string system_metrics_json() {
  const double memory = read_memory_percent();
  const double cpu = read_cpu_percent();
  struct statvfs disk {};
  double disk_used = -1.0;
  if (statvfs("/", &disk) == 0 && disk.f_blocks) {
    disk_used = 100.0 * (disk.f_blocks - disk.f_bavail) / disk.f_blocks;
  }
  const double temperature = read_temperature();
  const std::time_t now = std::time(nullptr);
  char timestamp[32];
  std::strftime(timestamp, sizeof timestamp, "%Y-%m-%d %H:%M:%S", std::localtime(&now));
  append_activity(timestamp, cpu, memory, disk_used, temperature);

  std::ostringstream output;
  output << "{\"cpu_percent\":" << cpu << ",\"memory_percent\":" << memory
         << ",\"disk_percent\":" << disk_used << ",\"temp_celsius\":" << temperature
         << ",\"timestamp\":\"" << timestamp << "\"}";
  return output.str();
}

// Converts valid rows in the activity CSV into JSON for the history table.
std::string system_activity_log_json() {
  std::ifstream log(kActivityLog);
  std::string line;
  std::ostringstream output;
  output << '[';
  bool first = true;
  while (std::getline(log, line)) {
    std::stringstream row(line);
    std::string timestamp, cpu, memory, disk, temperature;
    if (!std::getline(row, timestamp, ',') || timestamp == "timestamp" ||
        !std::getline(row, cpu, ',') || !std::getline(row, memory, ',') ||
        !std::getline(row, disk, ',') || !std::getline(row, temperature, ',')) continue;
    try {
      std::stod(cpu); std::stod(memory); std::stod(disk); std::stod(temperature);
    } catch (...) { continue; }
    if (!first) output << ',';
    first = false;
    output << "{\"timestamp\":\"" << timestamp << "\",\"cpu_percent\":" << cpu
           << ",\"memory_percent\":" << memory << ",\"disk_percent\":" << disk
           << ",\"temp_celsius\":" << temperature << '}';
  }
  output << ']';
  return output.str();
}
