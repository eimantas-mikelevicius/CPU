// Eimantas Mikelevičius ISF25/2
#include <fstream>
#include <iostream>

void IN(unsigned short& IR, unsigned char regs[], char& PC, unsigned char& eflags, std::ifstream& input_file)
{
    char temppaskr;

    unsigned char paskr = (IR >> 8) & 0b00000111;

    if (input_file.get(temppaskr))
    {
        regs[paskr] = (unsigned char)temppaskr;
        eflags &= ~(1 << 2);
    }
    else
    {
        regs[paskr] = 0;
        eflags |= (1 << 2);
    }
}

void MOVC(unsigned short& IR, unsigned char regs[], char& PC)
{
    unsigned char paskNR = (IR & 0b0000011100000000) >> 8;
    unsigned char konst = IR & 0b0000000011111111;
    regs[paskNR] = konst;
}

void MOV(unsigned short& IR, unsigned char regs[], char& PC)
{
    unsigned char paskr = (IR >> 8) & 0b00000111;
    unsigned char saltr = (IR >> 4) & 0b00000111;
    regs[paskr] = regs[saltr];
}

void OUT(unsigned short& IR, unsigned char regs[], char& PC, std::ofstream& output_file)
{
    unsigned char saltr = (IR >> 4) & 0b00000111;
    output_file << (char)regs[saltr];
}

void JMP(unsigned short& IR, unsigned char regs[], char& PC, unsigned char& eflags)
{
    signed char konst = (IR & 0b0000000011111111);
    PC = PC + konst - 2;
}

void JZ(unsigned short& IR, unsigned char regs[], char& PC, unsigned char& eflags)
{
    signed char konst = (IR & 0b0000000011111111);
    if(eflags & (1<<4))
    {
    PC = PC + konst - 2;
    }
}

void JNZ(unsigned short& IR, unsigned char regs[], char& PC, unsigned char& eflags)
{
    signed char konst = (IR & 0b0000000011111111);
    if((eflags & (1<<4)) == 0)
    {
    PC = PC + konst - 2;
    }
    
}

void JC(unsigned short& IR, unsigned char regs[], char& PC, unsigned char& eflags)
{
    signed char konst = (IR & 0b0000000011111111);
    if(eflags & (1<<0))
    {
        PC = PC + konst - 2;
    }
}

void JO(unsigned short& IR, unsigned char regs[], char& PC, unsigned char& eflags)
{
    signed char konst = (IR & 0b0000000011111111);
    if(eflags & (1<<5))
    {
        PC = PC + konst - 2;
    }
}

void JFE(unsigned short& IR, unsigned char regs[], char& PC, unsigned char& eflags)
{
    signed char konst = (IR & 0b0000000011111111);
    if(eflags & (1<<2))
    {
        PC = PC + konst - 2;
    }

}

void INC(unsigned short& IR, unsigned char regs[], char& PC)
{
    unsigned char paskr = (IR >> 8) & 0b00000111;
    regs[paskr]++;
    unsigned char rez = regs[paskr];
}

void DEC(unsigned short& IR, unsigned char regs[], char& PC)
{
    unsigned char paskr = (IR >> 8) & 0b00000111;
    regs[paskr]--;
    unsigned char rez = regs[paskr];
}

void ADD_op(unsigned short& IR, unsigned char regs[], char& PC, unsigned char& eflags)
{
    unsigned char paskr = (IR >> 8) & 0b00000111;
    unsigned char mode = (IR >> 7) & 0x01;
    unsigned char salt1 = (IR >> 4) & 0x07;
    unsigned char salt2 = (IR >> 0) & 0x07;

    unsigned char pirm;
    unsigned char antr;

    if (mode) 
    {
    pirm = regs[salt1];
    antr = regs[salt2];
    } 
    else
    {
    pirm = regs[salt2];
    antr = regs[salt1];
    }

    unsigned short rez = (unsigned short)pirm + (unsigned short)antr;

    regs[paskr] = (unsigned char)rez;

}

void XOR_op(unsigned short& IR, unsigned char regs[], char& PC, unsigned char& eflags)
{
    unsigned char paskr = (IR >> 8) & 0b00000111;
    unsigned char mode = (IR >> 7) & 0x01;
    unsigned char salt1 = (IR >> 4) & 0x07;
    unsigned char salt2 = (IR >> 0) & 0x07;

    if(mode){
        regs[paskr] = regs[salt1] ^ regs[salt2];
    }
    else{
        regs[paskr] = regs[paskr] ^ regs[salt1];
    }

    if(regs[paskr] == 0)
        eflags |= (1<<4);
    else
        eflags &= ~(1<<4);
}

void OR_op(unsigned short& IR, unsigned char regs[], char& PC,unsigned char &eflags)
{
    unsigned char paskr = (IR >> 8) & 0b00000111;
    unsigned char mode = (IR >> 7) & 0x01;
    unsigned char salt1 = (IR >> 4) & 0x07;
    unsigned char salt2 = (IR >> 0) & 0x07;

    if(mode){
        regs[paskr] = regs[salt1] | regs[salt2];
    }
    else{
        regs[paskr] = regs[paskr] | regs[salt1];
    }

    if(regs[paskr] == 0)
        eflags |= (1<<4);
    else
        eflags &= ~(1<<4);
}

