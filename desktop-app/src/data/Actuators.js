export default {
    parachute: 0,
    balloon: 1,
    actuators:[
        { 
            cansatIndex: 0,
            actuatorIndex: 0, 
            type:'cansat.resources.actuators.parachute',
            status: 'cansat.resources.close',
            clear: true     // Only the first element defines clear. This parameter determines if we need to clean the sensor array before loading it
        },
        { 
            cansatIndex: 0,
            actuatorIndex: 1, 
            type:'cansat.resources.actuators.balloon',
            status: 'cansat.resources.close'
        }
    ]
}