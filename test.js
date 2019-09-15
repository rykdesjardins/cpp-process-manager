function testHTTP() {
    console.log('Hello from Node!');
    console.log('Now keeping process alive while not blocking event loop');

    require('http').createServer().listen(8081);
}

function waitAndExit() {
    console.log("Hello from Node!");
    console.log("This process will exit soon.");
    setTimeout(() => {
        console.log("Event loop is empty, ready to exit");
    }, Math.round(Math.random() * 3000));
}

waitAndExit();
