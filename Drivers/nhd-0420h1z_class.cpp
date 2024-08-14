


class LCDDriver
{

public:
    /** Create an LCDDriver for use with the shift register
    *
    *    @param chip_select DigitalOut object used as CS line
    *    @param serial_port SPI object to communicate with the shitf register
    */
    LCDDriver(DigitalOut* chip_select, SPI* serial_port)
        : m_CS {chip_select}
        , m_serial_port {serial_port}

private:
    DigitalOut* m_CS;
    SPI* m_serial_port;
};
