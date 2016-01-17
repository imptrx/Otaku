class User < ActiveRecord::Base
  # Iterates through all users in the databsae and pushes pins to their timeline
  def self.push_pins
    User.all.each do |user|
      push_anime_pins(user)
    end
  end

  # Exchange the user's authorization code for an refresh token
  def get_refresh_token(authorization_code)
    base_uri = 'https://anilist.co/api'
    url = base_uri + 'auth/access_token?grant_type=refresh_token'
    url += '&client_id=navies-tm6ox'
    url += '&client_secret=' + ENV['ANILIST_CLIENT_SECRET']
    url += '&redirect_uri=' + 'pebblejs://close'
    url += '&code=' + authorization_code

    response = HTTParty.post(url)
    body = JSON.parse(response)

    return body['refresh_token']
  end

  # Exchange the user's refresh token for an access token
  def get_access_token(user)
    base_uri = 'https://anilist.co/api/'
    url = base_uri + 'auth/access_token?grant_type=refresh_token'
    url += '&client_id=navies-tm6ox'
    url += '&client_secret=' + ENV['ANILIST_CLIENT_SECRET']
    url += '&refresh_token=' + user.anilist_token

    response = HTTParty.post(url)
    body = JSON.parse(response)

    return body['access_token']
  end

  # Get a list of the user's airing anime and push it to the Pebble timeline
  def push_anime_pins(user)
    access_token = get_access_token(user)
    api = PebbleTimeline::API.new(ENV['PEBBLE_TIMELINE_API_KEY'])
    user_pin = PebbleTimeline::Pins.new(api, 'user', user.timeline_token)

    base_uri = 'https://anilist.co/api/'
    url = base_uri + 'user/airing?access_token=' + access_token
    response = HTTParty.get(url)
    body = JSON.parse(response.body)

    body.each do |show|
      # Pins are uniquely identified as the show title + episode number
      id = show['anime']['title_romaji'] + show['anime']['airing']['next_episode']
      time = show['anime']['time']
      layout = {
        :type => 'genericPin',
        :title => show['anime']['title_romaji'],
        :subtitle => 'Episode: ' + show['anime']['airing']['next_episode'],
        :tinyIcon => 'system://images/TV_SHOW'
      }
      user_pin.create(id: id, time: time, layout: layout)
    end
  end
end
