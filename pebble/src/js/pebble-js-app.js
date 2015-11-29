/**
 * Helper function for XMLHttpRequests
 *
 * @params request, The HTTP method
 * @params url, The URL of the request
 */
function sendDataRequest(request, url) {
    var req = new XMLHttpRequest();
    var response;

    req.open(request, url, false);
    req.send(null);
    if (req.readyState == 4 && req.status == 200) {
        response = JSON.parse(req.responseText);
    }

    return response;
}

/**
 * Helper function for adding pin to timeline
 * This function is from 'https://github.com/pebble-examples/timeline-push-pin'
 * 
 * @params pin, The JSON pin to insert
 * @params type, The type of request, either 'PUT' or 'DELETE'
 */
function timelineRequest(pin, type) {
    // The timeline public URL root
    var API_URL_ROOT = 'https://timeline-api.getpebble.com/';

    // User or shared?
    var url = API_URL_ROOT + 'v1/user/pins/' + pin.id;

    // Create XHR
    var xhr = new XMLHttpRequest();
    xhr.onload = function () {
        console.log('timeline: response received: ' + this.responseText);
    };
    xhr.open(type, url);

    // Get token
    Pebble.getTimelineToken(function(token) {
        // Add headers
        xhr.setRequestHeader('Content-Type', 'application/json');
        xhr.setRequestHeader('X-User-Token', '' + token);

        // Send
        xhr.send(JSON.stringify(pin));
        console.log('timeline: request sent.');
    }, function(error) { console.log('timeline: error getting timeline token: ' + error); });
}

/**
 * Get a list of the user's airing anime and push it to the Pebble timeline
 */
function getAiringAnime() {
    var access_token = localStorage.getItem('access_token');
    var data = sendDataRequest('GET', 'https://anilist.co/api/user/airing?access_token=' + access_token);
    if (data) {
        for (var i = 0; i < data.length; i++) {
            // The returned JSON object is a 'small model',
            // fetch the 'large model' to obtain more information about the anime
            var anime = sendDataRequest('GET', "https://anilist.co/api/anime/" + data[i].anime.id + "?access_token=" + access_token);
            // Maximum of 1024 bytes for the body of a Pebble timeline pin
            var total = Math.min(anime.description.length, 1000);
            var description = anime.description.slice(0, total);

            // Create the pin to be sent to the watch
            var pin = {
                'id': anime.title_romaji + anime.airing.next_episode,
                'time': anime.airing.time,
                'layout': {
                    'type': 'genericPin',
                    'title': anime.title_romaji,
                    'subtitle': 'Episode: ' + anime.airing.next_episode,
                    'body': description,
                    'tinyIcon': 'system://images/TV_SHOW'
                }
            };
            timelineRequest(pin, 'PUT');

            var message = {TITLE_KEY: anime.title_romaji, SUBTITLE_KEY: 'Episode: ' + anime.airing.next_episode.toString()};
            Pebble.sendAppMessage(message, null, null);
        }
    }
}

/**
 * Get a list of manga from the user's list
 */
function getMangaList() {
    var access_token = localStorage.getItem('access_token');
    // Grab userID for mangaList query
    var data = sendDataRequest('GET', 'https://anilist.co/api/user?access_token=' + access_token);
    if (data) {
        var user_id = data.id;
        var mangas = sendDataRequest('GET', 'https://anilist.co/api/user/' + user_id + '/mangalist?access_token=' + access_token);
        if (mangas) {
            for (var i = 0; i < mangas.lists.reading.length; i++) {
                var title = mangas.lists.reading[i].manga.title_english;
                var status = mangas.lists.reading[i].manga.publishing_status;
                console.log(title + ':' + status);
                var message = {TITLE_KEY: title, SUBTITLE_KEY: status};
                Pebble.sendAppMessage(message, null, null);
            }
        }
    }
}

Pebble.addEventListener('ready', function(e) {
    console.log(localStorage.getItem('access_token'));
});

// Called when incoming message from the Pebble is received
Pebble.addEventListener('appmessage', function(e) {
    console.log(JSON.stringify(e));
    switch(e.payload.CONTENT_KEY) {
        case "Anime":
            getAiringAnime();
        break;

        case "Manga":
            getMangaList();
        break;
    }
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