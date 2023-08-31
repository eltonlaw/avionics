/* i2c.c - i2c serial communication AVR api
 *
 * Control unit monitors the TWI bus and when some certain events happen the
 * TWI interrupt flag/TWINT is asserted. In the next clock cycle the TWSR is updated
 * with a status code. TWSR only contains relevant info when TWI interrupt flag
 * is asserted. At all other times, the TWSR contains a special status code
 * indicating no relevant status info is available. When the TWI interrupt flag
 * is set, the SCL line is held low. Application software can complete some tasks before
 * allowing the TWI transmission to continue.
 *
 * TWINT flag set in following situations. Interrupts are issued after all bus
 * events. AVR TWI is byte oriented and interrupt based
 *
 * - After the TWI transmitted a START/REPEATED START condition
 * - After the TWI transmitted SLA+R/W
 * - After the TWI transmitted an addres byte
 * - After the TWI lost arbitration
 * - After the TWI addressed by own slave address or general call
 * - After the TWI received a data byte
 * - After a STOP or REPEATED START has been received while still addressed as a slave
 * - When a bus occur has occured due to an illegal START or STOP condition
 *
 * "(N)ACK register not directly accessible by the application software.  When
 * receiving, it can be set or cleared by manipulating TWCR. Value of the
 * received (N)ACK bit can be determined by the value in the TWSR."
 *
 * "When operating as master, arbitration detection hardware continuously
 * monitors the transmission trying to determine if arbitrartion is in process.
 * If the TWI has lost an arbitration, the control unit is informed"
 *
 * REGISTERS (all are 8 bits)
 * ---------
 * TWDR: In transmit mode, contains the next byte to be transmitted. IN receive mode, contains the last byte received.
 *
 * TWCR (control register): Certain bits cause things to happen and others show status
 * - 0/TWIE (interrupt enable)
 * - 1/not used
 * - 2/TWEN (i2c enable): Needs to be set for anything to happen. If this turns
 *   off, i2c "is switched off and all transmissions are terminated regardless"
 *   of ongoing operation
 * - 3/TWWC (write collision flag)[Read-Only]
 * - 4/TWSTO (stop condition):
 * - 5/TWSTA (start condition): Generates a START condition if the i2c bus is
 *   free, otherwise waits until a STOP condition detected. Must be cleared by
 *   software when the START condition has been transmitted
 * - 6/TWEA (enable ack):
 * - 7/TWINT (interrupt flag): Set by hardware when i2c interface finished
 *   current task (reads/writes). While the TWINT flag is set, the SCL low
 *   period is stretched. This is basically a pause for the app to figure out
 *   what to do. "TWINT flag must be cleared by software by writing a logic
 *   one to it". Clearing the flag starts the operation of the TWI
 *
 * TWSR (status register):
 * - 0-1/TWPS0-TWPS1: Setting the bit rate using the prescaler. Usually just left at the defaults
 * - 2/not used: Always read as zero
 * - 3-7/TWS3-TWS7 [Read only]: 5 bit status code of the i2c bus. The prescaler
 *   bits should be masked to zero when checking the status bits. So that
 *   status checking is independent of prescaler setting.
 */
#include <avr/io.h>
#include <util/twi.h>

#include "error.h"
#include "log.h"
#include "i2c.h"

// FIXME: will likely need to parameterize this when
// there are  other speeds for SCL
// Make sure that PRWTI (bit 7 of PRR/Power Reduction Register) is not enabled
// (which disables TWI to reduce usage). By default it's 0/enabled
void i2c_init(unsigned long bus_speed) {
    // Set prescaler value to 1.  Atmel suggests keeping it as 1 and twiddling
    // with TWBR only
    TWSR = 0x00;

    unsigned long base_freq = F_CPU / bus_speed;
    TWBR = (uint8_t) ((base_freq - 16) / 2);
    // enable twi (avr's name for i2c) in the control register
    TWCR |= (1 << TWEN);
}


/* Wait for status (until TWINT resets zero) */
void i2c_wait_for_complete() {
    loop_until_bit_is_set(TWCR, TWINT);
}

/* Use registers to tell processor to send the i2c start condition (SDA low
 * then SCL low). */
avcerr_t i2c_start() {
    begin:
    // Have to write a 1 to TWINT to clear the flag. TWI will still be paused
    // otherwise. After setting this register, the MCU will send a falling edge
    // on SDA while SCL is held high
    TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWSTA));
    i2c_wait_for_complete();
    switch (TW_STATUS) {
        case TW_REP_START:
        case TW_START:
            break;
        case TW_MT_ARB_LOST:
            goto begin;
        default:
            log_error("Failed to initialize: %x");
            return E_UNKNOWN;
    }
    return E_OK;
}

void assert_status(uint8_t expected) {
    if (TW_STATUS != expected) {
        panic("Expected %x, got %x", expected, TW_STATUS);
    }
}

void i2c_stop() {
    // After setting this register, the MCU will send a rising edge
    // on SDA while SCL is held high

    TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWSTO));
}

uint8_t i2c_read_ack() {
    TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWEA));
    i2c_wait_for_complete();
    assert_status(TW_MR_DATA_ACK);
    return TWDR;
}

uint8_t i2c_read_nack() {
    TWCR = (_BV(TWINT) | _BV(TWEN));
    i2c_wait_for_complete();
    assert_status(TW_MR_DATA_NACK);
    return TWDR;
}

void i2c_write(uint8_t data) {
    // "...writable when the TWI is not in the process of shifting a byte, this occurs"
    TWDR = data;
    TWCR = (_BV(TWINT) | _BV(TWEN));
    i2c_wait_for_complete();
}

void i2c_write_sla_w(uint8_t data) {
    i2c_write(data);
    assert_status(TW_MT_SLA_ACK);
}

void i2c_write_data(uint8_t data) {
    i2c_write(data);
    assert_status(TW_MT_DATA_ACK);
}

uint8_t i2c_status(void) {
    return TW_STATUS;
}
