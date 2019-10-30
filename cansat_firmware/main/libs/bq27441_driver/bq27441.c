#include "bq27441.h"
#include <stdbool.h>
#include <stdint.h>

#include "esp_timer.h"
#include "i2c_manager/i2c_manager.h"
#include "config/cansat.h"
#include "freertos/FreeRTOS.h"
#include "driver/i2c.h"

#include "esp_log.h"
static const char* TAG = "bq27441";

#define constrain(amt,low,high) ((int8_t)(amt)<(int8_t)(low)?(int8_t)(low):((int8_t)(amt)>(int8_t)(high)?(int8_t)(high):(int8_t)(amt)))

static bool _sealFlag = false;              // Global to identify that IC was previously sealed
static bool _userConfigControl = false;     // Global to identify that user has control over entering/exiting config

// Read a specified number of bytes over I2C at a given subAddress
static esp_err_t i2c_read_bytes(uint8_t subAddress, uint8_t * dest, uint8_t count)
{
    esp_err_t err = i2c_manager_read_register_multiple(GENERAL_I2C_NUMBER, BQ27441_I2C_TIMEOUT / portTICK_PERIOD_MS,
     BQ27441_I2C_ADDRESS, subAddress, count, dest);

	return err;
}

// Write a specified number of bytes over I2C to a given subAddress
static esp_err_t i2c_write_bytes(uint8_t subAddress, uint8_t * src, uint8_t count)
{
	esp_err_t err = i2c_manager_write_register_multiple(GENERAL_I2C_NUMBER, BQ27441_I2C_TIMEOUT / portTICK_PERIOD_MS,
     BQ27441_I2C_ADDRESS, subAddress, count, src);

	return err;
}

/**
    Issue a block_data_control() command to enable BlockData access
    
    @return true on success
*/
static bool block_data_control(void)
{
    uint8_t enableByte = 0x00;
	return i2c_write_bytes(BQ27441_EXTENDED_CONTROL, &enableByte, 1) == ESP_OK;
}

/**
    Issue a DataClass() command to set the data class to be accessed
    
    @param id is the id number of the class
    @return true on success
*/
static bool block_data_class(uint8_t id)
{
    return i2c_write_bytes(BQ27441_EXTENDED_DATACLASS, &id, 1) == ESP_OK;
}

/**
    Issue a DataBlock() command to set the data block to be accessed
    
    @param offset of the data block
    @return true on success
*/
static bool block_data_offset(uint8_t offset)
{
    return i2c_write_bytes(BQ27441_EXTENDED_DATABLOCK, &offset, 1) == ESP_OK;
}

// Read the current checksum using block_data_checksum()
static uint8_t block_data_checksum(void)
{
    uint8_t csum;
	i2c_read_bytes(BQ27441_EXTENDED_CHECKSUM, &csum, 1);
	return csum;
}

// Use BlockData() to read a byte from the loaded extended data
static uint8_t read_block_data(uint8_t offset)
{
    uint8_t ret = 0;
	uint8_t address = offset + BQ27441_EXTENDED_BLOCKDATA;
	i2c_read_bytes(address, &ret, 1);
	return ret;
}

/**
    Use BlockData() to write a byte to an offset of the loaded data
    
    @param offset is the position of the byte to be written
            data is the value to be written
    @return true on success
*/
static bool write_block_data(uint8_t offset, uint8_t data)
{
    uint8_t address = offset + BQ27441_EXTENDED_BLOCKDATA;
	return i2c_write_bytes(address, &data, 1) == ESP_OK;
}

/**
    Read all 32 bytes of the loaded extended data and compute a 
    checksum based on the values.
    
    @return 8-bit checksum value calculated based on loaded data
*/
static uint8_t compute_block_checksum(void)
{
    uint8_t data[32];
	i2c_read_bytes(BQ27441_EXTENDED_BLOCKDATA, data, 32);

	uint8_t csum = 0;
	for (int i=0; i<32; i++)
	{
		csum += data[i];
	}
	csum = 255 - csum;
	
	return csum;
}

