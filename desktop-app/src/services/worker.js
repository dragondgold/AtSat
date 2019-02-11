const path = require('path');
const cc1101 = require('./cc1101.js');
let pathMCP2210CLI = path.resolve('./../../build/MCP2210/MCP2210CLI.exe');
process.send(cc1101.cc1101_init(pathMCP2210CLI));

process.on('message', (msg) => {
    console.log('Message from parent:', msg);
  });

  let counter = 0;
  
  setInterval(() => {
    process.send({ counter: counter++ });
  }, 1000);
