#include <string>
#include <thread>
#include <iostream>

#include "CppNet.h"
#include "Socket.h"

using namespace cppnet;

int index = 0;
std::string msg = "test msg => ";

std::string GetMsg() {
    index++;
    return (msg + std::to_string(index));
}

void WriteFunc(const Handle& handle, uint32_t len, uint32_t error) {
    if (error != CEC_SUCCESS) {
        std::cout << "[WriteFunc]  something error : " << error << std::endl;
    }
}

void ReadFunc(const Handle& handle, base::CBuffer* data, uint32_t len, uint32_t error) {
    if (error != CEC_CLOSED && error != CEC_CONNECT_BREAK) {
        std::cout << "[ReadFunc]" << std::endl;
        std::cout << *(data) << std::endl;
        data->Clear();
        std::cout << "Thread ID : " << std::this_thread::get_id() << std::endl;
        std::cout << "Read size : " << len << std::endl << std::endl;
        auto msg = GetMsg();
        handle->Write(msg.c_str(), msg.length());
    } else {
        std::cout << "Close" << std::endl;
    }
}

void ConnectFunc(const Handle& handle, uint32_t err) {
    if (err == CEC_SUCCESS) {
        std::string ip;
        uint16_t port;
        handle->GetAddress(ip, port);
        std::cout << "[ConnectFunc] : ip : " << ip << "port : " << port << std::endl;

    } else {
        std::cout << "[ConnectFunc] some thing error : " << err << std::endl;
    }
}

void DisConnectionFunc(const Handle& handle, uint32_t err) {
    std::cout << "[DisConnectionFunc] : " << err << std::endl;
}

int main() {

    cppnet::CCppNet net;
    net.Init(1);

    net.SetAcceptCallback(ConnectFunc);
    net.SetWriteCallback(WriteFunc);
    net.SetReadCallback(ReadFunc);
    net.SetDisconnectionCallback(DisConnectionFunc);

    net.ListenAndAccept("0.0.0.0", 8921);

    net.Join();
}