/**
    Use the block_data_checksum() command to write a checksum value
    
    @param csum is the 8-bit checksum to be written
    @return true on success
*/
static bool write_block_checksum(uint8_t csum)
{
    return i2c_write_bytes(BQ27441_EXTENDED_CHECKSUM, &csum, 1) == ESP_OK;
}

/**
    Read a 16-bit subcommand() from the BQ27441-G1A's control()
    
    @param function is the subcommand of control() to be read
    @return 16-bit value of the subcommand's contents
*/	
static uint16_t read_control_word(uint16_t function)
{
	uint8_t subCommandMSB = (function >> 8);
	uint8_t subCommandLSB = (function & 0x00FF);
	esp_err_t err;

    // Acquire the I2C module
    if((err = i2c_manager_acquire(GENERAL_I2C_NUMBER, pdMS_TO_TICKS(200))) != ESP_OK)
    {
        ESP_LOGW(TAG, "Error acquiring port");
        return 0;
    }

    // Create the commands
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, BQ27441_I2C_ADDRESS, true);
    i2c_master_write_byte(cmd, 0x00, true);		// Always 0x00 for control()
	i2c_master_write_byte(cmd, subCommandLSB, true);
    i2c_master_stop(cmd);
    if((err = i2c_master_cmd_begin(GENERAL_I2C_NUMBER, cmd, pdMS_TO_TICKS(200))) != ESP_OK)
	{
		ESP_LOGE(TAG, "Error sending control() byte: %d", err);
	}
    i2c_cmd_link_delete(cmd);
	// Delay
	int64_t start = esp_timer_get_time();
	while(esp_timer_get_time() - start < 1000);

	// Send first sub-control byte
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, BQ27441_I2C_ADDRESS, true);
	i2c_master_write_byte(cmd, 0x01, true);		// Always 0x00 for control()
	i2c_master_write_byte(cmd, subCommandMSB, true);
	i2c_master_stop(cmd);
	if((err = i2c_master_cmd_begin(GENERAL_I2C_NUMBER, cmd, pdMS_TO_TICKS(200))) != ESP_OK)
	{
		ESP_LOGE(TAG, "Error sending sub-control LSB byte: %d", err);
	}
	i2c_cmd_link_delete(cmd);	
	// Delay
	start = esp_timer_get_time();
	while(esp_timer_get_time() - start < 1000);

	// Release the bus
    i2c_manager_release(GENERAL_I2C_NUMBER);

	// Send second sub-control byte and read the 2 bytes
	uint8_t data[2] = {0};
	err = i2c_manager_read_register_multiple(GENERAL_I2C_NUMBER, pdMS_TO_TICKS(200), BQ27441_I2C_ADDRESS, 0x00, 2, data);
	// Delay
	start = esp_timer_get_time();
	while(esp_timer_get_time() - start < 1000);

    if(err != ESP_OK)
	{
		ESP_LOGE(TAG, "Error sending sub-control MSB byte and reading: %d", err);
		return 0;
	}

	// Return data read
	return ((uint16_t)data[1] << 8) | data[0];
}

