export default {
    'vBatt': 0,
    'v3v3': 1,
    'v5v': 2,
    powerSuplies:[
        { 
            'cansatIndex': 0,
            'psIndex': 0,  // V Batt
            'voltage': "3.7 V",
            'current': "0 mA",
            'maxCurrent': "1 A",
            'status': "Active"
        },
        { 
            'cansatIndex': 0, 
            'psIndex': 1,   // 3.3 V
            'voltage': '3.3 V',
            'current': ' 0 mA',
            'maxCurrent': '300 mA',
            'status' : 'Error'
        }, 
        { 
            'cansatIndex': 0, 
            'psIndex': 2,   // 5 V
            'voltage': '5 V',
            'current': ' 0 mA',
            'maxCurrent': '500 mA',
            'status' : 'Error'
        }
    ]
}