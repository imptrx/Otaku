json.array!(@users) do |user|
  json.extract! user, :id, :timeline_token, :anilist_token
  json.url user_url(user, format: :json)
end
