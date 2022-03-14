#include <iostream>
#include <Windows.h>
#include<conio.h>

HANDLE COM_1;
HANDLE COM_2;

const LPCTSTR NAME_PORT_1 = L"COM1";
const LPCTSTR NAME_PORT_2 = L"COM2";

void dcb_state(DCB& dcb);
void read_from_port();
void write_in_port(char data_to_write);

int main(){
    COM_1 = ::CreateFile(NAME_PORT_1, GENERIC_WRITE, 0,  nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    COM_2 = ::CreateFile(NAME_PORT_2, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    DCB dcb = { 0 }; 
    dcb_state(dcb);
    std::cout << "Input a character:";
    write_in_port(_getch());
    read_from_port();
}

void read_from_port() {
    DWORD recieved_size;
    char recieved_data;

    if (!ReadFile(COM_2, &recieved_data, 1, &recieved_size, 0)) {
        std::cout<<std::endl << "Error reading port!" << std::endl;
        exit(-1);
    }
    std::cout<< std::endl << "Recieved data(COM-2): " << recieved_data << std::endl;
}

void write_in_port(char data) {
    DWORD size = sizeof(data);
    DWORD written_bytes;

    if (!WriteFile(COM_1, &data, size, &written_bytes, 0)) {
        std::cout<< std::endl << "Error writing byte!" << std::endl;
        exit(-1);
    }
    std::cout<< std::endl << "Written ("<<written_bytes<<" bytes) in port(COM - 1) : " << data <<std::endl;
}


void dcb_state(DCB& dcb) {
    dcb.DCBlength = sizeof(dcb);
    if (!GetCommState(COM_1, &dcb)) {
        std::cout<< std::endl << "Error setting port state!" << std::endl;
        exit(-1);
    }
    dcb.BaudRate = CBR_9600;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;
    if (!SetCommState(COM_2, &dcb)) {
        exit(-1);
    }
}