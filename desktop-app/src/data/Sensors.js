export default {
    newSensor:{
        cansatIndex: 0,     // Default index for the selected CanSat
        id: '',              // ID = CMD Sent and received from CanSat
        type: '',           // Showed in table. We need translate it
        lastValue: 0,       // Last value obtained from the CanSat of this specific sensor
        minValue: 0,        // Min Value allowed by sensor
        maxValue: 0,        // Max Value allowed by sensor
        minThreshold: 0,    // Min threshold set by user
        maxThreshold: 0,    // Max threshold set by user
        unit: '',           // Unit measured by the sensor
        step: 1,            // Minimum step when we change threshold
        status: 'danger',   // If we get 'Ok' show 'fa-check' else we show 'fa-exclamation-triangle' in the table
        _type: 'user'       // Used to apply filters: 
                            //  1) If it's 'vector' we get (x,y,z) to track it.
                            //  2) If it's 'scalar' we get only one value to track.
                            //  3) If it´s 'power' same as 'scalar' but only shown in the power supplies table 
                            //  4) If was created by user we get 'user'. Structure equals to 'scalar'.
                            //  5) If it´s 'gps'
    },
    sensors:[
        { 
            cansatIndex: 0, 
            id: 1, 
            type: 'cansat.resources.sensors.vBatt',
            lastValue: 0,
            minValue: 3.6,
            maxValue: 4.2,
            minThreshold: 3.6,
            maxThreshold: 4.2,
            unit: 'V',
            step: 0.005, 
            status: 'ok',
            _type: 'power',
            clear: true     // Only the first element defines clear. This parameter determines if we need to clean the sensor array before loading it
        },
        { 
            cansatIndex: 0, 
            id: 2, 
            type: 'cansat.resources.sensors.iBatt',
            lastValue: 0,
            minValue: 0,
            maxValue: 1,
            minThreshold: 0,
            maxThreshold: 1,
            unit: 'A',
            step: 0.01, 
            status: 'danger',
            _type: 'power',
        },
        { 
            cansatIndex: 0, 
            id: 3, 
            type: 'cansat.resources.sensors.v3v3',
            lastValue: 0,
            minValue: 3.0,
            maxValue: 3.6,
            minThreshold: 3.0,
            maxThreshold: 3.5,
            unit: 'V',
            step: 0.005, 
            status: 'danger',
            _type: 'power',
        },
        { 
            cansatIndex: 0, 
            id: 4, 
            type: 'cansat.resources.sensors.i3v3',
            lastValue: 0,
            minValue: 0,
            maxValue: 300,
            minThreshold: 0,
            maxThreshold: 300,
            unit: 'mA',
            step: 1, 
            status: 'danger',
            _type: 'power',
        },
        { 
            cansatIndex: 0, 
            id: 5, 
            type: 'cansat.resources.sensors.v5v',
            lastValue: 0,
            minValue: 4.5,
            maxValue: 5.5,
            minThreshold: 4.5,
            maxThreshold: 5.5,
            unit: 'V',
            step: 0.005, 
            status: 'danger',
            _type: 'power',
        },
        { 
            cansatIndex: 0, 
            id: 6, 
            type: 'cansat.resources.sensors.i5v',
            lastValue: 0,
            minValue: 0,
            maxValue: 500,
            minThreshold: 0,
            maxThreshold: 500,
            unit: 'mA',
            step: 0.01, 
            status: 'danger',
            _type: 'power'
        },
        { 
            cansatIndex: 0, // Dummy sensor, is not used because it has an other structure but is necessary for not generate corrupt projects and missions
            id: 7,       
            _type: 'gps',
            hide: true     // Not show in UI
        },
        { 
            cansatIndex: 0,
            id: 8, 
            type: 'cansat.resources.sensors.giroscope',
            x: 0,
            y: 0,  
            z: 0,
            minValue: 0,
            minThreshold: 0, 
            maxValue: 2000,
            maxThreshold:2000,
            step: 25,  
            unit: '°/s',        
            status: 'danger',
            _type: 'vector'
        },
        { 
            cansatIndex: 0,
            id: 9, 
            type: 'cansat.resources.sensors.magnetometer',
            x: 0,
            y: 0,  
            z: 0,
            minValue: 0,
            minThreshold: 0, 
            maxValue: 1300,
            maxThreshold: 1300, 
            unit: 'µT', 
            step: 25,          
            status: 'danger',
            _type: 'vector'
        },
        { 
            cansatIndex: 0,
            id: 10, 
            type: 'cansat.resources.sensors.accelerometer',
            x: 0,
            y: 0,  
            z: 0,
            minValue: 0,
            minThreshold: 0, 
            maxValue: 16,
            maxThreshold: 16,  
            unit: 'g', 
            step: 0.1,          
            status: 'danger',
            _type: 'vector'
        },
        { 
            cansatIndex: 0,
            id: 11, 
            type: 'cansat.resources.sensors.orientation',
            x: 0,
            y: 0,  
            z: 0,
            minValue: 0,
            minThreshold: 0, 
            maxValue: 16,
            maxThreshold: 16,  
            unit: '°', 
            step: 0.1,          
            status: 'danger',
            _type: 'vector'
        },
        { 
            cansatIndex: 0,
            id: 12, 
            type: 'cansat.resources.sensors.temperature',
            lastValue: -10,
            minValue: -10,
            maxValue: 85, 
            minThreshold: -10, // Min limit set by user
            maxThreshold: 85, // Max limit set by user
            unit: '°C',
            step: 1,          
            status: 'danger',
            _type: 'scalar'
        },
        { 
            cansatIndex: 0,
            id: 13, 
            type: 'cansat.resources.sensors.humidity',
            lastValue: 0,
            minValue: 0,
            maxValue: 90,
            minThreshold: 0,
            maxThreshold: 90, 
            unit: '%', 
            step: 5,          
            status: 'danger',
            _type: 'scalar'
        },
        { 
            cansatIndex: 0, 
            id: 14, 
            type: 'cansat.resources.sensors.pressure',
            lastValue: 300,
            minValue: 300,
            maxValue: 1200,
            minThreshold: 300,
            maxThreshold: 1200,
            unit: 'mbar',
            step: 10,            
            status: 'danger',
            _type: 'scalar'
        },
        { 
            cansatIndex: 0, 
            id: 15, 
            type: 'cansat.resources.sensors.altitude',
            lastValue: 0,
            minValue: 0,
            maxValue: 65535,
            minThreshold: 0,
            maxThreshold: 65535,
            unit: 'm',
            step: 50,            
            status: 'danger',
            _type: 'scalar'
        }
    ]
}