/**
    Execute a subcommand() from the BQ27441-G1A's control()
    
    @param function is the subcommand of control() to be executed
    @return true on success
*/	
static bool execute_control_word(uint16_t function)
{
	uint8_t subCommandMSB = (function >> 8);
	uint8_t subCommandLSB = (function & 0x00FF);
	esp_err_t err1, err2;

    // Acquire the I2C module
    if((err1 = i2c_manager_acquire(GENERAL_I2C_NUMBER, pdMS_TO_TICKS(200))) != ESP_OK)
    {
        ESP_LOGW(TAG, "Error acquiring port: %d", err1);
        return 0;
    }

    // Create the commands
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, BQ27441_I2C_ADDRESS, true);
    i2c_master_write_byte(cmd, 0x00, true);		// Always 0x00 for control()
	i2c_master_write_byte(cmd, subCommandLSB, true);
    i2c_master_stop(cmd);
    if((err1 = i2c_master_cmd_begin(GENERAL_I2C_NUMBER, cmd, pdMS_TO_TICKS(200))) != ESP_OK)
	{
		ESP_LOGE(TAG, "Error sending control() byte on execute_control_word(): %d", err1);
	}
    i2c_cmd_link_delete(cmd);
	// Delay
	int64_t start = esp_timer_get_time();
	while(esp_timer_get_time() - start < 1000);

	// Send first sub-control byte
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, BQ27441_I2C_ADDRESS, true);
	i2c_master_write_byte(cmd, 0x01, true);		// Always 0x01 for control()
	i2c_master_write_byte(cmd, subCommandMSB, true);
	i2c_master_stop(cmd);
	if((err2 = i2c_master_cmd_begin(GENERAL_I2C_NUMBER, cmd, pdMS_TO_TICKS(200))) != ESP_OK)
	{
		ESP_LOGE(TAG, "Error sending sub-control LSB byte on execute_control_word(): %d", err2);
	}
	i2c_cmd_link_delete(cmd);	
	// Delay
	start = esp_timer_get_time();
	while(esp_timer_get_time() - start < 1000);

	// Release the bus
    i2c_manager_release(GENERAL_I2C_NUMBER);

	if(err1 == ESP_OK && err2 == ESP_OK)
	{
		return true;
	}
	return false;
}

/**
    Read a 16-bit command word from the BQ27441-G1A
    
    @param subAddress is the command to be read from
    @return 16-bit value of the command's contents
*/	
static uint16_t read_word(uint8_t subAddress)
{
    uint8_t data[2] = {0};
	i2c_read_bytes(subAddress, data, 2);
	return ((uint16_t) data[1] << 8) | data[0];
}

/**
    Check if the BQ27441-G1A is sealed or not.
    
    @return true if the chip is sealed
*/
static bool sealed(void)
{
    uint16_t stat = bq27441_get_status();
	return stat & BQ27441_STATUS_SS;
}

/**
    Seal the BQ27441-G1A
    
    @return true on success
*/
static bool seal(void)
{
    return read_control_word(BQ27441_CONTROL_SEALED);
}

/**
    UNseal the BQ27441-G1A
    
    @return true on success
*/
static bool unseal(void)
{
    // To unseal the BQ27441, write the key to the control
	// command. Then immediately write the same key to control again.
	if (execute_control_word(BQ27441_UNSEAL_KEY))
	{
		if(!execute_control_word(BQ27441_UNSEAL_KEY))
		{
			return false;
		}

		// Check if unseal was executed
		if(!sealed())
		{
			return true;
		}
	}

	return false;
}
    
/**
    Read the 16-bit op_config register from extended data
    
    @return op_config register contents
*/
static uint16_t op_config(void)
{
    return read_word(BQ27441_EXTENDED_OPCONFIG);
}

