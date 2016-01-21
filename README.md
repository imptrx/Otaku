# Otaku Pebble Application
## Description
Otaku is a Pebble smartwatch application initally developed for [HackWestern 2](http://hackwestern2.devpost.com/). It pushes timely pins to the Pebble timeline to notify the user whenever a new episode of an user's followed anime is about to air.
Note that the app uses the [AniList API](https://anilist-api.readthedocs.org/en/latest/), so user's are required to have an AniList account and to give the app API permissions to edit, create or remove your AniList data.

## Screenshots
![](./screenshots/pebble_screenshot_2015-11-29_03-38-03.png)
![](./screenshots/pebble_screenshot_2015-11-29_03-38-11.png)
![](./screenshots/pebble_screenshot_2015-11-29_03-39-01.png)

## Building and Installing
The application is compiled with SDK v3.8.2 and physically tested on Basalt (Pebble Time/Time Steel) platforms.
For Aplite (Pebble OG) and Chalk (Pebble Time Round), testing has only been done through the emulators, and so feedback is greatly appreciated.
Some features for the server side portion of the application have yet to be completed.
However, there is a .pbw compiled with SDK 3.7 that pushes pins to the timeline through the phone whenever the user enters app configuration.

### Sideloading the app
```bash
# Make sure developer mode is active on your phone and then establish connection
$ pebble login
# Deletes all current build files in preparation for a clean build
$ pebble clean
# Install the watchapp on your Pebble
$ pebble install --cloudpebble
```
## Contributers
Contributions are always welcomed via the issue tracker, bug tracker and through pull requests.

- [Edward](https://github.com/navies)
- [Peter](https://github.com/peter-cbxu)
- [Melissa](https://github.com/SherlocksDoctor)
