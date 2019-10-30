using mcp2210_dll_m;
using Mono.Options;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace mcp2210_cli
{
    class Program
    {
        // Default MCP2210 VID and PID
        private const ushort DEFAULT_VID = 0x4d8;
        private const ushort DEFAULT_PID = 0xde;
        private const bool DEBUG = false;

        static int Main(string[] args)
        {
            // Default SPI parameters
            bool shouldShowHelp = false;
            uint deviceIndex = 0;
            uint pbaudRate2 = 1000000;
            uint pidleCsVal2 = 0xfff;       // Idle value for CS
            uint pactiveCsVal2 = 0x000;     // Active value for CS
            uint pcsToDataDly2 = 0;         // CS to data delay
            uint pdataToCsDly2 = 0;         // Last data byte to CS delay
            uint pdataToDataDly2 = 0;       // Delay between bytes
            uint ptxferSize2 = 4;           // Bytes to Transfer per SPI Transaction
            byte pspiMd2 = 0;               // SPI Mode
            uint csIndex = 0;
            String gpio = null;
            String data = null;

            // Set the command line parser
            var options = new OptionSet {
                    { "i|index=", $"index of the MCP2210 device (defaults to {deviceIndex})", (uint i) => deviceIndex = i },
                    { "b|baud=", $"baud rate in MHz (defaults to {pbaudRate2})", (uint b) => pbaudRate2 = b },
                    { "csToDataDly=", $"delay between CS assertion and data send in units of 100 us (defaults to {pcsToDataDly2})", (uint d) => pcsToDataDly2 = d },
                    { "dataToCsDly=", $"delay between the last byte sent and CD deassert in units of 100 us (defaults to {pdataToCsDly2})", (uint d) => pdataToCsDly2 = d },
                    { "dataToDataDly=", $"delay between bytes in units of 100 us (defaults to {pdataToDataDly2})", (uint d) => pdataToDataDly2 = d },
                    { "m|mode=", $"SPI mode (defaults to {pspiMd2})", (byte m) => pspiMd2 = m },
                    { "cs=", $"CS index (defaults to {csIndex})", (uint i) => csIndex = i },
                    { "d|data=", "Array of bytes to send in hex format separated by commas", s => data = s },
                    { "gpioW=[number][1|0]", "Write 1 or 0 to the specified GPIO (example: gpioW=21, write 1 to GPIO 2)", s => gpio = s },
                    { "h|help", "show this message and exit", h => shouldShowHelp = h != null },
                };
            try
            {
                // Parse the command line
                options.Parse(args);

                // Show help menu
                if (shouldShowHelp)
                {
                    options.WriteOptionDescriptions(Console.Out);
                    return 0;
                }

                // We always need data
                if (data == null)
                {
                    return 0;
                }
            }
            catch (OptionException e)
            {
                // output some error message
                Console.Write("greet: ");
                Console.WriteLine(e.Message);
                Console.WriteLine("Try `greet --help' for more information.");
            }

            // Search connected MCP2210 devices
            int devCount = MCP2210.M_Mcp2210_GetConnectedDevCount(DEFAULT_VID, DEFAULT_PID);
            if(DEBUG) Console.WriteLine(devCount + " devices found");

            if (devCount > 0)
            {
                // Connect to the MCP2210
                StringBuilder path = new StringBuilder();
                IntPtr deviceHandle = new IntPtr();
                int res;

                // Open the device first
                deviceHandle = MCP2210.M_Mcp2210_OpenByIndex(DEFAULT_VID, DEFAULT_PID, deviceIndex, path); 
                res = MCP2210.M_Mcp2210_GetLastError();
                if (res != MCP2210.M_E_SUCCESS)
                {
                    Console.WriteLine("Failed to open connection");
                    return -1;
                }

                // Check if any GPIO must be written
                if(gpio != null && gpio.Length == 2)
                {
                    uint gpioNumber = uint.Parse(gpio[0].ToString());
                    uint value = uint.Parse(gpio[1].ToString());
                    byte[] pins = new byte[MCP2210.M_MCP2210_GPIO_NR];

                    uint gpioValue = (uint)(1 << (int)gpioNumber);

                    // Set everything as GPIO
                    for(int n = 0; n < pins.Length; ++n)
                    {
                        pins[n] = (byte)MCP2210.M_MCP2210_PIN_DES_GPIO;
                    }

                    // Set GPIO
                    MCP2210.M_Mcp2210_SetGpioConfig(deviceHandle, (byte)MCP2210.M_MCP2210_VM_CONFIG, pins, gpioValue, 0, (byte)MCP2210.M_MCP2210_REMOTE_WAKEUP_DISABLED,
                        (byte)MCP2210.M_MCP2210_INT_MD_CNT_NONE, (byte)MCP2210.M_MCP2210_SPI_BUS_RELEASE_DISABLED);
                }

                // Convert the data string to bytes to send
                string[] hexStrings = data.Split(',');
                byte[] txData = new byte[hexStrings.Length], rxData = new byte[hexStrings.Length];
                for(int n = 0; n < hexStrings.Length; ++n)
                {
                    txData[n] = Convert.ToByte(hexStrings[n], 16);
                }

                // Create the CS mask
                uint csmask = (uint)((int)1 << (int)csIndex);

                // Set the SPI transaction length to the number of bytes that will be sent
                ptxferSize2 = (uint)(txData.Length > 65535 ? 65535 : txData.Length);

                // Configure the MCP2210 and send the data, everything in one call
                res = MCP2210.M_Mcp2210_xferSpiDataEx(deviceHandle, txData, rxData, ref pbaudRate2, ref ptxferSize2, csmask, ref pidleCsVal2, ref pactiveCsVal2, ref pcsToDataDly2,
                    ref pdataToCsDly2, ref pdataToDataDly2, ref pspiMd2);
                if (res != MCP2210.M_E_SUCCESS)
                {
                    MCP2210.M_Mcp2210_Close(deviceHandle);
                    Console.WriteLine(" Transfer error: " + res);
                    return res;
                }

                // Write the output data in hex format
                var txString = new StringBuilder(">TxData: ");
                var rxString = new StringBuilder(">RxData: ");
                for(int n = 0; n < txData.Length; ++n)
                {
                    txString.Append(txData[n].ToString("X"));
                    txString.Append(",");
                    rxString.Append(rxData[n].ToString("X"));
                    rxString.Append(",");
                }

                // Remove last comma
                txString.Length -= 1;
                rxString.Length -= 1;

                Console.WriteLine(txString);
                Console.WriteLine(rxString);

                MCP2210.M_Mcp2210_Close(deviceHandle);
            }

            return 0;
        }
    }
}
