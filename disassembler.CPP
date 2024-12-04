#include <iostream>
#include <unordered_map>
#include <cstdint>
#include <string>
#include <fstream>

using namespace std;

unordered_map<uint8_t, string> opcodeMap = {
    {0x01, "SET"},
    {0x02, "ADD"},
    {0x03, "SUB"},
    {0x04, "MUL"},
    {0x05, "MLI"},
    {0x06, "DIV"},
    {0x07, "DVI"},
    {0x08, "MOD"},
    {0x09, "MDI"},
    {0x0A, "AND"},
    {0x0B, "BOR"},
    {0x0C, "XOR"},
    {0x0D, "SHR"},
    {0x0E, "ASR"},
    {0x0F, "SHL"},
    {0x10, "IFB"},
    {0x11, "IFC"},
    {0x12, "IFE"},
    {0x13, "IFN"},
    {0x14, "IFG"},
    {0x15, "IFA"},
    {0x16, "IFL"},
    {0x17, "IFU"},
    {0x1A, "ADX"},
    {0x1B, "SBX"},
    {0x1E, "STI"},
    {0x1F, "STD"}
};

unordered_map<uint8_t, string> specialOpcodeMap = {
    {0x01, "JSR"},
    {0x08, "INT"},
    {0x09, "IAG"},
    {0x0A, "IAS"},
    {0x0B, "RFI"},
    {0x0C, "IAQ"},
    {0x10, "HWN"},
    {0x11, "HWQ"},
    {0x12, "HWI"}
};

const string REGISTERS[8]   = {"A", "B", "C", "X", "Y", "Z", "I", "J"};
const int    REGISTER_COUNT = 8;

string decodeOperand(uint8_t value, uint16_t nextWord = 0, bool in_b = false) 
{
    if(value < 0x07)
        return REGISTERS[value];
    else if (value < 0x0f)
        return "["+REGISTERS[value - REGISTER_COUNT] + "]";
    else if (value < 0x17)
        return "["+REGISTERS[value - REGISTER_COUNT*2]+" + "+to_string(nextWord)+"]"; 
    else if (value == 0x18) 
        return in_b ? "(PUSH / [--SP])" : "(POP / [SP++])"; // checks if the operand is in a or b
    else if (value == 0x19)
        return "[SP] / PEEK";
    else if (value == 0x1a)
        return "[SP + " + to_string(nextWord) + "] / PICK n";
    else if (value == 0x1b)
        return "SP";
    else if (value == 0x1c)
        return "PC";
    else if (value == 0x1d)
        return "EX";
    else if (value == 0x1e)
        return "[" + to_string(nextWord) + "]";
    else if (value == 0x1f)
        return to_string(nextWord);
    else if (value < 0x3f && !in_b)     // (only for a) literal value 0xffff-0x1e (-1..30)
        return to_string(value - 0x21); // -1 to 30 is represented by 0x20 to 0x3f
    else
        return "UNKNOWN OPERAND";
}

void disassemble(uint16_t word, uint16_t nextWord = 0)
{
    uint8_t opcode =  word & 0x1F;        // Extract ooooo
    uint8_t b      = (word >> 5) & 0x1F;  // Extract bbbbb
    uint8_t a      = (word >> 10) & 0x3F; // Extract aaaaaa

    // Check if the opcode is special: aaaaaaooooo00000
    if(opcode == 0x00) 
    {  
        if (specialOpcodeMap.find(b) != specialOpcodeMap.end()) // opcode occupies the b bits
            cout << specialOpcodeMap[b] << " " << decodeOperand(a, nextWord);
        else
            cout << "UNKNOWN SPECIAL OPCODE";
    } 
    else if(opcodeMap.find(opcode) != opcodeMap.end())
    {
        cout << opcodeMap[opcode];
        cout << " "  << decodeOperand(b, nextWord, true);
        cout << ", " << decodeOperand(a, nextWord);
    } 
    else
        cout << "UNKNOWN OPCODE";
}

int main()
{
    uint16_t word = 0x8752;     // Example instruction
    uint16_t nextWord = 0x0001; // Example next word
    disassemble(word, nextWord);

    return 0;
} 