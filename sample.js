var aria2 = require('./build/Release/node_aria2');

aria2.config({
  'dir': './temp/',
  'bt-save-metadata': true,
  'bt-metadata-only': true
});

var rain = aria2.download('http://www.soundjay.com/nature/rain-07.wav');
rain.start();

rain.on('start', function () {
  console.log('rain on start');
});

rain.on('pause', function () {
  console.log('rain on pause');
});

rain.on('stop', function () {
  console.log('rain on stop');
});

rain.on('complete', function (path) {
  console.log('rain on complete : ' + path);
});

rain.on('error', function (code) {
  console.log('rain on error : ' + code);
});

rain.on('progress', function (total, completed, speed) {
  var percent = total == 0 ? 0 : completed / total * 100;
  console.log('rain on progress : ' + completed + '/' + total + ' ' + parseInt(percent) + '% ' + parseInt(speed / 1024) + 'KiB/s');
});

process.on( 'SIGINT', function() {
  console.log( "\n SIGINT" );
});