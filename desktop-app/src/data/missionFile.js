export default {
    location:{               // GPS data  
        lat: '',
        lng: '',    
        history:[]          // Location record
    },
    sensors:[   // Sensor array. We need set that to test and graph correctly
        {
            id: '',           // ID = CMD Sent and received from CanSat
            type: '',         // Showed in table. We need translate it
            status:'',        // If we get 'Ok' show 'fa-check' else we show 'fa-exclamation-triangle' in the table
            step: '',         // Minimum step when we change threshold
            minValue: '',     // Min Value allowed by sensor
            maxValue: '',     // Max Value allowed by sensor
            unit: '',         // Unit measured by the sensor
            cansatIndex:'',   // Default index for the selected CanSat
            _type: '',        // Used to apply filters: 
        }
    ]
}