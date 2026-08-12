#include<iostream>
#include<fstream>
#include<sstream>
#include<chrono>
#include<thread>
#include<ctime>
#include<sys/statvfs.h>
using namespace std;

struct Metrics {
    double cpuPercent;
    double memPercent;
    double diskPercent;
    double tempCelsius;
    string timestamp;
};

class CpuCollector {
private:
    long prevTotal = 0;
    long prevIdle = 0;

public:
    double read() {
        ifstream file("/proc/stat");
        string line;
        getline(file, line);

        istringstream ss(line);
        string cpuLabel;
        long user, nice, system, idle, iowait, irq, softirq, steal;
        ss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

        long idleTime = idle + iowait;
        long totalTime = user + nice + system + idle + iowait + irq + softirq + steal;

        long totalDelta = totalTime - prevTotal;
        long idleDelta = idleTime - prevIdle;

        double cpuPercent = 0.0;
        if (totalDelta > 0) {
            cpuPercent = 100.0 * (totalDelta - idleDelta) / totalDelta;
        }

        prevTotal = totalTime;
        prevIdle = idleTime;

        return cpuPercent;
    }
};

class MemCollector {
public:
    double read() {
        ifstream file("/proc/meminfo");
        string line;
        long memTotal = 0, memAvailable = 0;

        while (getline(file, line)) {
            istringstream ss(line);
            string label;
            long value;
            ss >> label >> value;

            if (label == "MemTotal:") memTotal = value;
            if (label == "MemAvailable:") memAvailable = value;
        }

        return 100.0 * (memTotal - memAvailable) / memTotal;
    }
};

class DiskCollector {
private:
    string path;

public:
    DiskCollector(const string& p = "/") : path(p) {}

    double read() {
        struct statvfs stat;
        if (statvfs(path.c_str(), &stat) != 0) {
            return -1.0;
        }

        unsigned long total = stat.f_blocks * stat.f_frsize;
        unsigned long free = stat.f_bfree * stat.f_frsize;
        unsigned long used = total - free;

        return 100.0 * used / total;
    }
};

class TempCollector {
private:
    string sensorPath;

public:
    TempCollector(const string& path = "/sys/class/thermal/thermal_zone0/temp")
        : sensorPath(path) {}

    double read() {
        ifstream file(sensorPath);
        if (!file.is_open()) {
            return -1.0;
        }
        long milliDegrees = 0;
        file >> milliDegrees;
        return milliDegrees / 1000.0;
    }
};

class Clock {
public:
    string now() {
        time_t t = time(nullptr);
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&t));
        return string(buf);
    }
};

class Logger {
private:
    ofstream file;

public:
    Logger(const string& filename) {
        file.open(filename, ios::app);
        file << "timestamp,cpu_percent,mem_percent,disk_percent,temp_celsius\n";
    }

    void log(const Metrics& m) {
        file << m.timestamp << "," << m.cpuPercent << "," << m.memPercent
             << "," << m.diskPercent << "," << m.tempCelsius << "\n";
        file.flush();
    }
};

class SystemMonitor {
private:
    CpuCollector cpuCollector;
    MemCollector memCollector;
    DiskCollector diskCollector;
    TempCollector tempCollector;
    Clock clock;
    Logger logger;
    int intervalSeconds;

public:
    SystemMonitor(const string& logFile, int interval)
        : logger(logFile), intervalSeconds(interval) {}

    Metrics collect() {
        Metrics m;
        m.cpuPercent = cpuCollector.read();
        m.memPercent = memCollector.read();
        m.diskPercent = diskCollector.read();
        m.tempCelsius = tempCollector.read();
        m.timestamp = clock.now();
        return m;
    }

    void printMetrics(const Metrics& m) {
        cout << m.timestamp << " | CPU: " << m.cpuPercent
             << "% MEM: " << m.memPercent
             << "% DISK: " << m.diskPercent
             << "% TEMP: " << m.tempCelsius << "C\n";
    }

    void run() {
        while (true) {
            Metrics m = collect();
            printMetrics(m);
            logger.log(m);
            this_thread::sleep_for(chrono::seconds(intervalSeconds));
        }
    }
};

int main() {
    SystemMonitor monitor("activity_log.txt", 10);
    monitor.run();
    return 0;
}