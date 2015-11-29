var express = require('express');  // import the express module

var Timeline = require('pebble-api');

// create a new Timeline object with our API key passed as an environment variable
var timeline = new Timeline();

var app = express(); // create an express instance
app.set('port', (process.env.PORT || 5000)); // set the port on the instance

// push a pin to :userToken from our webservice
app.get('/senduserpin/:userToken', function(req, res) {
  var userToken = req.params.userToken;

  var pin = new Timeline.Pin({
    id: parseInt(Math.random() * (999999 - 100000) + 100000).toString(), // random pin id
    time: new Date(new Date().getTime() + 60*60*1000), // current date and time plus 1 hour
    layout: new Timeline.Pin.Layout({
      type: Timeline.Pin.LayoutType.GENERIC_PIN,
      tinyIcon: Timeline.Pin.Icon.PIN,
      title: 'Pin from Heroku!'
    })
  });

  timeline.sendUserPin(userToken, pin, function (err, body, resp) {
    if(err) {
      res.status(400);
      res.send(err);
    } else {
      res.send('Pin successfully pushed!');
    }
  });
});

// start the webserver
var server = app.listen(app.get('port'), function () {
  console.log('Package Tracker server listening on port %s', app.get('port'));
});