void AND_op(unsigned short& IR, unsigned char regs[], char& PC,unsigned char &eflags)
{
    unsigned char paskr = (IR >> 8) & 0b00000111;
    unsigned char mode = (IR >> 7) & 0x01;
    unsigned char salt1 = (IR >> 4) & 0x07;
    unsigned char salt2 = (IR >> 0) & 0x07;

    if(mode){
        regs[paskr] = regs[salt1] & regs[salt2];
    }
    else{
        regs[paskr] = regs[paskr] & regs[salt1];
    }

    if(regs[paskr] == 0)
        eflags |= (1<<4);
    else
        eflags &= ~(1<<4);
}

void LSR_op(unsigned short& IR, unsigned char regs[], char& PC,unsigned char &eflags)
{
    unsigned char paskr = (IR >> 8) & 0b00000111;
    unsigned char salt1 = (IR >> 4) & 0x07;
    regs[paskr] = regs[salt1] >> 1;
    if(regs[salt1] & 1){
        eflags |= (1<<0);
    }
    else{
        eflags &= ~(1<<0);
    }

    if(regs[paskr] == 0)
        eflags |= (1<<4);
    else
        eflags &= ~(1<<4);
}

void LSL_op(unsigned short& IR, unsigned char regs[], char& PC, unsigned char &eflags)
{
    unsigned char paskr = (IR >> 8) & 0b00000111;
    unsigned char salt1 = (IR >> 4) & 0x07;

    regs[paskr] = regs[salt1] << 1;

    if(regs[salt1] & (1<<7)){
        eflags |= (1<<0);
    }
    else{
        eflags &= ~(1<<0);
    }

    if(regs[paskr]==0){
        eflags |= (1<<4);
    }
    else{
        eflags &= ~(1<<4);
    }
}

void SUB_op(unsigned short& IR, unsigned char regs[], char& PC, unsigned char& eflags)
{
    unsigned char paskr = (IR >> 8) & 0b00000111;
    unsigned char mode = (IR >> 7) & 0x01;
    unsigned char salt1 = (IR >> 4) & 0x07;
    unsigned char salt2 = (IR >> 0) & 0x07;

    unsigned char pirm;
    unsigned char antr;

    if (mode) 
    {
    pirm = regs[salt1];
    antr = regs[salt2];
    } 
    else
    {
    pirm = regs[salt2];
    antr = regs[salt1];
    }

    unsigned short rez = (unsigned short)pirm - (unsigned short)antr;

    regs[paskr] = (unsigned char)rez;
}

int main()
{
    unsigned char regs[8];
    unsigned char RAM[256];
    char PC = 0;
    unsigned short IR = 0;
    unsigned char eflags = 0b00000010;

    std::ifstream bin_file;
    bin_file.open("genialu.bin", std::ios::in | std::ios::binary);

    std::ifstream input_file;
    input_file.open("negenialu.txt", std::ios::in);

    std::ofstream output_file;
    output_file.open("rez.txt", std::ios::out);

    char symbol;
    int count = 0;
    while (bin_file.get(symbol)) {
        RAM[(unsigned char)count] = (unsigned char)symbol;
        count++;
    }
    bin_file.close();

    PC = 0;

    while (true)
    {
        unsigned char byte1 = RAM[(unsigned char)PC++];
        unsigned char byte2 = RAM[(unsigned char)PC++];

        IR = (byte1 << 8) | byte2;

        unsigned char opcode = (IR >> 11) & 0x1F;

        if (opcode == 0x01) IN(IR, regs, PC, eflags, input_file);
        else if (opcode == 0x02) OUT(IR, regs, PC, output_file);
        else if (opcode == 0x03) MOV(IR, regs, PC);
        else if (opcode == 0x04) MOVC(IR, regs, PC);
        else if (opcode == 0x05) JMP(IR, regs, PC,eflags);
        else if (opcode == 0x06) JZ(IR, regs, PC, eflags);
        else if (opcode == 0x07) JNZ(IR, regs, PC, eflags);
        else if (opcode == 0x08) JC(IR, regs, PC, eflags);
        else if (opcode == 0x09) JO(IR, regs, PC, eflags);
        else if (opcode == 0x0A) JFE(IR, regs, PC, eflags);
        else if (opcode == 0x0C) INC(IR, regs, PC);
        else if (opcode == 0x0D) DEC(IR, regs, PC);
        else if (opcode == 0x0E) ADD_op(IR, regs, PC, eflags);
        else if (opcode == 0x0F) SUB_op(IR, regs, PC, eflags);
        else if (opcode == 0x10) XOR_op(IR, regs, PC, eflags);
        else if (opcode == 0x11) OR_op(IR, regs, PC, eflags);
        else if (opcode == 0x12) AND_op(IR, regs, PC, eflags);
        else if (opcode == 0x13) LSR_op(IR, regs, PC, eflags);
        else if (opcode == 0x14) LSL_op(IR, regs, PC, eflags);
        else if (opcode == 0x00) break; // HALT

    }

    input_file.close();
    output_file.close();
    return 0;
}