/**
    Write a specified number of bytes to extended data specifying a 
    class ID, position offset.
    
    @param classID is the id of the class to be read from
            offset is the byte position of the byte to be read
            data is the data buffer to be written
            len is the number of bytes to be written
    @return true on success
*/
static bool write_extended_data(uint8_t classID, uint8_t offset, uint8_t * data, uint8_t len)
{
	ESP_LOGV(TAG, "Writting extended data with class ID %d and offset %d", classID, offset);

    if (len > 32)
		return false;
	
	if (!_userConfigControl) 
	{
		ESP_LOGV(TAG, "write_extended_data(): entering config mode");
		if(!bq27441_enter_config(false))
		{
			return false;
		}
	}
	
	// Enable block data memory control
	ESP_LOGV(TAG, "write_extended_data(): enable block data control");
	if (!block_data_control())
	{
		return false;
	}
	 // Write class ID using DataBlockClass()
	ESP_LOGV(TAG, "write_extended_data(): set block data class");
	if (!block_data_class(classID))
	{
		return false;
	}
	
	// Write 32-bit block offset (usually 0)
	ESP_LOGV(TAG, "write_extended_data(): set block data offset");
	if(!block_data_offset(offset / 32))
	{
		return false;
	}
	compute_block_checksum(); 			// Compute checksum going in
	uint8_t oldCsum = block_data_checksum();
	ESP_LOGV(TAG, "write_extended_data(): old checksum -> %d", oldCsum);

	// Write data bytes
	ESP_LOGV(TAG, "write_extended_data(): write block data");
	for (int i = 0; i < len; i++)
	{
		// Write to offset, mod 32 if offset is greater than 32
		// The block_data_offset above sets the 32-bit block
		if(!write_block_data((offset % 32) + i, data[i]))
		{
			ESP_LOGE(TAG, "write_extended_data(): error writting block data in position %d", i);
		}
	}
	
	// Write new checksum using block_data_checksum (0x60)
	uint8_t newCsum = compute_block_checksum(); // Compute the new checksum
	ESP_LOGV(TAG, "write_extended_data(): writting new checksum -> %d", newCsum);
	if(!write_block_checksum(newCsum))
	{
		ESP_LOGE(TAG, "Error writting new checksum");
	}

	ESP_LOGV(TAG, "write_extended_data(): Finish!");
	if (!_userConfigControl)
	{
		bq27441_exit_config(true);
	}
	
	return true;
}

/**
    Write the 16-bit op_config register in extended data
    
    @param New 16-bit value for op_config
    @return true on success
*/	
static bool write_op_config(uint16_t value)
{
    uint8_t opConfigMSB = value >> 8;
	uint8_t opConfigLSB = value & 0x00FF;
	uint8_t opConfigData[2] = {opConfigMSB, opConfigLSB};
	
	// OpConfig register location: BQ27441_ID_REGISTERS id, offset 0
	return write_extended_data(BQ27441_ID_REGISTERS, 0, opConfigData, 2);
}

/**
    Issue a soft-reset to the BQ27441-G1A
    
    @return true on success
*/	
static bool soft_reset(void)
{
    return execute_control_word(BQ27441_CONTROL_SOFT_RESET);
}

