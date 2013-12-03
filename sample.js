var aria2 = require('./build/Release/node_aria2');

aria2.config({
  'dir': './temp/',
  'bt-save-metadata': true,
  'bt-metadata-only': true
});

var dice = aria2.download('http://freedownloads.last.fm/download/612562195/Black%2BDice.mp3');

dice.on('start', function () {
  console.log('dice start');
});

dice.on('pause', function () {
  console.log('dice pause');
});

dice.on('stop', function () {
  console.log('dice stop');
});

dice.on('complete', function (path) {
  console.log('dice complete : ' + path);
});

dice.on('error', function (code) {
  console.log('dice error : ' + code);
});

dice.on('progress', function (total, completed, speed) {
  var percent = completed / total * 100;
  console.log('dice progress : ' + completed + '/' + total + ' ' + parseInt(percent) + '% ' + parseInt(speed / 1024) + 'KiB/s');
});

dice.start();
