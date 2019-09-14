console.log('Hello from node!');
console.log('Now keeping process alive while not blocking event loop');

require('http').createServer().listen(8081);
// setTimeout(() => {}, 1000);