/**
    Read a byte from extended data specifying a class ID and position offset
    
    @param classID is the id of the class to be read from
            offset is the byte position of the byte to be read
    @return 8-bit value of specified data
*/
static uint8_t read_extended_data(uint8_t classID, uint8_t offset)
{
    uint8_t retData = 0;
	if (!_userConfigControl) bq27441_enter_config(false);
		
	if (!block_data_control()) // // enable block data memory control
		return false; // Return false if enable fails
	if (!block_data_class(classID)) // Write class ID using DataBlockClass()
		return false;
	
	block_data_offset(offset / 32); // Write 32-bit block offset (usually 0)
	
	compute_block_checksum(); // Compute checksum going in
	//uint8_t oldCsum = block_data_checksum();
	/*for (int i=0; i<32; i++)
		Serial.print(String(read_block_data(i)) + " ");*/
	retData = read_block_data(offset % 32); // Read from offset (limit to 0-31)
	
	if (!_userConfigControl) bq27441_exit_config(true);
	
	return retData;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////// END OF STATIC FUNCTIONS ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

// Initializes I2C and verifies communication with the BQ27441.
bool bq27441_init(void)
{
	//ESP_LOGV(TAG, "bq27441 in bus?: %d", i2c_manager_slave_exists(GENERAL_I2C_NUMBER, pdMS_TO_TICKS(200), BQ27441_I2C_ADDRESS));
	ESP_LOGV(TAG, "bq27441_init()");
	uint16_t deviceID = bq27441_get_device_type(); // Read deviceType from BQ27441;
	ESP_LOGV(TAG, "Device ID: %d", deviceID);
	
	if (deviceID == BQ27441_DEVICE_ID)
	{
		return true; // If device ID is valid, return true
	}
	
	return false; // Otherwise return false
}

// Configures the design capacity of the connected battery.
bool bq27441_set_capacity(uint16_t capacity)
{
	// Write to STATE subclass (82) of BQ27441 extended memory.
	// Offset 0x0A (10)
	// Design capacity is a 2-byte piece of data - MSB first
	uint8_t capMSB = capacity >> 8;
	uint8_t capLSB = capacity & 0x00FF;
	uint8_t capacityData[2] = {capMSB, capLSB};
	return write_extended_data(BQ27441_ID_STATE, 10, capacityData, 2);
}

// Reads and returns the battery voltage
uint16_t bq27441_get_voltage(void)
{
	return read_word(BQ27441_COMMAND_VOLTAGE);
}

// Reads and returns the specified current measurement
int16_t bq27441_get_current(current_measure_t type)
{
	int16_t current = 0;
	switch (type)
	{
        case AVG:
            current = (int16_t) read_word(BQ27441_COMMAND_AVG_CURRENT);
            break;
        case STBY:
            current = (int16_t) read_word(BQ27441_COMMAND_STDBY_CURRENT);
            break;
        case MAX:
            current = (int16_t) read_word(BQ27441_COMMAND_MAX_CURRENT);
            break;
	}
	
	return current;
}

// Reads and returns the specified capacity measurement
uint16_t bq27441_get_capacity(capacity_measure_t type)
{
	uint16_t capacity = 0;
	switch (type)
	{
        case REMAIN:
            return read_word(BQ27441_COMMAND_REM_CAPACITY);
            break;
        case FULL:
            return read_word(BQ27441_COMMAND_FULL_CAPACITY);
            break;
        case AVAIL:
            capacity = read_word(BQ27441_COMMAND_NOM_CAPACITY);
            break;
        case AVAIL_FULL:
            capacity = read_word(BQ27441_COMMAND_AVAIL_CAPACITY);
            break;
        case REMAIN_F: 
            capacity = read_word(BQ27441_COMMAND_REM_CAP_FIL);
            break;
        case REMAIN_UF:
            capacity = read_word(BQ27441_COMMAND_REM_CAP_UNFL);
            break;
        case FULL_F:
            capacity = read_word(BQ27441_COMMAND_FULL_CAP_FIL);
            break;
        case FULL_UF:
            capacity = read_word(BQ27441_COMMAND_FULL_CAP_UNFL);
            break;
        case DESIGN:
            capacity = read_word(BQ27441_EXTENDED_CAPACITY);
	}
	
	return capacity;
}

// Reads and returns measured average power
int16_t bq27441_get_power(void)
{
	return (int16_t) read_word(BQ27441_COMMAND_AVG_POWER);
}

// Reads and returns specified state of charge measurement
uint16_t bq27441_get_soc(soc_measure_t type)
{
	uint16_t socRet = 0;
	switch (type)
	{
        case FILTERED:
            socRet = read_word(BQ27441_COMMAND_SOC);
            break;
        case UNFILTERED:
            socRet = read_word(BQ27441_COMMAND_SOC_UNFL);
            break;
	}
	
	return socRet;
}

// Reads and returns specified state of health measurement
uint8_t bq27441_get_soh(soh_measure_t type)
{
	uint16_t sohRaw = read_word(BQ27441_COMMAND_SOH);
	uint8_t sohStatus = sohRaw >> 8;
	uint8_t sohPercent = sohRaw & 0x00FF;
	
	if (type == PERCENT)	
		return sohPercent;
	else
		return sohStatus;
}

// Reads and returns specified temperature measurement
uint16_t bq27441_get_temperature(temp_measure_t type)
{
	uint16_t temp = 0;
	switch (type)
	{
        case BATTERY:
            temp = read_word(BQ27441_COMMAND_TEMP);
            break;
        case INTERNAL_TEMP:
            temp = read_word(BQ27441_COMMAND_INT_TEMP);
            break;
	}
	return temp;
}

// Get GPOUT polarity setting (active-high or active-low)
bool bq27441_gpout_polarity(void)
{
	uint16_t opConfigRegister = op_config();
	
	return (opConfigRegister & BQ27441_OPCONFIG_GPIOPOL);
}

// Set GPOUT polarity to active-high or active-low
bool bq27441_set_gpout_polarity(bool activeHigh)
{
	uint16_t oldOpConfig = op_config();
	
	// Check to see if we need to update op_config:
	if ((activeHigh && (oldOpConfig & BQ27441_OPCONFIG_GPIOPOL)) ||
        (!activeHigh && !(oldOpConfig & BQ27441_OPCONFIG_GPIOPOL)))
		return true;
		
	uint16_t newOpConfig = oldOpConfig;
	if (activeHigh)
		newOpConfig |= BQ27441_OPCONFIG_GPIOPOL;
	else
		newOpConfig &= ~(BQ27441_OPCONFIG_GPIOPOL);
	
	return write_op_config(newOpConfig);	
}

// Get GPOUT function (BAT_LOW or SOC_INT)
bool bq27441_get_gpout_function(void)
{
	uint16_t opConfigRegister = op_config();
	
	return (opConfigRegister & BQ27441_OPCONFIG_BATLOWEN);	
}

// Set GPOUT function to BAT_LOW or SOC_INT
bool bq27441_set_gpout_function(gpout_function_t function)
{
	uint16_t oldOpConfig = op_config();
	
	// Check to see if we need to update op_config:
	if ((function && (oldOpConfig & BQ27441_OPCONFIG_BATLOWEN)) ||
        (!function && !(oldOpConfig & BQ27441_OPCONFIG_BATLOWEN)))
		return true;
	
	// Modify BATLOWN_EN bit of op_config:
	uint16_t newOpConfig = oldOpConfig;
	if (function)
		newOpConfig |= BQ27441_OPCONFIG_BATLOWEN;
	else
		newOpConfig &= ~(BQ27441_OPCONFIG_BATLOWEN);

	// Write new op_config
	return write_op_config(newOpConfig);
}

// Get SOC1_Set Threshold - threshold to set the alert flag
uint8_t bq27441_soc1_set_threshold(void)
{
	return read_extended_data(BQ27441_ID_DISCHARGE, 0);
}

// Get SOC1_Clear Threshold - threshold to clear the alert flag
uint8_t bq27441_soc1_clear_threshold(void)
{
	return read_extended_data(BQ27441_ID_DISCHARGE, 1);	
}

// Set the SOC1 set and clear thresholds to a percentage
bool bq27441_set_soc1_thresholds(uint8_t set, uint8_t clear)
{
	uint8_t thresholds[2];
	thresholds[0] = constrain(set, 0, 100);
	thresholds[1] = constrain(clear, 0, 100);
	return write_extended_data(BQ27441_ID_DISCHARGE, 0, thresholds, 2);
}

// Get SOCF_Set Threshold - threshold to set the alert flag
uint8_t bq27441_socf_set_threshold(void)
{
	return read_extended_data(BQ27441_ID_DISCHARGE, 2);
}

// Get SOCF_Clear Threshold - threshold to clear the alert flag
uint8_t bq27441_socf_clear_threshold(void)
{
	return read_extended_data(BQ27441_ID_DISCHARGE, 3);	
}

// Set the SOCF set and clear thresholds to a percentage
bool bq27441_set_socf_thresholds(uint8_t set, uint8_t clear)
{
	uint8_t thresholds[2];
	thresholds[0] = constrain(set, 0, 100);
	thresholds[1] = constrain(clear, 0, 100);
	return write_extended_data(BQ27441_ID_DISCHARGE, 2, thresholds, 2);
}

// Check if the SOC1 flag is set
bool bq27441_get_soc_flag(void)
{
	uint16_t flagState = bq27441_get_flags();
	
	return flagState & BQ27441_FLAG_SOC1;
}

// Check if the SOCF flag is set
bool bq27441_get_socf_flag(void)
{
	uint16_t flagState = bq27441_get_flags();
	
	return flagState & BQ27441_FLAG_SOCF;
}

// Get the SOC_INT interval delta
uint8_t bq27441_get_soci_delta(void)
{
	return read_extended_data(BQ27441_ID_STATE, 26);
}

// Set the SOC_INT interval delta to a value between 1 and 100
bool bq27441_set_soci_delta(uint8_t delta)
{
	uint8_t soci = constrain(delta, 0, 100);
	return write_extended_data(BQ27441_ID_STATE, 26, &soci, 1);
}

// Pulse the GPOUT pin - must be in SOC_INT mode
bool bq27441_pulse_gpout(void)
{
	return execute_control_word(BQ27441_CONTROL_PULSE_SOC_INT);
}

// Read the device type - should be 0x0421
uint16_t bq27441_get_device_type(void)
{
	return read_control_word(BQ27441_CONTROL_DEVICE_TYPE);
}

// Enter configuration mode - set userControl if calling from an Arduino sketch
// and you want control over when to bq27441_exit_config
bool bq27441_enter_config(bool userControl)
{
	ESP_LOGV(TAG, "bq27441_enter_config()");
	if (userControl) _userConfigControl = true;
	
	if (sealed())
	{
		ESP_LOGV(TAG, "Unsealing");
		_sealFlag = true;
		unseal(); // Must be unsealed before making changes
	}
	
	if (execute_control_word(BQ27441_CONTROL_SET_CFGUPDATE))
	{
		// Wait for CFGUPMODE to be set
		ESP_LOGV(TAG, "Waiting to enter config mode");
		int16_t timeout = BQ27441_I2C_TIMEOUT;
		while ((timeout--) && (!(bq27441_get_flags() & BQ27441_FLAG_CFGUPMODE)))
		{
			// Wait 1 ms for a very rough timeout
			int64_t start = esp_timer_get_time();
			while(esp_timer_get_time() - start < 1000);
		}
		
		if (timeout > 0)
		{
			ESP_LOGV(TAG, "Config mode active");
			return true;
		}
		else
		{
			ESP_LOGV(TAG, "Time out!");
		}
	}
	else
	{
		ESP_LOGE(TAG, "Error executing control word BQ27441_CONTROL_SET_CFGUPDATE");
	}
	
	
	return false;
}

// Exit configuration mode with the option to perform a resimulation
bool bq27441_exit_config(bool resim)
{
	// There are two methods for exiting config mode:
	//    1. Execute the EXIT_CFGUPDATE command
	//    2. Execute the SOFT_RESET command
	// EXIT_CFGUPDATE exits config mode _without_ an OCV (open-circuit voltage)
	// measurement, and without resimulating to update unfiltered-SoC and SoC.
	// If a new OCV measurement or resimulation is desired, SOFT_RESET or
	// EXIT_RESIM should be used to exit config mode.
	if (resim)
	{
		if (soft_reset())
		{
			int16_t timeout = BQ27441_I2C_TIMEOUT;
			while ((timeout--) && ((bq27441_get_flags() & BQ27441_FLAG_CFGUPMODE)))
			{
				// Wait 1 ms for a very rough timeout
				int64_t start = esp_timer_get_time();
				while(esp_timer_get_time() - start < 1000);
			}

			if (timeout > 0)
			{
				ESP_LOGV(TAG, "Exit from config mode successfull");
				if (_sealFlag) seal(); // Seal back up if we IC was sealed coming in
				return true;
			}
			else
			{
				ESP_LOGV(TAG, "Time out to exit config mode!");
			}
			
		}
		else
		{
			ESP_LOGV(TAG, "Error applying soft reset");
		}
		
		return false;
	}
	else
	{
		return execute_control_word(BQ27441_CONTROL_EXIT_CFGUPDATE);
	}	
}

// Read the bq27441_get_flags() command
uint16_t bq27441_get_flags(void)
{
	uint16_t word = read_word(BQ27441_COMMAND_FLAGS);
	return word;
}

// Read the CONTROL_STATUS subcommand of control()
uint16_t bq27441_get_status(void)
{
	return read_control_word(BQ27441_CONTROL_STATUS);
}

uint16_t bq27441_read_flags(void)
{
	return read_word(BQ27441_COMMAND_FLAGS);
}