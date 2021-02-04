void i2c_MasterInit(int baudrate){
    SSPSTAT=0x80;
    SSPCON=0x28;
    SSPCON2=0x00;
    TRISC3=1;
    TRISC4=1;
}
void i2c_SlaveInit(int address){
    SSPSTAT=0x80;
    SSPCON=0x26;
    SSPCON2=0x00;
    SSPADD=address;
    SSPIF=0;
    SSPIE=1;
    PEIE=1;
    GIE=1;
    TRISC3=1;
    TRISC4=1;
}
void i2c_Start(){
    SEN=1;
}