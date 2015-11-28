// Helper function for XMLHttpRequests
function sendDataRequest(action, url) {
    var req = new XMLHttpRequest();
    var response;

    req.open(action, url, false);
    req.send(null);
    if (req.readyState == 4 && req.status == 200) {
        response = JSON.parse(req.responseText);
    }

    return response;
}

Pebble.addEventListener('ready', function(e) {
});

// Called when incoming message from the Pebble is recevied
Pebble.addEventListener('appmessage', function(e) {
});

// Called when the configuration window is opened
Pebble.addEventListener('showConfiguration', function(e) {
    Pebble.openURL('https://anilist.co/api/auth/authorize?grant_type=authorization_code&client_id=navies-tm6ox&redirect_uri=pebblejs://close&response_type=code');
});

// Called when the configuration window is closed
Pebble.addEventListener("webviewclosed", function(e) {
    console.log(e.response);
    var code = e.response.match(/=\S{1,}&/)[0];
    // Trim extra trapped characters from regexp
    code = code.slice(1, code.length - 1);

    // Exchange code for access token
    var req = new XMLHttpRequest();
    req.open('POST', 'https://anilist.co/api/auth/access_token?grant_type=authorization_code&client_id=navies-tm6ox&client_secret=uhVelNwXkO3go0CHSwuwsJO6&redirect_uri=pebblejs://close&code=' + code, false);
    req.send(null);

    var response = JSON.parse(req.responseText);
    console.log(response.access_token);
    // Store the access token into phone storage
    localStorage.setItem('access_token', response.access_token);
});